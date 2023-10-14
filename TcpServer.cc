#include "TcpServer.h"

TcpServer::TcpServer(const string &ip, unsigned short port) : _acceptor(ip, port), _loop(_acceptor) {}

TcpServer::~TcpServer() {}

void TcpServer::start() {
  _acceptor.getReady();
  _loop.startLoop();
}

void TcpServer::stop() { _loop.stopLoop(); }

void TcpServer::setAllCallback(TcpCallBack &&cb1, TcpCallBack &&cb2, TcpCallBack &&cb3) {
  _loop.setConnectCallback(cb1);
  _loop.setMessageCallback(cb2);
  _loop.setCloseCallback(cb3);
}