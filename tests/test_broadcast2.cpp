#include <Network/UdpServer.h>
#include <Util/logger.h>
#include <signal.h>
#include <mutex>
#include <Network/sockutil.h>
using namespace std;
using namespace toolkit;


class Broadcast : public UdpSession {
public:
  Broadcast(const Socket::Ptr& sock) : UdpSession(sock) {
    InfoL << SockUtil::setBroadcast(getSock()->rawFD(), true);
  }

  void onRecv(const Buffer::Ptr& buf) {

    InfoL << this->get_peer_ip() << ":" << this->get_peer_port() << ", " <<buf->data();
    SocketHelper::send(std::make_shared<BufferLikeString>("yes! I recieved"));
  }
  void onError(const SockException& err) {
		
    ErrorL << err.what();
  }
  void onManager() {


		
  }

private:
  std::once_flag flag;
};



int main() {
	
  static semaphore sem;
  signal(SIGINT, [](int) {sem.post(); });
	
	
  Logger::Instance().add(std::make_shared<ConsoleChannel>());
  Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());





  UdpServer::Ptr _server = std::make_shared<UdpServer>();

  _server->start<Broadcast>(7000);


  sem.wait();
  return 0;
}