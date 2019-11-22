#include "fasada.hpp"
#include "processor_add.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

namespace fasada
{

//default HTML form for this processor
std::string processor_add::Form=
        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">\n"
        "\n<input name=\"html\"   type=\"hidden\" >"
        "NAME:&nbsp; "
        "<input type=\"text\" name=\"name\" size=\"" STR_DEFAULT_LEN_OF_NAME "\"><br>\n"
        "VALUE: "
///        "<input type=\"text\" name=\"value\" size=\"" STR_WIDTH_MAX_OF_FIELD "\"><br>\n"
        "<textarea name=\"value\" rows=\"1\" cols=\"" STR_WIDTH_MAX_OF_FIELD "\"></textarea><br>"
        "WILL BE ADDED TO <B class=fasada_path>'$path'</B> <br>"
        "<input type=\"submit\" value=\"OK\">"
        "</form>";

processor_add::processor_add(const char* name):
    tree_processor(WRITER_READER,name) //also READER because is able to create its own FORM
{
}

processor_add::~processor_add()
{}

void processor_add::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
//Implement_read WRITER'a powinno przygotować FORM jeśli jest to format "html"
{
    std::string fullpath=request.getFullPath();
    std::string tmp=top.get_value<std::string>();
    unsigned    noc=top.size();//czy ma elementy składowe?
    bool html=request.asHTML();

    if(html)//TYPE HEADER AND HTML HEADER
    {
         o+=ipc::string(EXT_PRE)+"htm\n";
         o+=getHtmlHeaderDefaults(fullpath)+"\n";
         if(top.data()=="")
         {
             //Podmienić ścieżkę i wartość domyślną
             std::string ReadyForm=Form;
             boost::replace_all(ReadyForm,"$proc",procName);
             boost::replace_all(ReadyForm,"$fullpath",fullpath);
             boost::replace_all(ReadyForm,"$path",request["&path"]);
             o+=ReadyForm;
         }
         else
         {
             o+="<H2>WARNING!</H2><P>Only not-leaf type nodes could be modified by '"+procName+"'</P>";
         }
         o+=getHtmlClosure();
    }
    else
    {
        o+=ipc::string(EXT_PRE)+"txt\n";
        o+="FOR ADD USE: "+fullpath+"?!"+procName+"&name=[NEW_NAME]&value=[NEW_VALUE]";
    }
}

void processor_add::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    std::string fullpath;

    if(top.data()!="")//Jeśli ma wartość własną to jest liściem
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT ADD CHILD INTO LEAF NODE!"));

    std::string name=request["name"];
    if(name=="")
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT ADD CHILD WITHOUT NAME!"));

    bool html=request.asHTML();

    if(html)
    {
        fullpath=request.getFullPath();
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(fullpath)+"\n<P>";
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";

    //top.add_child(name,pt::ptree{request["value"]});//May duplicate node names!
    top.put(name,request["value"]);//Not duplicate names.

    if(html)
    {
       o+="DONE <B class=fasada_path>'"+name+"'</B> = <I class=\"fasada_val\">'"+top.get_child(name).data()+"'</I>";
       o+="\n"+getActionLink(fullpath+"?ls&long&html",HTMLBack,"Go back");
       o+="</P>";
       o+=getHtmlClosure();
    }
    else o+="DONE '"+name+"'='"+top.get_child(name).data()+"'";
}

}//namespace "fasada"
