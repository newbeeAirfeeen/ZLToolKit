//
// Created by 沈昊 on 2022/2/25.
//
#include <iostream>
#include <Http/Http.hpp>
#include <Util/logger.h>
#include <Network/TcpServer.h>
#include <Http/Interceptors/HttpBasicInterceptor.hpp>
using namespace std;
using namespace toolkit;
using namespace Http;

void test_http_encode(){
  HttpRequest<JsonBody> request;
  request.setHeader<Http::HttpRequestHeader::accept>("*/*");
  request.setHeader<Http::HttpRequestHeader::host>(std::string("oaho' http request"));
  request.setHeader<Http::HttpRequestHeader::accept_encoding>("utf-8");
  request.setPath("/path");
  request.setParameter("name", "shenhao");
  request.setParameter("id", 1);
  request.body().set("name","shenhao");
  request.body().set("id", 1);
  std::cout << request.toString();
}

int main(){

  static semaphore sam;
  Logger::Instance().add(std::make_shared<ConsoleChannel>());
  //test_http_encode();
  TcpServer::Ptr server(new TcpServer());

  auto interceptor = Http::HttpRequestHandler::apiRegister<Http::JsonBody, Http::JsonBody>("/api",
        [](const typename Http::HttpRequest<Http::JsonBody>::Ptr& request,
        typename Http::HttpResponse<Http::JsonBody>::Ptr& response){
        InfoL << "收到http请求";
        response->setStatus<Http::Status::ok>();
        response->body().set("name", "shenhao");
        response->body().set("id", 1);

  });
  Http::HttpBasicInterceptor func;
  interceptor->addInterceptor(func);


  server->start<Http::HttpSession>(8080);
  //退出程序事件处理
  static semaphore sem;
  signal(SIGINT, [](int) { sem.post(); });// 设置退出信号
  sem.wait();
  return 0;
}