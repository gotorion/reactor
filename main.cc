#include <unistd.h>

#include <iostream>

// #include "Acceptor.h"
// #include "EventLoop.h"
// #include "TcpConnection.h"
#include "TcpServer.h"
using namespace std::placeholders;
// using TcpconPtr = std::shared_ptr<TcpConnection>;

class Task {
 public:
  void processConn(const TcpconPtr& tp) { std::cout << tp->toString() << " connected!" << std::endl; }
  void processMsg(const TcpconPtr& tp) { std::cout << "->" << tp->receive() << std::endl; }
  void processClose(const TcpconPtr& tp) { std::cout << tp->toString() << " closed!" << std::endl; }
};

int main() {
  TcpServer server("127.0.0.1", 7777);
  Task tk;
  auto cb1 = std::bind(&Task::processConn, &tk, _1);
  auto cb2 = std::bind(&Task::processMsg, &tk, _1);
  auto cb3 = std::bind(&Task::processClose, &tk, _1);
  // loop.setConnectCallback(std::bind(&Task::processConn, &tk, _1));
  // loop.setMessageCallback(std::bind(&Task::processMsg, &tk, _1));
  // loop.setCloseCallback(std::bind(&Task::processClose, &tk, _1));

  // ac.getReady();
  // loop.startLoop();
  server.setAllCallback(cb1, cb2, cb3);
  server.start();
  return 0;
}