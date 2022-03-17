/*
 * @file_name: JsonBody.hpp
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
#ifndef HTTP_JSON_BODY_HPP
#define HTTP_JSON_BODY_HPP
#include <jsoncpp/json.h>
#include <memory>
#include "HttpBody.hpp"
namespace Http {

  class JsonBody : public HttpBody, public Json::Value {
  public:
    ~JsonBody() override = default;

    std::ostream &print(std::ostream &os) const override;

    void loadFromString(std::string &string) override;

    const char *getContentType() const override;

    size_t getContentLength() const override;
  protected:
    void sendHttpBody(const std::shared_ptr<toolkit::Socket> &ptr) const override;
  };
}; // namespace http

#endif