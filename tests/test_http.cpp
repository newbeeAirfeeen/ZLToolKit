//
// Created by 沈昊 on 2022/2/25.
//
#include <Http/Http.hpp>
#include <Network/TcpServer.h>
#include <Util/logger.h>
using namespace std;
using namespace toolkit;
using namespace Http;

void test_http_encode(){

}

int main()
{

  static semaphore sam;
  Logger::Instance().add(std::make_shared<ConsoleChannel>());
  // test_http_encode();
  TcpServer::Ptr server(new TcpServer());

  HttpRequestHandler::apiRegister("/api", [](const HttpRequest& request, HttpResponse& response){
    InfoL << request;

    std::shared_ptr<Http::StringBody> body = std::make_shared<Http::StringBody>();
    body->append("yes", 3);
    response.setBody(body);

  });





  server->start<Http::HttpSession>(8080);
  //退出程序事件处理
  static semaphore sem;
  signal(SIGINT, [](int)
         { sem.post(); }); // 设置退出信号
  sem.wait();
  return 0;
}