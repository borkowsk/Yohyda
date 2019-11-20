//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Modified for FASADA (c) 2019 by Wojciech Borkowski
// (wborkowsk MAŁPA gmail KROPA com , wborkowski MAŁPA uw KROPA edu KROPA pl)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

namespace http {
namespace server {

extern "C" //Fasada interface functions
{
    //void set_host_and_port(const char* iHost,const char* iPort);
    bool communicate_with_fasada(const request& curr_request, reply& curr_reply);
}

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

void request_handler::handle_request(const request& req, reply& rep)
{
  std::cout<<"Request: "<<req.uri<<std::endl;
  std::cout<<"HTTP: "<<req.http_version_major<<"."<<req.http_version_minor<<std::endl;
  std::cout<<"methed: "<<req.method<<std::endl;
  for(auto h:req.headers)
      std::cout<<h.name<<" : "<<h.value<<std::endl;

  //COMMUNICATION WITH fasada IS ONLY IN THIS FUNCTION:
  //    bool communicate_with_fasada(const request& req, reply& rep);
  //CALLED BELOW:
  if (req.uri.find_first_of("@?!&")!=std::string::npos)
  {
      std::cout<<"Request: "<<req.uri<<" is for FACJATA "<<std::endl;
      if(!communicate_with_fasada(req,rep)) //when totally failed
      {                                     //return stock_reply
          std::cout<<"Request: "<<req.uri<<" not handled "<<std::endl;
          rep = reply::stock_reply(reply::not_found);
      }
      return;
  }

  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    std::cout<<"Request: "<<req.uri<<" bad_request "<<std::endl;
    return;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = reply::stock_reply(reply::bad_request);
    std::cout<<"Request: "<<req.uri<<" bad_path "<<std::endl;
    return;
  }

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/')      // If path ends in slash (i.e. is a directory) then add "index.html".
  {
    request_path += "index.html";
  }

  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if(last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::string full_path = doc_root_ + request_path;
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
      rep = reply::stock_reply(reply::not_found);
      std::cout<<"Request: "<<req.uri<<" not_found "<<std::endl;
      return;
  }
  else
  {
      // Fill out the reply to be sent to the client.
      rep.status = reply::ok;
      char buf[512];
      while (is.read(buf, sizeof(buf)).gcount() > 0)
          rep.content.append(buf, is.gcount());
  }

  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = mime_types::extension_to_type(extension);

  std::cout<<"\nRequest: "<<req.uri<<" done."<<std::endl;
  std::cout<<rep.headers[0].name<<" : "<<rep.headers[0].value<<std::endl;
  std::cout<<rep.headers[1].name<<" : "<<rep.headers[1].value<<std::endl;
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace http

/*
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
*/
