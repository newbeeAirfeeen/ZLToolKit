//
// Created by 沈昊 on 2022/3/5.
//
#include <Util/logger.h>
#include <Network/Socket.h>
#include <Network/sockutil.h>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
using namespace toolkit;
int main(){

  Logger::Instance().add(std::make_shared<ConsoleChannel>());
  Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

  static semaphore sem;
  signal(SIGINT, [](int){ sem.post();});

  //
  Socket::Ptr _socket = Socket::createSocket();
  _socket->bindUdpSock(7001);
  //设置广播
  SockUtil::setBroadcast(_socket->rawFD(), true);


  _socket->setOnRead([](const Buffer::Ptr &buf, struct sockaddr *addr, int addr_len){
    InfoL << buf->data();
  });

  _socket->setOnErr([](const SockException &err){
    ErrorL << err.what();
  });


  struct sockaddr_in sock;
  memset(&sock, 0 , sizeof(sockaddr_in));
  sock.sin_family = AF_INET;
  sock.sin_port = htons(7000);
  auto ret = inet_pton(AF_INET, "192.168.31.255", &sock.sin_addr);
  InfoL << ret;
  //ret = _socket->bindPeerAddr((sockaddr*)&sock, sizeof(sockaddr_in));
  //InfoL << ret;


  std::weak_ptr<Socket> sock_self = _socket;
  _socket->getPoller()->doDelayTask(5000, [sock_self, sock]() -> uint64_t {
    auto stronger_self = sock_self.lock();
    if(!stronger_self){
      return 0;
    }
    //stronger_self->send("message bump!");
    stronger_self->send("message send ", (struct sockaddr*)&sock, sizeof(sock), true);
    return 5000;
  });

  sem.wait();
  return 0;
}