//Base class for all json data processors working inside
//local server of "fasada" resources stored
//

#include "fasada.hpp"
#include "tree_processor.h"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <iostream>

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
        "$action_href"
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

    std::cerr<<"Registering "<<procName<<" processor ";

    try
    {
        //WRITER could be also a READER - for preparing a FORM
        if((procCategory & WRITER) !=0 )
        {
            map_of_writers()[procName]=this;
            std::cout<<procName<<" registered as WRITER"<<std::endl;
        }
        if((procCategory & READER) !=0 )
        {
            map_of_readers()[procName]=this;
            std::cout<<procName<<" registered as READER"<<std::endl;
        }
    }
    catch(...)
    {
        std::cerr <<"tree_processor:"<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
    }

    std::cerr<<"DONE"<<std::endl;
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

std::string  tree_processor::getActionLink(const std::string& Href,const std::string& Content)
{
    std::string ReadyLink=HTMLAction;
    boost::replace_all(ReadyLink,"$action_href",Href);
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
        else
            link+=request["&path"]+"/"+data;
        out+="<a class=\"fasada_view\" href=\""+link+"\" title=\""+link+"\">"+Content+"</a>";
    }
    return out;
}

std::string  tree_processor::getHtmlClosure()
//Compatible set of tags for end of html document
{
    return HTMLFooter;
}


bool tree_processor::is_link(std::string str)
{
    return str.find("http:",0)==0 || str.find("https:",0)==0
            || str.find("ftp:",0)==0 || str.find("ftps:",0)==0;
}

bool tree_processor::is_local_file(std::string str)
{
    auto len=str.length();
    return str.rfind(".html",len-5)==len-5 || str.rfind(".htm",len-4)==len-4
            || str.rfind(".gif",len-4)==len-4 || str.rfind(".png",len-4)==len-4
            || str.rfind(".jpeg",len-5)==len-5 || str.rfind(".jpg",len-4)==len-4
            || str.rfind(".mp4",len-4)==len-4
            ;
}

}//namespace "fasada"
