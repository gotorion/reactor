#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Acceptor.h"
#include "EventLoop.h"
using std::function;

using TcpconPtr = shared_ptr<TcpConnection>;
using TcpCallBack = function<void(TcpconPtr)>;

class TcpServer {
 public:
  TcpServer(const string& ip, unsigned short port);
  ~TcpServer();
  void start();
  void stop();
  void setAllCallback(TcpCallBack&& cb1, TcpCallBack&& cb2, TcpCallBack&& cb3);

 private:
  Acceptor _acceptor;
  EventLoop _loop;
};
#endif