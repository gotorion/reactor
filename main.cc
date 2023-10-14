#include "Acceptor.h"
#include "TcpConnection.h"
#include <iostream>
#include <unistd.h>

int main() {
  Acceptor ac("127.0.0.1", 7777);
  ac.getReady();
  int fd = ac.accept();
  TcpConnection con(fd);
  std::cout << "server is listening..." << std::endl;
  while (1) {
    string tmp = con.receive();
    sleep(2);
    std::cout << "receive msg from client: " << tmp << std::endl;
  }
  return 0;
}