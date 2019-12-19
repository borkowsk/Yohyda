/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///
/// See also: https://en.cppreference.com/w/cpp/preprocessor/replace
///
#define UNIT_IDENTIFIER "Format_toolbox"
#include "fasada.hpp"
#include "format_toolbox.h"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/regex.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>


namespace fasada
{

format_toolbox::format_toolbox(){}

/// Informative functions
/// //////////////////////////////////////////////////////////////////////

/// How many 'c' characters in a string?
unsigned int format_toolbox::countCharacters(std::string str,char c)
//https://stackoverflow.com/questions/3867890/count-character-occurrences-in-a-string-in-c
{
int count = 0, size=str.size();

for (int i = 0; i < size; i++)
  if (str[i] == c)
        count++;

return count;
}

/// Is protocol in allowed set of protocols?
std::string allowedFileProtocols= "http: https: ftp: ftps: ";

bool format_toolbox::isLink(const std::string& str)
{
    auto pos=str.find(":");

    //std::cerr<<"'"<<str<<"' "<<pos<<std::endl;

    if(pos==str.npos || pos>7 || pos<2 )
        return false;

    auto prot=str.substr(0,pos+1)+" ";

    //std::cerr<<"'"<<str<<"' "<<pos<<"   "<<prot<<std::endl;

    if(allowedFileProtocols.find(prot,0)!=allowedFileProtocols.npos) // Extension in string of allowed.
    {
        //std::cerr<<"'"<<prot<<"' -> '"<<str<<"' "<<std::endl;
        return true;
    }
/*
    return str.find("http:",0)==0 || str.find("https:",0)==0
            || str.find("ftp:",0)==0 || str.find("ftps:",0)==0
            ;//More? mail? what else? TODO
*/
    return false;
}

/*
std::vector<std::string> allowedFileTypes{".html",".htm",".css",".js"
                                         ,".json",".xml",".csv",".txt",".log"
                                         ,".jpeg",".jpg",".png",".gif"
                                         ,".mpeg",".mp4",".mp3",".mov"
                                         ,".md",".sh",".ls",".cpp",".hpp",".h"
                                         };
*/

/// Is en extension from a set of allowed file types?
std::string allowedFileTypes=   ".html .htm .css .js"
                                ".json .xml .csv .txt .log"
                                ".jpeg .jpg .png .gif"
                                ".mpeg .mp4 .mp3 .mov"
                                ".md .sh .ls .cpp .hpp .h";

inline
bool isExtAllowed(const std::string& str)
{
    auto pos=str.rfind(".");

    if(pos!=str.npos)
    {
        auto ext=str.substr(pos)+" ";
        if(ext.size()>7)//Extensions are not longer than 5 characters
            return false;

//        std::cerr<<ext<<"; ";
        if(allowedFileTypes.find(ext,0)!=allowedFileTypes.npos) // Extension in string of allowed.
            return true;
/*
        if (std::find(allowedFileTypes.begin(), allowedFileTypes.end(), ext) != allowedFileTypes.end())
        {
             return true;// Element in vector.
        }
*/
    }
    else return false;
}

/// Is a path to a local allowed type file?
bool format_toolbox::isLocalFile(const std::string& str)
{
    return  str.length() >4
            && !isLink(str)
            && isExtAllowed(str);
            /*
            && str.find("http:",0)==str.npos && str.find("https:",0)==str.npos
            && str.find("ftp:",0)==str.npos && str.find("ftps:",0)==str.npos
            &&
            (  str.rfind(".html",len-5)==len-5 || str.rfind(".htm",len-4)==len-4
            || str.rfind(".css", len-4)==len-4 || str.rfind(".js", len-3)==len-3
            || str.rfind(".json",len-5)==len-5 || str.rfind(".md", len-3)==len-3
            || str.rfind(".txt", len-4)==len-4 || str.rfind(".sh", len-3)==len-3
            || str.rfind(".log", len-4)==len-4 || str.rfind(".ls", len-3)==len-3
            || str.rfind(".jpeg",len-5)==len-5 || str.rfind(".jpg",len-4)==len-4
            || str.rfind(".gif" ,len-4)==len-4 || str.rfind(".png",len-4)==len-4
            || str.rfind(".mpeg",len-5)==len-5 || str.rfind(".mp4",len-4)==len-4
            || str.rfind(".mov" ,len-4)==len-4
            || str.rfind(".cpp", len-4)==len-4 || str.rfind(".h", len-2)==len-2 ||  str.rfind(".hpp", len-4)==len-4
            )
            ;// TODO - dictionary of less popular file extensions
            */
}


/// HTML construction functions
/// ////////////////////////////////////////////////////////////////////////

//Replacing ${variable_name} with variables from request
//This is virtual!
std::string format_toolbox::replace_all_variables(std::string template_version,URLparser& request)
{
    request.getFullPath();//generuje "fullpath" w request'cie jak by jeszcze nie bylo
    //MODYFIKACJE ZMIENNYMI Z REQUEST I ODDANIE
    return substitute_variables(template_version,request);//Już zmodyfikowana
}

//SOME SYMBOLS
// &#x1F3F0; - castle eur.
// &#x1F3E0; - house building
// &#x2302;  - house (simple)
// &#x1F3E2; - office building (skyscraper)
//PAGE HEADING
std::string format_toolbox::HTMLHeader=
        "<HTML>\n<HEAD>\n"
        "<TITLE>$page_title</TITLE>\n"
        "<meta charset=\"utf-8\">\n"
        "<link rel=\"stylesheet\" type=\"text/css\" href=\"/_skin/fasada.css\">\n"
        "</HEAD>\n<BODY>\n";

std::string  format_toolbox::getHtmlHeaderDefaults(const std::string& Title)
//Default set of html <HEAD> lines finishing by <BODY>
{
    std::string ReadyHeader=HTMLHeader;
    boost::replace_all(ReadyHeader,"$page_title",Title);
    return ReadyHeader;
}

//PAGE FOOTING ;-)
std::string format_toolbox::HTMLFooter=
        "\n</BODY></HTML>\n";

std::string  format_toolbox::getHtmlClosure(const std::string& _unit_comp)
//Compatible set of tags for end of html document - TODO change using replace_all_variables()
{
    std::string Footer="\n<HR class=\"footer_hr\"><P class=\"footer_p\">"
                       "<a href=\"https://sites.google.com/view/fasada-cpp/\" target=\"fasada_info_page\" ><b>Fasada</b></a> version ";
    Footer+=_version_str+std::string(" &#x2302;&Cap;&cuvee;&#x26C2;&cuvee;&Cap;&#x2302; ")+( _unit_comp.length()!=0? _unit_comp : _compiled )+"</P>";
    Footer+=HTMLFooter;// &ofcir; ?
    return Footer;
}

//To jest bardzo często stosowane w wielu miejscach
std::string format_toolbox::HTMLBack=
        "&nwarhk;";///"UP", "RETURN","WRÓĆ" or "<--|";

//Action links on pages (READ or WRITE)
std::string format_toolbox::HTMLAction=
        "<A HREF=\""
        "${action_href}"
        "\" class=\"fasada_action\""
        " title=\""
        "${title_href}"
        "\">"
        "${link_content}"
        "</A>"
        ;

std::string  format_toolbox::getActionLink(const std::string& Href,const std::string& Content,const std::string& Title)
{
    std::string ReadyLink=HTMLAction;
    boost::replace_all(ReadyLink,"${action_href}",Href);
    boost::replace_all(ReadyLink,"${title_href}",Title);
    boost::replace_all(ReadyLink,"${link_content}",Content);
    return ReadyLink;
}


//See only link (READ., newer WRITE)
std::string  format_toolbox::getSeeLink(const std::string& data,URLparser& request,const std::string& Content)
{
    std::string out="";
    if(request["&debug"]=="true")
    {
        out+="\n<pre>";
        out+="\ndata: "+data+"\nprivate_directory: "+request["&private_directory"]+"\npath: "+request["&path"];
        std::string link="file://"+request["&private_directory"]+request["&path"]+"/"+data;
        out+="\n<a href=\""+link+"\" > "+Content+" "+link+"</a>";

        link="http://"+request["&domain"]+":"+request["&port"];
        if(data.at(0)=='/')
            link+=data;
        else
            link+=request["&path"]+"/"+data;
        out+="\n<a href=\""+link+"\" > "+Content+" "+link+"</a>";
        out+="</pre>\n";
    }
    else
    {
        std::string link="http://"+request["&domain"]+":"+request["&port"];
        if(data.at(0)=='/')
            link+=data;
        else if(data.at(0)=='.' && data.at(1)=='/')
            link=request.getParentPath()+(data.c_str()+1)+"?self&html";
        else
            link+=request["&path"]+"?checkFile&html";//Awaryjnie

        out+="<a class=\"fasada_view\" href=\""+link+"\"";
        out+=">"+Content+"</a>";
    }
    return out;
}

std::string  format_toolbox::getNodePanel(const pt::ptree& node,const std::string& fullpath,URLparser& request)
{
    std::string o;
    const std::string& data=node.data();
    std::string proplong=(request.asLONG()?"&long":"");

    o+="&nbsp; "+getActionLink(fullpath+"?get&html&long","?","Get value");

    if(data=="")    //Czy to liść czy (potencjalny) węzeł?
    {
        if(node.size()>0)
        {
          o+="&nbsp; "+getActionLink(fullpath+"?ls&html"+proplong,"&hellip;","Print as list");
          o+="&nbsp; "+getActionLink(fullpath+"?dfs&html"+proplong,"&forall;","Print as tree");
        }

        o+="&nbsp; "+getActionLink(fullpath+"?add&html","+","Add!");

        auto attr=node.find("local_uri");
        if(attr!=node.not_found())
        {
            //Łatwy podgląd już sprawdzonego pliku. TODO
            o+="&nbsp; "+getActionLink(attr->second.data(),"&#x1f441;","See as file "
                                       +std::string(attr->first.data())+":"+attr->second.data());//HTML Eye https://www.fileformat.info/info/unicode/char/1f441/index.htm
        }
    }
    else
    {
        if(writing_enabled() && data.at(0)=='!')
        {
            bool haveQ=(data.find('?',0)!=data.npos);//Is parameter list already started?
            o+="&nbsp; "+getActionLink(fullpath+data+(haveQ?"&html":"?self&html"),"RUN!","Run link read/write");
        }
        else
        if(data.at(0)=='?')
        {
            o+="&nbsp; "+getActionLink(fullpath+data+"&html","run","Run link as read only");
        }
        else
        if(isLink(data))
        {
            o+="&nbsp; "+getActionLink(data,"&#x1F30D;","Follow link");//&#x1F30D; :globe:
        }
        else
        if(isLocalFile(data))
        {
            if(data.at(0)=='/' || (data.at(0)=='.' && data.at(1)=='/' ))
                o+="&nbsp; "+getSeeLink(data,request,"&#x1f441;");//Plik sprawdzony

            else  if(writing_enabled())
                o+="&nbsp; "+getActionLink(fullpath+"!checkFile?self&html","check!");//Plik do sprawdzenia
        }

        auto codeContent=urlEncode(data);//TODO - raczej enkodowane powinny być całe linki CHECK IT!
        auto serverTop=request["&protocol"]+"://"+request["&domain"]+':'+request["&port"];//FIND!
        o+="&nbsp; "+getActionLink(serverTop+"?find&html&sugestion="+codeContent,"&#x1F50D;","Find similar i values or node names");
    }

    if(writing_enabled())
        o+="&nbsp; "+getActionLink(fullpath+"?set&html","=","Set value");

    std::string parentpath=fullpath.substr(0,fullpath.rfind("/"));
    o+="&nbsp;"+getActionLink(parentpath+"?ls&html"+proplong,"&isin;","Is node of "+parentpath);

    return o;
}

std::string format_toolbox::preprocessIntoHtml(const std::string& str)
//Preprocess links and other markers into HTML tags.
//http://www.cplusplus.com/reference/regex/regex_replace/
//https://en.wikipedia.org/wiki/Emoticon, https://www.w3schools.com/charsets/ref_emoji_smileys.asp, http://defindit.com/ascii.html
//https://dev.w3.org/html5/html-author/charref
//https://stackoverflow.com/questions/1342103/what-unicode-character-do-you-use-in-your-website-instead-of-image-icons
//https://stackoverflow.com/questions/12036038/is-there-unicode-glyph-symbol-to-represent-search
//https://tutorialzine.com/2014/12/you-dont-need-icons-here-are-100-unicode-symbols-that-you-can-use
//
//TODO - REIMPLEMENTATION LIKE THERE:
//https://www.boost.org/doc/libs/1_53_0/libs/regex/doc/html/boost_regex/ref/regex_replace.html
{
    std::string tmp=str;

    //\r \n , ) ] i . sprawiają problemy przy połaczeniu z linkami - U+200A z http://jkorpela.fi/chars/spaces.html
    boost::replace_all(tmp,", ","\u200A, " /*"\x7f"*/);
    boost::replace_all(tmp,")","\u200A)" /*"\x7f"*/);
    boost::replace_all(tmp,"]","\u200A]" /*"\x7f"*/);
    boost::replace_all(tmp,">","\u200A>" /*"\x7f"*/);
    boost::replace_all(tmp,". ","\u200A. " /*"\x7f"*/);
    boost::replace_all(tmp,"|","\u200A| " /*"\x7f"*/);
    boost::replace_all(tmp,"\r","\x7f\r" /*"\x7f"*/);
    boost::replace_all(tmp,"\n","\u200A\n" /*"\x7f"*/);

    //std::regex link(URLparser::URLpattern);
    boost::regex link(URLparser::URLpattern);
    //auto out=std::regex_replace(tmp,link,"<A HREF=\"$&\">$&</A>");
    auto out=boost::regex_replace(tmp,link,"<A HREF=\"$&\">$&</A>");

    boost::regex hashtag("[ \\t\\n](#)([A-Za-z0-9]*)");//Może się pomylić z # pod koniec URL'a
    // https://www.regextester.com/96112 - improved
    // "(?:\s|^)#[A-Za-z0-9\-\.\_]+(?:\s|$)" //Alternative REGEX is here: https://www.regextester.com/96916
    out=boost::regex_replace(out,hashtag,"<A HREF=\"/?find&html&value=$2\">$& </A>");

    boost::regex hashtag2("(?:\\s|^)(#)([A-Za-z0-9]*)");//Tylko od początku linii? a może całości?
    out=boost::regex_replace(out,hashtag2,"<A HREF=\"/?find&html&value=$2  \">$& </A>");

    boost::regex attag1("[ \\t\\n](@)([0-9a-zA-Z_]+)");//Może się mylic z email'em
    out=boost::regex_replace(out,attag1,"<A HREF=\"/?find&html&sugestion=$2\" >$& </A>");

    boost::regex attag2("@\\[([0-9]+):([0-9]+):([0-9a-zA-Z _][^\\]]+)\\]");//To się z mailem nie pomyli
    //Na tym przykładzie działa świetnie: @[137501236414754:274:Memetyka] alama cota @[108325199188305:274:Pleistocene] a olapsa @[113919085285005:274:Jacek Dukaj] o filmach SF
    //https://www.regextester.com/
    out=boost::regex_replace(out,attag2,"<A HREF=\"/?find&html&sugestion=$&\" >$& </A>");

    boost::regex email("[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*");//https://www.regextester.com/19
    out=boost::regex_replace(out,email,"<A HREF=\"/?find&html&value=$&\" >$& </A>");

    boost::replace_all(out,"\u200A\n","\n");//Przywracamy
    boost::replace_all(out,"\u200A,", ",");//Przywracamy
    boost::replace_all(out,"\u200A)", ")");//Przywracamy
    boost::replace_all(out,"\u200A]", "]");//Przywracamy
    boost::replace_all(out,"\u200A>", ">");//Przywracamy
    boost::replace_all(out,"\u200A. ",". ");//Przywracamy

    boost::replace_all(out,"-->","&rarr;");
    boost::replace_all(out,"==>","&rArr;");
    boost::replace_all(out,":eye:","&#x1f441;");//OKO
    boost::replace_all(out,":link:","&#x1f517;");//LINK
    boost::replace_all(out,":earth:","&#x1F30D;");//Earth globe https://www.compart.com/en/unicode/U+1F30D
    boost::replace_all(out,":loupe:","&#x1F50D;");//monocular or magnifier
    boost::replace_all(out,":-)","&#x1F60A;");//uśmiech
    boost::replace_all(out,":)", "&#x1F642;");//inny uśmiech
    boost::replace_all(out,":-(","&#x1F61E;");//smutek
    boost::replace_all(out,":( ", "&#x1F61F; ");//obawa
    boost::replace_all(out,":-D","&#x1F600;");//szeroki uśmiech
    boost::replace_all(out,":D ", "&#x1F603; ");//inny szeroki uśmiech
    boost::replace_all(out,":P ", "&#x1F60B; ");//ozorek
    boost::replace_all(out,":-P","&#x1F61B;");//ozorek centralny
    boost::replace_all(out," :/ ", " &#x1F615; ");//krzywa geba. skonfundowany - bez spacji niszczy linki!
    boost::replace_all(out,":-/","&#x1F615;");//krzywa geba
    boost::replace_all(out,";) ", "&#x1F609; ");//wink
    boost::replace_all(out,";-)","&#x1F609;");//wink
    boost::replace_all(out,":^)","&#x1F921;");//uśmiechnięty klown
    boost::replace_all(out,"\n","\n<BR>");//break LINEs
    return out;
}

}//namespace "fasada"
