/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///
/// @file "URLparser.hpp" - Fasada compatible URL parser
///
#ifndef FACJATA_URL_PARSER_H
#define FACJATA_URL_PARSER_H (1)
/*
 /// This class split URL into following parts available by [] operator:
 ///      "&protocol" "&domain" "&port" "&path" ["&extension"] "&query"
 ///
 /// Additionally, when parse_query is true all parameters in query are available
 /// on their own names. First parameter (after ? ) without value is available under name
 /// "&processor" - it`s specific for the "fasada" technology.
 /// Alternatively last part between ! and ? is also "&processor" - when WRITE processing
 /// is requested.
 *
 * Example:
 *
 * https://localhost:8000/public/index.json?proc&long&html=true&color=true
 *
 * https://localhost:8000/public/index.json!proc?long&html=true&color=true
 *
 * will be parsed as follow:
 *
 * &domain = localhost
 * &extension = json
 * &path = /https://localhost:8000/public/index.html
 * &port = 8000
 * &processor = proc
 * &protocol = http
 * &query = proc&long&html=true&color=true | proc?long&html=true&color=true
 * color = true
 * html = true
 * long = true
 *
 * but the second form allow to implement write - modification of the database
 */

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_types.h"
#include <string>
#include <map>

namespace fasada
{

val_string urlDecode(const val_string& str);
val_string urlEncode(const val_string& str);

class URLparser:public std::map<key_string,val_string>
{
protected:
    bool parse_query; //Should split "query" into "variables"?
    val_string bakURL;//Original URL content

    //For lazy checking important parts
    int HTML_output;  //Output with HTML?
    int MINI_output;  //Minimal output
    int LONG_output;  //Maximal output
    val_string FULL_path;
    val_string PARENT_path;


public://Construction, destruction etc.
    URLparser(bool pq=false):parse_query(pq),HTML_output(-1),MINI_output(-1),LONG_output(-1){}
    URLparser(const char* URL,bool pq=true):parse_query(pq),HTML_output(-1),MINI_output(-1),LONG_output(-1){ doParsing(URL); }
   ~URLparser(){}

static
    const char* URLpattern;//Regex pattern for URL used in this class

    const val_string& getOriURL(){ return bakURL;} //Oryginalne "brzmienie" URLa - LANG?
    val_string  getFullPath();   //Zwraca i jednoczesnie dopisuje "fullpath" do słownika
    val_string  getParentPath(); //Zwraca i jednoczesnie dopisuje "parentpath" do słownika
    bool        asHTML();//Wymagany wynik w HTMLu
    bool        asMINI();//Minimalny jednoznaczny format danych
    bool        asLONG();//Format rozbudowany w HTMLu lub NIE!

protected://Implementation
    //Parse URL into elements
    void doParsing(const val_string& URL);//May throw on errors!
};

/// Make string from pattern substituting variables of form ${var_namke} from map.
/// Of course ${} are removed. NESTED calls not alloved!!!
/// Usable for made URL, HTML FORMS and many others texts.
val_string& substitute_variables(const val_string& pattern,const std::map<key_string,val_string>& variables);

}//namespace "fasada"

#endif //FACJATA_URL_PARSER_H
