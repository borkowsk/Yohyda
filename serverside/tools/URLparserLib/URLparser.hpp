/// File "URLparser.hpp" - Facies/Facjata compatible URL parser
///
#ifndef FACJATA_URL_PARSER
#define FACJATA_URL_PARSER (1)

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#include <string>
#include <string_view>
#include <map>
/*
 * This class split URL into following parts available by [] operator:
 *      "&protocol" "&domain" "&port" "&path" ["&extension"] "&query"
 *
 * Additionally, when parse_query is true all parameters in query are available
 * on their own names. First parameter without value is available under name
 * "&processor" - it`s specific for the Facjata technology.
 *
 * Example:
 * https://localhost:8000/public/index.html?ls&color&info=full&lhc
 * will be parsed as follow:
 *
 * &domain = localhost
 * &extension = html
 * &path = /https://localhost:8000/public/index.html
 * &port = 8000
 * &processor = ls
 * &protocol = http
 * &query = ls&color&info=full&lhc
 * color = true
 * info = full
 * lhc = true
 */
namespace facjata
{
using key_string=std::string;//string_view? https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map
using val_string=std::string;

class URLparser:public std::map<key_string,val_string>
{
    bool parse_query;
public:
    URLparser(bool pq=false):parse_query(pq){}
    URLparser(const char* URL,bool pq=true):parse_query(pq){Parse(URL);}
   ~URLparser(){}
    void Parse(const val_string& URL);//May throw on errors!
//OUTPUT:
    val_string bakURL;
};

}//namespace facjata

#endif //FACJATA_URL_PARSER
