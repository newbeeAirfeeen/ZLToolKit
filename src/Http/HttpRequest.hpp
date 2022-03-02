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
#include "HeaderFields.hpp"
#include "HttpBase.hpp"
#include "HttpMethod.hpp"
#include "Util/string_view.hpp"
#include "Util/Url.hpp"
#include <memory>
namespace Http{

    template<typename _body_type>
    class basic_message<true, HttpRequestHeader, _body_type>: protected http_message_base<HttpRequestHeader, _body_type>{
    public:
      friend class HttpRequestHandler;
      friend class HttpRequestInvoker;
    public:
      using base_header_type = http_message_base<HttpRequestHeader, _body_type>;
      using header_type      = typename base_header_type::header_type;
      using body_type        = typename base_header_type::body_type;
      using value_type       = typename base_header_type::value_type;
      using this_type        = basic_message<true, HttpRequestHeader, body_type>;
      using Ptr              = std::shared_ptr<this_type>;
    public:
      basic_message(){
        //加入content-type
        setHeader<Http::HttpRequestHeader::content_type>(body_type::content_type);
      }
      /*
       * 得到HttpHeader的字段值
       * */
      template<typename T, typename = typename std::enable_if<Http::is_request_field<T>::value>::type>
      const value_type& getHeader() const {
        return base_header_type::getHeader(T::value);
      }

      /*
       * 设置HttpHeader
       * */
      template<typename T, typename R, typename = typename std::enable_if<Http::is_request_field<T>::value>::type>
      void setHeader(const R& val){
        return base_header_type::setHeader(T::value, std::move(std::to_string(val)));
      }

      template<typename T, typename = typename std::enable_if<Http::is_request_field<T>::value>::type>
      void setHeader(const std::string& val){
        return base_header_type::setHeader(T::value, val);
      }

      template<typename T, typename = typename std::enable_if<Http::is_request_field<T>::value>::type>
      void setHeader(const char* val){
        return base_header_type::setHeader(T::value, val);
      }

      template<typename T, typename = typename std::enable_if<Http::is_http_method<T>::value>::type>
      void setMethod(){
          _method = T::name;
      }

      const std::string& getMethod() const{
          return _method;
      }
      /*
      * @description: 设置请求路径
      * @date: 2022/2/28
      * @param:  path为路径
      */
      void setPath(const std::string& path){
        if(path.empty() || path[0] != '/')
          throw std::logic_error("请求路径必须要以\"/\"开始");
        _url.setPath(path);
      }

      /*
       * 得到请求路径
       * */
      const std::string& getRequestPath() const{
        return _url.getPath();
      }
      /*
       * 设置请求参数
       * */
      template<typename T>
      void setParameter(const std::string& field, const T& val){
        _url.setParameter(field, std::to_string(val));
      }

      void setParameter(const std::string& field, const char* val){
        _url.setParameter(field, val);
      }

      void setParameter(const std::string& field, const std::string& val){
        _url.setParameter(field, val);
      }

      const std::string& getParameter(const std::string& key) const{
        return _url.getParameter(key.c_str());
      }

      const std::string& getParameter(const char* key) const{
        return _url.getParameter(key);
      }
      /*
      * @description: 得到对端的端口
      * @date: 2022/2/28
      * @param:
      * @return: 端口
      */
      unsigned short getRemotePort() const {
        return _url.getPort();
      }

      const body_type& getBody() const {
        return base_header_type::getBody_l();
      }

      void setBody(const body_type& body){
        return base_header_type::setBody_l(body);
      }

      body_type& body() {
        return base_header_type::body_l();
      }

      std::string toString() const {
        std::string url_encode  = _url.getPath();
        const auto& query_map = _url.getQueryParameters();
        if(isHttpMethod<Http::Method::get>(getMethod())){
          if(url_encode[url_encode.size() - 1] == '/'){
            url_encode.erase(url_encode.size() - 1);
            if(url_encode.empty() && !query_map.empty()){
              throw std::logic_error("当有请求参数的时候,必须指定请求路径");
            }
          }
          url_encode += "?";
          for(const auto& query : query_map){
            url_encode += query.first;
            url_encode += "=";
            url_encode += query.second;
            url_encode += "&";
          }
          url_encode.erase(url_encode.size() - 1);
        }
        std::string str = getMethod() + " " + url_encode + " HTTP/1.1\r\n";
        //刷新一下body内容
        getBody().flush();
        auto body_str = std::move(getBody().toString());
        if(body_type::need_content_length){
          str += Http::HttpRequestHeader::content_length::value;
          str += ": ";
          str += std::to_string(body_str.size());
          str += "\r\n";
        }
        str += std::move(base_header_type::toString());
        str += "\r\n";
        str += body_str;
        return std::move(str);
      }

      std::weak_ptr<HttpSession> getHttpSession() const{
        return _session;
      }

    public:
      bool isHttpRequest() const override{
        return true;
      }

      typename base_header_type::Ptr clone() const override{
        return nullptr;
      }
  private:
    /*
     * 收到header body
     * */
    void onHeaderBody(std::unordered_map<std::string, std::string>& _header, std::string& header, std::string& body){
      base_header_type::loadHeader(header, _header);
      /* 处理body */
      base_header_type::body_l().loadFromString(body);
    }
  private:
      std::string _method{"GET", 3};
      Url _url{"http://oaho.com/"};
      std::weak_ptr<HttpSession> _session;
    };

    template<typename body> using HttpRequest = basic_message<true, HttpRequestHeader, body>;
};
#endif

