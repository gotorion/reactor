#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include <functional>
#include <memory>
#include <string>

#include "InetAddress.h"
#include "SockIO.h"
#include "Socket.h"

using std::enable_shared_from_this;
using std::function;
using std::shared_ptr;
using std::string;
// ATTENTION: must be public inherit from [enable_shared_from_this]
class TcpConnection : public enable_shared_from_this<TcpConnection> {
  using TcpconPtr = shared_ptr<TcpConnection>;
  using TcpCallback = function<void(const TcpconPtr &)>;

 public:
  TcpConnection(int fd);
  ~TcpConnection();
  void send(const string &msg);
  string receive();
  bool isExit();
  void setConnect(const TcpCallback &);
  void setMessage(const TcpCallback &);
  void setClose(const TcpCallback &);
  void handleConnectCallback();
  void handleMessageCallback();
  void handleCloseCallback();
  // for debug
  string toString();

 private:
  InetAddress getLocalAddr();
  InetAddress getPeerAddr();

 private:
  Socket _sock;
  SockIO _sockIO;

  InetAddress _localAddr;
  InetAddress _peerAddr;

  TcpCallback _onConnect;
  TcpCallback _onMessage;
  TcpCallback _onClose;
};

#endif