#include "EventLoop.h"

#include <unistd.h>

#include <iostream>
#include <ostream>

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(createEpollfd()),
      _isLooping(false),
      _evtLists(1024),
      _conns(),
      _acceptor(acceptor),
      _onConnect(nullptr),
      _onMessage(nullptr),
      _onClose(nullptr) {
  _evtLists.reserve(1024);
}

EventLoop::~EventLoop() {}

void EventLoop::startLoop() {
  _isLooping = true;
  addToEpoll(_acceptor.fd());
  while (_isLooping) {
    waitOnEpoll();
  }
}

void EventLoop::waitOnEpoll() {
  int nready = 0;
  do {
    nready = ::epoll_wait(_epfd, &(*_evtLists.begin()), _evtLists.size(), -1);
  } while (-1 == nready && errno == EINTR);

  if (-1 == nready) {
    perror("epoll_wait");
    exit(-1);
  } else if (0 == nready) {
    ::printf("epoll_wait timeout\n");
  } else {  // two situation, either listenfd or oldfd
    if (nready == (int)_evtLists.size()) {
      _evtLists.resize(2 * nready);
    }
    for (int i = 0; i < nready; ++i) {
      struct epoll_event evt = _evtLists.at(i);
      if (evt.data.fd == _acceptor.fd()) {
        if (evt.events & EPOLLIN) {
          handleConnect();
        }
      } else {
        if (evt.events & EPOLLIN) {
          handleMessafe(evt.data.fd);
        }
      }
    }
  }
}

void EventLoop::stopLoop() { _isLooping = false; }

void EventLoop::setConnectCallback(TcpCallback &&cb) { _onConnect = std::move(cb); }

void EventLoop::setMessageCallback(TcpCallback &&cb) { _onMessage = std::move(cb); }

void EventLoop::setCloseCallback(TcpCallback &&cb) { _onClose = std::move(cb); }

void EventLoop::handleConnect() {
  int connfd = _acceptor.accept();
  TcpconPtr newTcp(new TcpConnection(connfd));
  newTcp->setConnect(_onConnect);
  newTcp->setMessage(_onMessage);
  newTcp->setClose(_onClose);
  addToEpoll(connfd);
  _conns.insert(std::make_pair(connfd, newTcp));
  newTcp->handleConnectCallback();
}

void EventLoop::handleMessafe(int fd) {
  auto iter = _conns.find(fd);
  if (iter == _conns.end()) {
    ::printf("Connection does not exist");
  } else {
    bool flag = iter->second->isExit();
    if (flag) {
      handleClose(fd);  // 可能是read的时候数据没有读完
    } else {
      iter->second->handleMessageCallback();
    }
  }
}

void EventLoop::handleClose(int fd) {
  delFromEpoll(fd);
  auto it = _conns.find(fd);
  it->second->handleCloseCallback();
  _conns.erase(fd);
  // close(fd);  // server close, work to do
}

// private
int EventLoop::createEpollfd() {
  int epfd = epoll_create(10);
  if (-1 == epfd) {
    perror("epoll_create");
    exit(-1);
  }
  return epfd;
}

void EventLoop::addToEpoll(int fd) {
  struct epoll_event evt;
  evt.events = EPOLLIN;
  // 这里监听要设置成EPOLLIN，之前设置成EPOLLIN|EPOLLOUT会使epoll一直就绪
  evt.data.fd = fd;
  int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
  if (ret) {
    perror("epoll_ctl_add");
    exit(-1);
  }
}

void EventLoop::delFromEpoll(int fd) {
  struct epoll_event evt;
  evt.events = EPOLLIN;
  evt.data.fd = fd;
  int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
  if (ret) {
    perror("epoll_ctl_del");
    exit(-1);
  }
}