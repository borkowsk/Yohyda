<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <meta name="viewport" content="width=device-width,initial-scale=1.0" />
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>doc/html/boost_asio/example/cpp11/http/server/reply.cpp - 1.62.0</title>  <link rel="icon" href="/favicon.ico" type="image/ico" />
  <link rel="stylesheet" type="text/css" href="/style-v2/section-doc.css" />
  <!--[if IE 7]> <style type="text/css"> body { behavior: url(/style-v2/csshover3.htc); } </style> <![endif]-->

</head>

<body>
  <div id="heading">
    <div class="heading-inner">
  <div class="heading-placard"></div>

  <h1 class="heading-title">
  <a href="/">
  <img src="/gfx/space.png" alt= "Boost C++ Libraries" class="heading-logo" />
  <span class="heading-boost">Boost</span>
  <span class="heading-cpplibraries">C++ Libraries</span>
  </a></h1>

  <p class="heading-quote">
  <q>...one of the most highly
  regarded and expertly designed C++ library projects in the
  world.</q> <span class="heading-attribution">&mdash; <a href=
  "http://www.gotw.ca/" class="external">Herb Sutter</a> and <a href=
  "http://en.wikipedia.org/wiki/Andrei_Alexandrescu" class="external">Andrei
  Alexandrescu</a>, <a href=
  "http://safari.awprofessional.com/?XmlId=0321113586" class="external">C++
  Coding Standards</a></span></p>
</div>
  </div>
  <div class="boost-common-header-notice"><a class="boost-common-header-inner" href="/doc/libs/release/doc/html/boost_asio/example/cpp11/http/server/reply.cpp">This is the documentation for an old version of Boost.
                Click here to view this page for the latest version.</a></div>

  <div id="body">
    <div id="body-inner">
      <div id="content">
        <div class="section" id="docs">
          <div class="section-0">
            <div class="section-body">
              <h3>doc/html/boost_asio/example/cpp11/http/server/reply.cpp</h3>
<pre>
//
// reply.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at <a href="http://www.boost.org/LICENSE_1_0.txt">http://www.boost.org/LICENSE_1_0.txt</a>)
//

#include &quot;reply.hpp&quot;
#include &lt;string&gt;

