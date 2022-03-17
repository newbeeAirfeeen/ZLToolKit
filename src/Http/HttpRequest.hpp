/*
 * @file_name: http_request_impl.hpp
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
#ifndef HTTP_REQUEST_IMPL_HPP
#define HTTP_REQUEST_IMPL_HPP
#include "HttpCommon.hpp"
#include "HttpBase.hpp"
#include "HttpMethod.hpp"
#include "Util/Url.hpp"
#include <memory>
namespace Http
{

  class HttpRequest : public HttpMessage
  {
  public:
    friend class HttpRequestHandler;
    friend class HttpRequestInvoker;
    friend std::ostream& operator << (std::ostream& os, const HttpRequest& other);
  public:
    using Super = HttpMessage;
    using Ptr = std::shared_ptr<HttpRequest>;

  public:
    HttpRequest() = default;

    template <typename T, typename = typename std::enable_if<Http::is_http_method<T>::value>::type>
    void setMethod(){
      _method = T::name;
    }

    const std::string& getMethod() const;
    /*
     * @description: 设置请求路径
     * @date: 2022/2/28
     * @param:  path为路径
     */
    void setPath(const std::string &path);

    /*
     * 得到请求路径
     * */
    const std::string &getRequestPath() const;
    /*
     * 设置请求参数
     * */
    template <typename T>
    void setParameter(const std::string &field, const T &val){
      _url.setParameter(field, std::to_string(val));
    }

    void setParameter(const std::string &field, const char *val);

    void setParameter(const std::string &field, const std::string &val);

    const std::string &getParameter(const std::string &key) const;

    const std::string &getParameter(const char *key) const;
  private:
    /*
     * 收到header body
     * */
    void onHeaderBody(std::unordered_map<std::string, std::string> &_header, std::string &header, std::string &body);
  private:
    std::string _method{"GET", 3};
    Url _url{"http://oaho.com/"};
    std::weak_ptr<HttpSession> _session;
  };

};
#endif
