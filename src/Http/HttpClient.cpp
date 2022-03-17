/*
* @file_name: HttpClient.cpp
* @date: 2022/03/02
* @author: oaho
* Copyright @ hz oaho, All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "HttpClient.hpp"
#include "Util/util.h"
#include "Util/logger.h"
#include <string>
using namespace toolkit;
namespace Http{

  HttpClient::HttpClient(const std::string& host, size_t time_out, const EventPoller::Ptr &poller):Super(poller),
                                                                                                   _time_out(time_out){
    onCheckHost(host);
  }

  void HttpClient::setTimeout(size_t time_out){
    _time_out = time_out;
  }

  void HttpClient::setHost(const std::string& host){
    onCheckHost(host);
  }

  void HttpClient::onConnect(const SockException &ex){

  }

  void HttpClient::onRecv(const Buffer::Ptr &buf) {

  }

  void HttpClient::onErr(const SockException &ex) {
    ErrorL << ex.what();
  }

  void HttpClient::onCheckHost(const std::string& host){
    this->raw_host = host;
    this->host = host.substr(0, 7);
    toolkit::strToLower(this->host);
    if(this->host != "http://") {
      this->host.insert(0, "http://", 7);
    }
    this->host.append(host);
    if(raw_host.size() && raw_host[raw_host.size() - 1] == '/' ){
      raw_host.erase(raw_host.size() - 1);
    }
  }

  void HttpClient::onRecvHttp(header_type&, toolkit::string_view line, std::string& header, std::string& body){

  }

  void HttpClient::onRecvHttpChunked(header_type& , toolkit::string_view line, std::string& header, std::string& chunked_body, bool isEnd){

  }
}