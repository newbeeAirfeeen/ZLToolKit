/*
* @file_name: http_header_base.cpp
* @date: 2021/12/06
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
#include "HttpHeaderBase.hpp"
namespace Http{

http_header_base& http_header_base::operator = (http_header_base&& other){
  SuperType::operator=(std::move(other));
  return *this;
}

void http_header_base::loadHeader(std::string& header_str, SuperType& _header){
  this -> _header_str = std::move(header_str);
  SuperType::operator=(std::move(_header));
}

std::string http_header_base::toString() const {
  std::stringstream str;
  auto begin = SuperType::cbegin();
  auto end = SuperType::cend();
  while(begin != end){
    str << (*begin).first << ": " << (*begin).second << "\r\n";
    ++begin;
  }
  return std::move(str.str());
}
}