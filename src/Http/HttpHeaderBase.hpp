/*
 * @file_name: HttpHeader.hpp
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
#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP
#include <unordered_map>
#include <sstream>
namespace Http
{

  class HttpHeader : public std::unordered_map<std::string, std::string>
  {
  public:
    using SuperType = std::unordered_map<std::string, std::string>;
    using field_type = typename SuperType::key_type;
    using value_type = typename SuperType::mapped_type;

  public:
    HttpHeader(HttpHeader &&other) noexcept : SuperType(std::move(other)) {}
    HttpHeader() = default;
    HttpHeader &operator=(HttpHeader &&other);
    /*
     * @description: 留给子类构造http请求头调用
     * @date: 2022/3/1
     * @param: _header_str 不带request_response的str
     * @param: _header 已构造好的map
     */
    void loadHeader(SuperType &_header);
  public:
    std::string toString() const;
  };
};

#endif // VIDEO_ON_DEMAND_HTTP_HEADER_HPP
