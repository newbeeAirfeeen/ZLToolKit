/*
 * @file_name: HttpBody.hpp
 * @date: 2022/03/14
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
#ifndef _OAHO_HTTP_BODY
#define _OAHO_HTTP_BODY
#include <memory>
#include <string>
#include <iostream>
#include "../HttpCommon.hpp"
#include <Network/Socket.h>
namespace Http
{
    class HttpBody
    {
      friend class HttpRequestInvoker;
      friend class HttpResponseInvoker;
    public:
      using Ptr = std::shared_ptr<HttpBody>;
    public:
        virtual ~HttpBody() = default;
    public:
       /*
        * @description: 输出到流
        * @date: 2022/3/16
        * @param: std::ostream
        */
        virtual std::ostream& print(std::ostream& os) const = 0;
        /*
        * @description: 从string中构造body
        * @date: 2022/3/16
        * @param:string
        */
        virtual void loadFromString(std::string&) = 0;
        /*
        * @description: 得到body的mime类型
        * @date: 2022/3/16
        * @return: string描述
        */
        virtual const char* getContentType() const = 0;

        /*
        * @description: 得到body的长度
        * @date: 2022/3/17
        * @return: length
        */
        virtual size_t getContentLength() const = 0;
    protected:
        /*
        * @description: 发送http接口
        * @date: 2022/3/14
        * @param: http会话内会有文件描述符
        */
        virtual void sendHttpBody(const std::shared_ptr<toolkit::Socket>&) const = 0;
    };
} // namespace Http

#endif