#fasada
Project FASADA version 0.01 (single thread WWW server README.txt)
=================================================================

Do współpracy z przeglądarkami WWW, które stanowią podstawowy interfejs aplikacji projektu FASADA konieczny jest jakiś server WWW. Najlepiej najprostrzy i nie jak najbardziej bezobsługowy, gdyż użytkownik nie ma obowiązku znać się na konfigurowaniu i obsłudze serwera WWW. Jednakże moduł  *"commwithfasada.cpp"* tego mini serwera może posłużyć programistom jako przykład do implementacji komunikacji z bardziej zaawansowanymi serwerami.

Jak niemal każda aplikacja projektu FASADA serwer WWW wymaga do kompilacji i działania biblioteki BOOST. Instalacja boost na UBUNTU 16.04: 

sudo apt-get install libboost-all-dev

HTTP Server 
-----------

Punktem startowym jest serwer HTML przykładowy dla biblioteki boost:

*This example illustrates the use of asio in a simple single-threaded server implementation of HTTP 1.0. It demonstrates how to perform a clean shutdown by cancelling all outstanding asynchronous operations*


    boost_asio/example/cpp11/http/server/connection.cpp
    boost_asio/example/cpp11/http/server/connection.hpp
    boost_asio/example/cpp11/http/server/connection_manager.cpp
    boost_asio/example/cpp11/http/server/connection_manager.hpp
    boost_asio/example/cpp11/http/server/header.hpp
    boost_asio/example/cpp11/http/server/main.cpp
    boost_asio/example/cpp11/http/server/mime_types.cpp
    boost_asio/example/cpp11/http/server/mime_types.hpp
    boost_asio/example/cpp11/http/server/reply.cpp
    boost_asio/example/cpp11/http/server/reply.hpp
    boost_asio/example/cpp11/http/server/request.hpp
    boost_asio/example/cpp11/http/server/request_handler.cpp
    boost_asio/example/cpp11/http/server/request_handler.hpp
    boost_asio/example/cpp11/http/server/request_parser.cpp
    boost_asio/example/cpp11/http/server/request_parser.hpp
    boost_asio/example/cpp11/http/server/server.cpp
    boost_asio/example/cpp11/http/server/server.hpp

https://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/examples/cpp11_examples.html
https://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/examples.html (nowsze. Tu jako WWW Server 1.)


