//
// Created by 沈昊 on 2022/3/2.
//
#include "HttpBasicInterceptor.hpp"
#include <Util/logger.h>
using namespace toolkit;
namespace Http{

  bool HttpBasicInterceptor::operator()(HTTP_INTERCEPTOR_ARGS){
    InfoL << request_method << "/" << request_path.getPath();
    InfoL << request_body;
    return true;
  }
}
