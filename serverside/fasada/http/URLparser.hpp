/// File "URLparser.hpp" - "fasada" compatible URL parser
///
#ifndef FACJATA_URL_PARSER_H
#define FACJATA_URL_PARSER_H (1)

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_types.h"
#include <string>
#include <map>
/*
 * This class split URL into following parts available by [] operator:
 *      "&protocol" "&domain" "&port" "&path" ["&extension"] "&query"
 *
 * Additionally, when parse_query is true all parameters in query are available
 * on their own names. First parameter without value is available under name
 * "&processor" - it`s specific for the "fasada" technology.
 *
 * Example:
 * https://localhost:8000/public/index.json?ls&long&html=true&color=true
 * will be parsed as follow:
 *
 * &domain = localhost
 * &extension = json
 * &path = /https://localhost:8000/public/index.html
 * &port = 8000
 * &processor = ls
 * &protocol = http
 * &query = ls&color&info=full&lhc
 * color = true
 * html = true
 * long = true
 */
namespace fasada
{

class URLparser:public std::map<key_string,val_string>
{
protected://Configuration & redundancy
    bool parse_query;//is this name OK?
    val_string bakURL;

public://Construction, destruction etc.
    URLparser(bool pq=false):parse_query(pq){}
    URLparser(const char* URL,bool pq=true):parse_query(pq){ doParsing(URL); }
   ~URLparser(){}

static
    const char* URLpattern;//Regex pattern for URL used in this class

    const val_string& getOriURL(){ return bakURL;} //Oryginalne "brzmienie" URLa
    val_string  getFullPath();   //Zwraca i jednoczesnie dopisuje "fullpath" do słownika
    val_string  getParentPath(); //Zwraca i jednoczesnie dopisuje "parentpath" do słownika

protected://Implementation
    //Parse URL into elements
    void doParsing(const val_string& URL);//May throw on errors!
};

}//namespace "fasada"

#endif //FACJATA_URL_PARSER_H
