/*
* @file_name: StringBody.cpp
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

#include "StringBody.hpp"
#include "../HttpSession.hpp"
#include <Network/Socket.h>
namespace Http{
  std::ostream& StringBody::print(std::ostream &os) const{
    os.write(std::string::data(), std::string::size());
    return os;
  }

  void StringBody::loadFromString(std::string &str){
    return std::string::swap(str);
  }

  const char* StringBody::getContentType() const{
    return "text/plain";
  }

  size_t StringBody::getContentLength() const{
    return std::string::size();
  }

  void StringBody::sendHttpBody(const std::shared_ptr<toolkit::Socket> &ptr) const {
    using namespace toolkit;
    using SuperType = std::string;
    ptr->send(SuperType::data(), SuperType::size(), nullptr, 0, true);
  }

};