namespace http {
namespace server {

namespace status_strings {

const std::string ok =
  &quot;HTTP/1.0 200 OK\r\n&quot;;
const std::string created =
  &quot;HTTP/1.0 201 Created\r\n&quot;;
const std::string accepted =
  &quot;HTTP/1.0 202 Accepted\r\n&quot;;
const std::string no_content =
  &quot;HTTP/1.0 204 No Content\r\n&quot;;
const std::string multiple_choices =
  &quot;HTTP/1.0 300 Multiple Choices\r\n&quot;;
const std::string moved_permanently =
  &quot;HTTP/1.0 301 Moved Permanently\r\n&quot;;
const std::string moved_temporarily =
  &quot;HTTP/1.0 302 Moved Temporarily\r\n&quot;;
const std::string not_modified =
  &quot;HTTP/1.0 304 Not Modified\r\n&quot;;
const std::string bad_request =
  &quot;HTTP/1.0 400 Bad Request\r\n&quot;;
const std::string unauthorized =
  &quot;HTTP/1.0 401 Unauthorized\r\n&quot;;
const std::string forbidden =
  &quot;HTTP/1.0 403 Forbidden\r\n&quot;;
const std::string not_found =
  &quot;HTTP/1.0 404 Not Found\r\n&quot;;
const std::string internal_server_error =
  &quot;HTTP/1.0 500 Internal Server Error\r\n&quot;;
const std::string not_implemented =
  &quot;HTTP/1.0 501 Not Implemented\r\n&quot;;
const std::string bad_gateway =
  &quot;HTTP/1.0 502 Bad Gateway\r\n&quot;;
const std::string service_unavailable =
  &quot;HTTP/1.0 503 Service Unavailable\r\n&quot;;

boost::asio::const_buffer to_buffer(reply::status_type status)
{
  switch (status)
  {
  case reply::ok:
    return boost::asio::buffer(ok);
  case reply::created:
    return boost::asio::buffer(created);
  case reply::accepted:
    return boost::asio::buffer(accepted);
  case reply::no_content:
    return boost::asio::buffer(no_content);
  case reply::multiple_choices:
    return boost::asio::buffer(multiple_choices);
  case reply::moved_permanently:
    return boost::asio::buffer(moved_permanently);
  case reply::moved_temporarily:
    return boost::asio::buffer(moved_temporarily);
  case reply::not_modified:
    return boost::asio::buffer(not_modified);
  case reply::bad_request:
    return boost::asio::buffer(bad_request);
  case reply::unauthorized:
    return boost::asio::buffer(unauthorized);
  case reply::forbidden:
    return boost::asio::buffer(forbidden);
  case reply::not_found:
    return boost::asio::buffer(not_found);
  case reply::internal_server_error:
    return boost::asio::buffer(internal_server_error);
  case reply::not_implemented:
    return boost::asio::buffer(not_implemented);
  case reply::bad_gateway:
    return boost::asio::buffer(bad_gateway);
  case reply::service_unavailable:
    return boost::asio::buffer(service_unavailable);
  default:
    return boost::asio::buffer(internal_server_error);
  }
}

} // namespace status_strings

namespace misc_strings {

const char name_value_separator[] = { ':', ' ' };
const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

std::vector&lt;boost::asio::const_buffer&gt; reply::to_buffers()
{
  std::vector&lt;boost::asio::const_buffer&gt; buffers;
  buffers.push_back(status_strings::to_buffer(status));
  for (std::size_t i = 0; i &lt; headers.size(); ++i)
  {
    header&amp; h = headers[i];
    buffers.push_back(boost::asio::buffer(h.name));
    buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
    buffers.push_back(boost::asio::buffer(h.value));
    buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  }
  buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}

namespace stock_replies {

const char ok[] = &quot;&quot;;
const char created[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Created&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;201 Created&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char accepted[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Accepted&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;202 Accepted&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char no_content[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;No Content&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;204 Content&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char multiple_choices[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Multiple Choices&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;300 Multiple Choices&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char moved_permanently[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Moved Permanently&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;301 Moved Permanently&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char moved_temporarily[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Moved Temporarily&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;302 Moved Temporarily&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char not_modified[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Not Modified&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;304 Not Modified&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char bad_request[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Bad Request&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;400 Bad Request&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char unauthorized[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Unauthorized&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;401 Unauthorized&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char forbidden[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Forbidden&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;403 Forbidden&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char not_found[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Not Found&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;404 Not Found&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char internal_server_error[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Internal Server Error&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;500 Internal Server Error&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char not_implemented[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Not Implemented&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;501 Not Implemented&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char bad_gateway[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Bad Gateway&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;502 Bad Gateway&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;
const char service_unavailable[] =
  &quot;&lt;html&gt;&quot;
  &quot;&lt;head&gt;&lt;title&gt;Service Unavailable&lt;/title&gt;&lt;/head&gt;&quot;
  &quot;&lt;body&gt;&lt;h1&gt;503 Service Unavailable&lt;/h1&gt;&lt;/body&gt;&quot;
  &quot;&lt;/html&gt;&quot;;

std::string to_string(reply::status_type status)
{
  switch (status)
  {
  case reply::ok:
    return ok;
  case reply::created:
    return created;
  case reply::accepted:
    return accepted;
  case reply::no_content:
    return no_content;
  case reply::multiple_choices:
    return multiple_choices;
  case reply::moved_permanently:
    return moved_permanently;
  case reply::moved_temporarily:
    return moved_temporarily;
  case reply::not_modified:
    return not_modified;
  case reply::bad_request:
    return bad_request;
  case reply::unauthorized:
    return unauthorized;
  case reply::forbidden:
    return forbidden;
  case reply::not_found:
    return not_found;
  case reply::internal_server_error:
    return internal_server_error;
  case reply::not_implemented:
    return not_implemented;
  case reply::bad_gateway:
    return bad_gateway;
  case reply::service_unavailable:
    return service_unavailable;
  default:
    return internal_server_error;
  }
}

} // namespace stock_replies

reply reply::stock_reply(reply::status_type status)
{
  reply rep;
  rep.status = status;
  rep.content = stock_replies::to_string(status);
  rep.headers.resize(2);
  rep.headers[0].name = &quot;Content-Length&quot;;
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = &quot;Content-Type&quot;;
  rep.headers[1].value = &quot;text/html&quot;;
  return rep;
}

} // namespace server
} // namespace http
</pre>
            </div>
          </div>
        </div>
      </div>

      <div class="clear"></div>
    </div>
  </div>

  <div id="footer">
    <div id="footer-left">
      <div id="revised">
        <p>Revised $Date$</p>
      </div>

      <div id="copyright">
        <p>Copyright Beman Dawes, David Abrahams, 1998-2005.</p>

        <p>Copyright Rene Rivera 2004-2008.</p>
      </div>  <div id="license">
    <p>Distributed under the <a href="/LICENSE_1_0.txt" class=
    "internal">Boost Software License, Version 1.0</a>.</p>
  </div>
    </div>

    <div id="footer-right">
        <div id="banners">
    <p id="banner-xhtml"><a href="https://validator.w3.org/check?uri=referer"
    class="external">XHTML 1.0</a></p>

    <p id="banner-css"><a href=
    "https://jigsaw.w3.org/css-validator/check/referer" class=
    "external">CSS</a></p>

    <p id="banner-osi"><a href=
    "https://opensource.org/docs/definition.php" class="external">OSI
    Certified</a></p>
  </div>
    </div>

    <div class="clear"></div>
  </div>
</body>
</html>
