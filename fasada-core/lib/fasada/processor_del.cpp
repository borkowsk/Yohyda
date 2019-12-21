/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///
#define UNIT_IDENTIFIER "processor_del"
#include "fasada.hpp"
#include "processor_del.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

namespace fasada
{

//default HTML form for this processor
std::string processor_del::Form=
        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">"
        "\n<input name=\"html\"   type=\"hidden\" >"
        "\n<B>&minusd; &minusd; &minusd; &minusd; &minusd; &minusd;</B>"
        "\n<BR>NAME: "
        "<input type=\"text\" name=\"name\" size=\"" STR_DEFAULT_LEN_OF_NAME "\"><br>\n"
        "will be DELeted from <B class=fasada_path>'$path'</B><BR>\n"
        "<input type=\"submit\" value=\"OK\">\n"
        "\n&nbsp;<input type=\"button\" value=\"CANCEL\" onclick=\"window.history.back();\" >"
        "\n&nbsp;<a class=\"fasada_action\" href=\"${fullpath}?ls&html&long\" >LSL</a> "
        "\n&nbsp;<a class=\"fasada_action\" href=\"${fullpath}?dfs&html&long\" >TREE</a> "
        "</form>";

processor_del::processor_del(const char* name):
    form_processor(name) //also READER because is able to create its own FORM
{
}

processor_del::~processor_del()
{}


void processor_del::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
//Implement_read WRITER'a powinno przygotować FORM jeśli jest to format "html"
{
    std::string fullpath=request.getFullPath();
    std::string tmp=top.get_value<std::string>();
    unsigned    noc=top.size();//czy ma elementy składowe?
    bool html=request.asHTML();

    if(html)//TYPE HEADER AND HTML HEADER
    {
         o+=ipc::string(EXT_PRE)+"htm\n";
         o+=getPageHeader(fullpath)+"\n";
         if(top.data()=="" && noc>0)
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
         o+=getPageClosure(_compiled);
    }
    else
    {
        o+=ipc::string(EXT_PRE)+"txt\n";
        o+="FOR DELETE USE: "+fullpath+"?!"+procName+"&name=[NAME]";
    }
}

void processor_del::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    std::string fullpath;

    if(top.data()!="")//Jeśli ma wartość własną to jest liściem
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT DELETE CHILD FROM LEAF NODE!"));

    std::string name=request["name"];
    if(name=="")
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT DELETE NONAME CHILD"));

    bool html=request.asHTML();

    if(html)
    {
        fullpath=request.getFullPath();
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getPageHeader(fullpath)+"\n<P>";
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";

    //top.put(name,request["value"]);//Not duplicate names.
    top.erase(name);

    if(html)
    {
       o+="DONE <B class=fasada_path>'"+request["&path"]+"/"+name+"'</B> deleted<BR>\n";
       o+="\n"+getActionLink(fullpath+"?ls&long&html",HTMLBack,"Go back");
       o+="</P>";
       o+=getPageClosure(_compiled);
    }
    else o+="DONE '"+name+"'='"+top.get_child(name).data()+"'";
}

}//namespace "fasada"
