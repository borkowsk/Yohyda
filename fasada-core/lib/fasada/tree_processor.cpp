/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

//Base class for all json data processors working inside
//local server of "fasada" resources stored
//

#include "fasada.hpp"
#include "tree_processor.h"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/regex.hpp>
#include <iostream>
#include <string>

namespace fasada
{

std::string tree_processor::HTMLHeader=
        "<HTML>\n<HEAD>\n"
        "<TITLE>$page_title</TITLE>\n"
        "<meta charset=\"utf-8\">\n"
        "<link rel=\"stylesheet\" type=\"text/css\" href=\"/_skin/fasada.css\">\n"
        "</HEAD>\n<BODY>\n";

std::string tree_processor::HTMLAction=
        "<A HREF=\""
        "$action_href"
        "\" class=\"fasada_action\""
        " title=\""
        "$title_href"
        "\">"
        "$link_content"
        "</A>"
        ;

std::string tree_processor::HTMLBack=
        "UP";/// "RETURN","WRÓĆ" or "<--|";

std::string tree_processor::HTMLFooter=
        "\n</BODY></HTML>\n";

// W C++ dopiero w main jest pewność że wewnętrzne struktury static
// z innych źródeł i z bibliotek zostały zainicjalizowane. To słabe, ale tak jest w C++
// Robimy sztuczkę...
// https://www.bfilipek.com/2018/02/static-vars-static-lib.html
// Czyli "lazy singleton pattern"
// https://stackoverflow.com/questions/6234791/how-to-wait-until-dynamic-initialization-of-static-variables-finishes

static
processors_map& map_of_readers()
{
   static processors_map _map_of_readers;
   return _map_of_readers;
}

static
processors_map& map_of_writers()
{
   static processors_map _map_of_writers;
   return _map_of_writers;
}

tree_processor::tree_processor(Category cat,const char* name):
    procCategory(cat),procName(name)
{
    if(cat>CONTROL)
      throw(tree_processor_exception("UNKNOWN CATEGORY OF PTREE PROCESSOR "+procName));

    try
    {
        //WRITER could be also a READER - for preparing a FORM//#include <regex>
        if((procCategory & WRITER) !=0 )
        {
            std::cout<<"Tree processor '"<<procName<<"' is registering as WRITER... ";
            map_of_writers()[procName]=this;
        }
        if((procCategory & READER) !=0 )
        {
            std::cout<<"Tree processor '"<<procName<<"' is registering as READER... ";
            map_of_readers()[procName]=this;
        }
    }
    catch(...)
    {
        std::cout<<std::endl;
        std::cerr<<"EXCEPTION occured when registering '"<<procName<<"' processor."<<std::endl;
        std::cerr <<"tree_processor:"<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
    }

    std::cerr<<"  DONE"<<std::endl;
}

tree_processor::~tree_processor()
{
    if(procCategory & WRITER !=0 )
        map_of_writers().erase(procName);
    if(procCategory & READER !=0 )
        map_of_readers().erase(procName);
}

tree_processor& tree_processor::getReadProcessor (const char* name)//may throw
{
    std::cout<<name<<std::endl;
    tree_processor* tmp=map_of_readers()[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR '")+name+"' NOT FOUND!"));
    return *tmp;
}

tree_processor& tree_processor::getWriteProcessor(const char* name)//may throw
{
    tree_processor* tmp=map_of_writers()[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR '")+name+"' NOT FOUND!"));
    return *tmp;
}

//Do some work, call _implement_read, clean & return
void tree_processor::read_tree(ShmString& o,const pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory & READER == 0 )
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' IS NOT A READER"));
    try{
        _implement_read(o,top,request);
    }
    catch(...)
    {
        std::cerr <<procName<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        o+="\n EXCEPTION OCCURED:"+boost::current_exception_diagnostic_information();
    }
    o+=MEM_END;//"DONE" MARKER FOR PARAREL PROCESS
}

//Do some work, call _implement_write, clean & return
void tree_processor::write_tree(ShmString& o,pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory & WRITER == 0 )
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' IS NOT A WRITER"));

    try{
        _implement_write(o,top,request);
    }
    catch(...)
    {
        std::cerr <<procName<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        o+="\n EXCEPTION OCCURED:"+boost::current_exception_diagnostic_information();
    }
    o+=MEM_END;//"DONE" MARKER FOR PARAREL PROCESS
}

std::string  tree_processor::getHtmlHeaderDefaults(const std::string& Title)
//Default set of html <HEAD> lines finishing by <BODY>
{
    std::string ReadyHeader=HTMLHeader;
    boost::replace_all(ReadyHeader,"$page_title",Title);
    return ReadyHeader;
}

std::string  tree_processor::getActionLink(const std::string& Href,const std::string& Content,const std::string& Title)
{
    std::string ReadyLink=HTMLAction;
    boost::replace_all(ReadyLink,"$action_href",Href);
    boost::replace_all(ReadyLink,"$title_href",Title);
    boost::replace_all(ReadyLink,"$link_content",Content);
    return ReadyLink;
}

std::string  tree_processor::getSeeLink(const std::string& data,URLparser& request,const std::string& Content)
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
            link=request.getParentPath()+(data.c_str()+1);
        else
            link+=request["&path"]+"?checkFile";//Awaryjnie

        out+="<a class=\"fasada_view\" href=\""+link+"\"";
        out+=">"+Content+"</a>";
    }
    return out;
}

std::string  tree_processor::getHtmlClosure(const char* _unit_comp)
//Compatible set of tags for end of html document
{
    std::string Footer="<HR class=\"footer_hr\"><P class=\"footer_p\">Fasada version ";
    Footer+=_version_str+std::string(" Unit compiled: ")+(*_unit_comp!='\0'?_unit_comp:_compiled)+"</P>";
    Footer+=HTMLFooter;
    return Footer;
}

