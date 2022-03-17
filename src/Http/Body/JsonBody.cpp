/*
* @file_name: JsonBody.cpp
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
#include "JsonBody.hpp"
namespace Http{

  std::ostream& JsonBody::print(std::ostream &os) const{
    Json::StreamWriterBuilder builder;
    auto str = std::move(Json::writeString(builder, *this));
    os << str;
    return os;
  }

  void JsonBody::loadFromString(std::string& str){
    Json::CharReaderBuilder read_builder;
    std::string err;
    std::unique_ptr<Json::CharReader> reader(read_builder.newCharReader());
    if (!reader)
      throw std::bad_alloc();
    Json::Value val;
    if (!reader->parse(str.data(), str.data() + str.size(), &val,
                       &err)){
      throw std::logic_error(err);
    }
    Json::Value::swap(val);
  }

  const char* JsonBody::getContentType() const{
    return "application/json";
  }

  size_t JsonBody::getContentLength() const{
    Json::StreamWriterBuilder builder;
    auto str = std::move(Json::writeString(builder, *this));
    return str.size();
  }
  void JsonBody::sendHttpBody(const std::shared_ptr<toolkit::Socket>& ptr) const {

  }
};