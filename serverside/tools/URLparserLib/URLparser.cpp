/// File "URLparser.cpp" - "fasada" compatible URL parser
///
///
#include "URLparser.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using std::string;
using std::vector;
using boost::split;
using boost::algorithm::is_any_of;
using std::cout;
using std::endl;
using std::stringstream;
//   http://www.jakis-serwer.pl:8080/katalog1/katalog2/plik?reader&parametr1=wartosc1&parametr2=wartosc2#fragment_dokumentu
//   http://www.jakis-serwer.pl:8080/katalog1/katalog2/plik!writer?parametr1=wartosc1&parametr2=wartosc2#fragment_dokumentu
//   \__/   \_________________/\___/\_____________________/ \___________________________________/ \________________/
//     |             |           |             |                              |                            |
//  schemat         host        port   ścieżka do pliku                   zapytanie                     fragment
//(protokół)   (nazwa serwera)
//
//WZÓR:
//http://www.zedwood.com/article/cpp-boost-url-regex ? bazuje na boost/regex.hpp
//
//ALTERNATYWY:
//https://stackoverflow.com/questions/2616011/easy-way-to-parse-a-url-in-c-cross-platform ?
//https://github.com/chmike/CxxUrl
//http://boost.2283326.n4.nabble.com/Plans-to-add-parameter-parsing-in-Beast-td4703335.html
//https://github.com/Microsoft/cpprestsdk/blob/b9dd8eab814892f110853be1098df1b3f77b2ff0/Release/src/uri/uri.cpp
//   z RESTSDK (https://github.com/microsoft/cpprestsdk)
//http://think-async.com/Urdl/doc/html/ ? Cała duża biblioteka
//http://cppcms.com/wikipp/en/page/ref_cppcms_url_parser ? też inna biblioteka: http://cppcms.com/wikipp/en/page/main
namespace fasada
{
    void URLparser::Parse(const val_string& URL)
    //May throw on errors!
    {
        using split_vector_type=vector< string >;
        boost::regex ex("(http|https|ftp|ftps)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
        boost::cmatch what;

        bakURL=URL;
        this->clear();

        if(regex_match(URL.c_str(), what, ex))
        {
            string protocol = string(what[1].first, what[1].second);
            string domain   = string(what[2].first, what[2].second);
            string port     = string(what[3].first, what[3].second);
            string path     = string(what[4].first, what[4].second);
            string query    = string(what[5].first, what[5].second);
            bool   proc_set=false;
            /*cout << protocol <<' '<< domain <<' '<< port <<' '<< path <<' '<< query << endl;*/
            //cout << "'"<< query << "'" << endl;
            (*this)["&protocol"]=protocol;
            (*this)["&domain"]=domain;
            (*this)["&port"]=port;
            (*this)["&path"]=path;
            (*this)["&query"]=query;

            if(parse_query)//Tak dokładne dzielenie może niekiedy nie być potrzebne
            {
                split_vector_type SplitVec;
                split( SplitVec, query , is_any_of("&") );// , token_compress_on );
                for(string& s:SplitVec)
                {
                    if(s=="") continue;//Zawsze chociaż jedno jest, nawet jak puste query

                    int pos=s.find_first_of(":=");
                    if(pos>0)
                    {
                        (*this)[s.substr(0,pos)]=s.substr(pos+1);
                        //cout<< s.substr(0,pos) <<" = "<<(*this)[s.substr(0,pos)]<<endl;
                    }
                    else
                    if(!proc_set)
                    {
                        (*this)["&processor"]=s;
                        proc_set=true;
                        //cout<< "&processor := "<<s<<endl;
                    }
                    else
                    {
                        (*this)[s]="true";
                        //cout<< s <<" = true"<<endl;
                    }
                }

                if( this->find("&processor") == this->end() )
                {
                    (*this)["&processor"]="default";
                    //cout<< "&processor := default"<<endl;
                }
            }

            int pos0=path.rfind('!');
            if(pos0!=path.npos)
            {
                (*this)["&processor"]=path.substr(pos0);
                path=((*this)["&path"]=path.substr(0,pos0));
            }

            int pos1=path.rfind('/');
            int pos2=path.rfind('.');
            if(pos1<pos2)
            {
                (*this)["&extension"]=path.substr(pos2+1);
            }

        }
    }
}
