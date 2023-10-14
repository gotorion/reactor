#include <unistd.h>

#include <iostream>

#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"

using namespace std::placeholders;
using TcpconPtr = std::shared_ptr<TcpConnection>;

class Task {
 public:
  void processConn(const TcpconPtr& tp) {
    std::cout << tp->toString() << " connected!" << std::endl;
  }
  void processMsg(const TcpconPtr& tp) {
    std::cout << "->" << tp->receive() << std::endl;
  }
  void processClose(const TcpconPtr& tp) {
    std::cout << tp->toString() << " closed!" << std::endl;
  }
};

int main() {
  Acceptor ac("127.0.0.1", 7777);
  EventLoop loop(ac);
  Task tk;
  loop.setConnectCallback(std::bind(&Task::processConn, &tk, _1));
  loop.setMessageCallback(std::bind(&Task::processMsg, &tk, _1));
  loop.setCloseCallback(std::bind(&Task::processClose, &tk, _1));

  ac.getReady();
  loop.startLoop();

  return 0;
}