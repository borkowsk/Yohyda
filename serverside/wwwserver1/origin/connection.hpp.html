<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <meta name="viewport" content="width=device-width,initial-scale=1.0" />
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>doc/html/boost_asio/example/cpp11/http/server/connection.hpp - 1.62.0</title>  <link rel="icon" href="/favicon.ico" type="image/ico" />
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
  <div class="boost-common-header-notice"><a class="boost-common-header-inner" href="/doc/libs/release/doc/html/boost_asio/example/cpp11/http/server/connection.hpp">This is the documentation for an old version of Boost.
                Click here to view this page for the latest version.</a></div>

  <div id="body">
    <div id="body-inner">
      <div id="content">
        <div class="section" id="docs">
          <div class="section-0">
            <div class="section-body">
              <h3>doc/html/boost_asio/example/cpp11/http/server/connection.hpp</h3>
<pre>
//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at <a href="http://www.boost.org/LICENSE_1_0.txt">http://www.boost.org/LICENSE_1_0.txt</a>)
//

#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include &lt;array&gt;
#include &lt;memory&gt;
#include &lt;<a href="../../../../../../../boost/asio.hpp">boost/asio.hpp</a>&gt;
#include &quot;reply.hpp&quot;
#include &quot;request.hpp&quot;
#include &quot;request_handler.hpp&quot;
#include &quot;request_parser.hpp&quot;

namespace http {
namespace server {

class connection_manager;

/// Represents a single connection from a client.
class connection
  : public std::enable_shared_from_this&lt;connection&gt;
{
public:
  connection(const connection&amp;) = delete;
  connection&amp; operator=(const connection&amp;) = delete;

  /// Construct a connection with the given socket.
  explicit connection(boost::asio::ip::tcp::socket socket,
      connection_manager&amp; manager, request_handler&amp; handler);

  /// Start the first asynchronous operation for the connection.
  void start();

  /// Stop all asynchronous operations associated with the connection.
  void stop();

private:
  /// Perform an asynchronous read operation.
  void do_read();

  /// Perform an asynchronous write operation.
  void do_write();

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The manager for this connection.
  connection_manager&amp; connection_manager_;

  /// The handler used to process the incoming request.
  request_handler&amp; request_handler_;

  /// Buffer for incoming data.
  std::array&lt;char, 8192&gt; buffer_;

  /// The incoming request.
  request request_;

  /// The parser for the incoming request.
  request_parser request_parser_;

  /// The reply to be sent back to the client.
  reply reply_;
};

typedef std::shared_ptr&lt;connection&gt; connection_ptr;

} // namespace server
} // namespace http

#endif // HTTP_CONNECTION_HPP
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
