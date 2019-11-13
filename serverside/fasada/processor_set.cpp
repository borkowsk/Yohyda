#include "fasada.hpp"
#include "processor_set.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

namespace fasada
{

//default HTML form for this processor
std::string processor_set::Form=
        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">\n"
        "VALUE: "
        "<input type=\"text\" size=\"$size_of_value\" name=\"value\" value=\"$value\" ><br>\n"
        "FOR <B class=fasada_path>'$path'</B><BR>\n"
        "<input type=\"submit\" value=\"OK\">\n"
        "</form>";

processor_set::processor_set(const char* name):
    tree_processor(WRITER_READER,name) //also READER because is able to create its own FORM
{
}

processor_set::~processor_set()
{}

void processor_set::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
//Implement_read WRITER'a powinno przygotować FORM jeśli jest to format "html"
{
    std::string fullpath=request.getFullPath();
            //request["&protocol"]+"://"+request["&domain"]+':'+request["&port"]+request["&path"];
    std::string tmp=top.get_value<std::string>();
    unsigned    noc=top.size();//czy ma elementy składowe?
    bool html=request["html"]!="false";

    if(html)//TYPE HEADER AND HTML HEADER
    {
         o+=ipc::string(EXT_PRE)+"htm\n";
         o+=getHtmlHeaderDefaults(fullpath)+"\n";
         if(noc==0)
         {
             //Podmienić procesor, ścieżki, wartość domyślną i ewentualputnie inne zmienne
             std::string ReadyForm=Form;
             boost::replace_all(ReadyForm,"$proc",procName);///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
             boost::replace_all(ReadyForm,"$fullpath",fullpath);
             boost::replace_all(ReadyForm,"$path",request["&path"]);
             unsigned value_size=tmp.size();
             if(value_size<1) value_size=UINT_DEFAULT_LEN_OF_NAME;
             if(value_size>132) value_size=132; //TODO TextAreA
             boost::replace_all(ReadyForm,"$size_of_value", boost::lexical_cast<std::string>(value_size) );
             boost::replace_all(ReadyForm,"$value",tmp);
             o+=ReadyForm;
         }
         else
             o+="<H2>WARNING!</H2><P>Only leaf type nodes could be modified by '"+procName+"'</P>";
         o+=getHtmlClosure();
    }
    else
    {
        o+=ipc::string(EXT_PRE)+"txt\n";
        o+="FOR CHANGE USE: "+fullpath+"?!"+procName+"&value=[NEW_VALUE]";
    }
}

void processor_set::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    std::string fullpath;
    unsigned    noc=top.size();//czy ma elementy składowe?

    if(noc!=0)
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT CHANGE VALUE OF NOT-LEAF NODE!"));
    }

    bool html=request["html"]!="false";

    if(html)
    {
        fullpath=request.getFullPath();
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(fullpath)+"\n<P>";
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";

    top.data()=request["value"];

    if(html)
    {
       o+="DONE <I class=\"fasada_val\">'"+top.data()+"'</I>";
       o+="\n"+getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack,"Go back");
       o+="</P>";
       o+=getHtmlClosure();
    }
    else o+="DONE '"+top.data()+"'";
}

}//namespace "fasada"
