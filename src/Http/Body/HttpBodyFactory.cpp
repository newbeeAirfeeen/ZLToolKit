/*
 * @file_name: HttpBodyFactory.cpp
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
#include "HttpBodyFactory.hpp"
#include "StringBody.hpp"
#include "JsonBody.hpp"
#include "FileBody.hpp"
#include <unordered_map>
#include <Util/string_view.hpp>
namespace Http
{
  HttpBody::Ptr HttpBodyFactory::createHttpBody(const char *mime_type)
  {
    HttpBody::Ptr body_ptr;
    toolkit::string_view mime(mime_type, strlen(mime_type));
    size_t pos = mime.find("application/json");
    if (pos == 0)
      return std::make_shared<JsonBody>();
    return std::make_shared<StringBody>();
  }
};