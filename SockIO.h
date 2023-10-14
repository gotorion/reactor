#ifndef __SOCKIO_H_
#define __SOCKIO_H_

class SockIO {
public:
  SockIO(int fd);
  ~SockIO();
  int readn(char *buf, int len);
  int readLine(char *buf, int len);
  int writen(const char *buf, int len);

private:
  int _fd;
};

#endif