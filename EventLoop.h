#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <sys/epoll.h>

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "Acceptor.h"
#include "TcpConnection.h"

using std::function;
using std::map;
using std::shared_ptr;
using std::vector;

class EventLoop {
  using TcpconPtr = shared_ptr<TcpConnection>;
  using TcpCallback = function<void(const TcpconPtr &)>;

 public:
  EventLoop(Acceptor &);
  ~EventLoop();
  void startLoop();
  void stopLoop();
  void waitOnEpoll();
  void setConnectCallback(TcpCallback &&);
  void setMessageCallback(TcpCallback &&);
  void setCloseCallback(TcpCallback &&);
  void handleConnect();
  void handleMessafe(int fd);
  void handleClose(int fd);

 private:
  int createEpollfd();
  void addToEpoll(int fd);
  void delFromEpoll(int fd);

 private:
  int _epfd;
  bool _isLooping;
  vector<struct epoll_event> _evtLists;
  map<int, TcpconPtr> _conns;
  Acceptor &_acceptor;
  // deal TCP half event
  TcpCallback _onConnect;
  TcpCallback _onMessage;
  TcpCallback _onClose;
};

#endif