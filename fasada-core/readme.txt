A. Serwer Facies/Facjaty jest lokalnie działającą aplikacją, która z jednej strony udostępnia zawartość sieci społecznościowej użytkownikowi, a z drugiej komunikuje się z innymi serwerami Facies i wymienia z nimi treści społecznosciowe w postaci ZASZYFROWANYCH plików JSON o określonych atrybutach i przechowuje ZASZYFROWANĄ zawartość sieci - pliki danego użytkownika i te, na których przechowywanie jawnie lub domyślnie zezwolił. 
B. Serwer może być napisany w C++, Javie lub Pythonie, oraz innych językach które społeczność Facies/Facjat uzna za pożądane. 
C. Niezaleznie od użytego języka wszystkie wersje muszą być funkcjonalnie równoważne i być w stanie działać na tym samym zestawie plików roboczych
D. ... z dokładnością do obszaru przechowywania danych, który w przypadku większych maszyn może być i lokalny i sieciowy (od serwisu ftp po chmurowy - pośrednikiem są małe aplikacje przenoszące dane), natomiast komórki i tablety są raczej skazane na zewnętrzne repozytoria, które mogą współdzielić z serwerami stacjonarnymi tych samych użytkowników.   

TESTOWANE NA UBUNTU 16.04 z biblioteką boost 1.65 

Punktem startowym jest serwer HTML przykładowy dla biblioteki boost:

HTTP Server

"This example illustrates the use of asio in a simple single-threaded server implementation of HTTP 1.0. It demonstrates how to perform a clean shutdown by cancelling all outstanding asynchronous operations."


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
