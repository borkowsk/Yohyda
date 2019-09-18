//
// connection_manager.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Adopted for FASADA (c) 2019 by Wojciech Borkowski
// (wborkowsk MAŁPA gmail KROPA com , wborkowski MAŁPA uw KROPA edu KROPA pl)
//

#include "connection_manager.hpp"

namespace http {
namespace server {

connection_manager::connection_manager()
{
}

void connection_manager::start(connection_ptr c)
{
  connections_.insert(c);
  c->start();
}

void connection_manager::stop(connection_ptr c)
{
  connections_.erase(c);
  c->stop();
}

void connection_manager::stop_all()
{
  for (auto c: connections_)
    c->stop();
  connections_.clear();
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