std::string  tree_processor::getNodePanel(const std::string& data,const std::string& fullpath,URLparser& request)
{
    std::string o;

    o+="&nbsp; "+getActionLink(fullpath+"?get&html&long","?","Get value");

    if(data=="")    //Czy to liść czy (potencjalny) węzeł?
    {
        o+="&nbsp; "+getActionLink(fullpath+"?dfs&html","&forall;","Print as tree");
        o+="&nbsp; "+getActionLink(fullpath+"?add&html","+","Add!");

       /* auto attr=t.find("local_uri");
        if(attr!=t.not_found())
        {
            auto data=t.data();//na góre
            //Łatwy podgląd już sprawdzonego pliku. TODO
        }*/
    }
    else
    if(writing_enabled() && data.at(0)=='!')
    {
        o+="&nbsp; "+getActionLink(fullpath+data,"RUN!","Run link read/write");
    }
    else
    if(data.at(0)=='?')
    {
        o+="&nbsp; "+getActionLink(fullpath+data,"run","Run link as read only");
    }
    else
    if(isLink(data))
    {
        o+="&nbsp; "+getActionLink(data,"follow","Follow link");
    }
    else
    if(isLocalFile(data))
    {
        if(data.at(0)=='/' || (data.at(0)=='.' && data.at(1)=='/' ))
            o+="&nbsp; "+getSeeLink(data,request,"see");//Plik sprawdzony
        else  if(writing_enabled())
            o+="&nbsp; "+getActionLink(fullpath+"!checkFile?html","check!");//Plik do sprawdzenia
    }

    if(writing_enabled())
        o+="&nbsp; "+getActionLink(fullpath+"?set&html","=","Set value");

    return o;
}

unsigned int tree_processor::countCharacters(std::string str,char c)
//https://stackoverflow.com/questions/3867890/count-character-occurrences-in-a-string-in-c
{
int count = 0, size=str.size();

for (int i = 0; i < size; i++)
  if (str[i] == c)
        count++;

return count;
}

bool tree_processor::isLink(std::string str)
{
    return str.find("http:",0)==0 || str.find("https:",0)==0
            || str.find("ftp:",0)==0 || str.find("ftps:",0)==0
            ;//More? mail? what else? TODO
}

bool tree_processor::isLocalFile(std::string str)
{
    auto len=str.length();
    return len>4
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
}

std::string tree_processor::preprocessIntoHtml(const std::string& str)
//Preprocess links and other markers into HTML tags.
//http://www.cplusplus.com/reference/regex/regex_replace/
//https://en.wikipedia.org/wiki/Emoticon, https://www.w3schools.com/charsets/ref_emoji_smileys.asp, http://defindit.com/ascii.html
{
    std::string tmp=str;

    //\r \n , ) ] i . sprawiają problemy przy połaczeniu z linkami - U+200A z http://jkorpela.fi/chars/spaces.html
    boost::replace_all(tmp,", ","\u200A, " /*"\x7f"*/);
    boost::replace_all(tmp,")","\u200A)" /*"\x7f"*/);
    boost::replace_all(tmp,"]","\u200A]" /*"\x7f"*/);
    boost::replace_all(tmp,". ","\u200A. " /*"\x7f"*/);
    boost::replace_all(tmp,"|","\u200A| " /*"\x7f"*/);
    boost::replace_all(tmp,"\r","\x7f\r" /*"\x7f"*/);
    boost::replace_all(tmp,"\n","\u200A\n" /*"\x7f"*/);

    //std::regex link(URLparser::URLpattern);
    boost::regex link(URLparser::URLpattern);
    //auto out=std::regex_replace (tmp,link,"<A HREF=\"$&\">$&</A>");
    auto out=boost::regex_replace (tmp,link,"<A HREF=\"$&\">$&</A>");

    boost::replace_all(out,"\u200A\n","\n");//Przywracamy
    boost::replace_all(out,"\u200A,",",");//Przywracamy
    boost::replace_all(out,"\u200A)",")");//Przywracamy
    boost::replace_all(out,"\u200A]","]");//Przywracamy
    boost::replace_all(out,"\u200A. ",". ");//Przywracamy

    boost::replace_all(out,"-->","&rarr;");
    boost::replace_all(out,"==>","&rArr;");
    boost::replace_all(out,":-)","&#x1F60A;");//uśmiech
    boost::replace_all(out,":)", "&#x1F642;");//inny uśmiech
    boost::replace_all(out,":-(","&#x1F61E;");//smutek
    boost::replace_all(out,":(", "&#x1F61F;");//obawa
    boost::replace_all(out,":-D","&#x1F600;");//szeroki uśmiech
    boost::replace_all(out,":D", "&#x1F603;");//inny szeroki uśmiech
    boost::replace_all(out,":P", "&#x1F60B;");//ozorek
    boost::replace_all(out,":-P","&#x1F61B;");//ozorek centralny
    boost::replace_all(out," :/", "&#x1F615;");//krzywa geba. skonfundowany - bez spacji niszczy linki!
    boost::replace_all(out,":-/","&#x1F615;");//krzywa geba
    boost::replace_all(out,";)", "&#x1F609;");//wink
    boost::replace_all(out,";-)","&#x1F609;");//wink
    boost::replace_all(out,":^)","&#x1F921;");//uśmiechnięty klown
    boost::replace_all(out,"\n","\n<BR>");//break LINEs
    return out;
}

}//namespace "fasada"
