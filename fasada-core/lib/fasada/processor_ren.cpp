/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

#include "fasada.hpp"
#include "processor_ren.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>

namespace fasada
{

//default HTML form for this processor
std::string processor_ren::Form=
        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">\n"
        "<input type=\"hidden\" name=\"html\" >\n"
        "<br><B>&mnplus; &mnplus; &mnplus; &mnplus; &mnplus; &mnplus;</B>"
        "\n<BR>OLD NAME: "
        "<input type=\"text\" name=\"old_name\" size=\"" STR_DEFAULT_LEN_OF_NAME "\"><br>\n"
        "NEW NAME: "
        "<input type=\"text\" name=\"new_name\" size=\"" STR_DEFAULT_LEN_OF_NAME "\"><br>\n"
        "will be RENamed in <B class=fasada_path>'$path'</B> <br>"
        "<input type=\"submit\" value=\"OK\">"
        "\n&nbsp;<input type=\"button\" value=\"CANCEL\" onclick=\"window.history.back();\" >"
        "</form>";


processor_ren::processor_ren(const char* name):
    form_processor(name) //also READER because is able to create its own FORM
{
}

processor_ren::~processor_ren()
{}

void processor_ren::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
//Implement_read WRITER'a powinno przygotować FORM jeśli jest to format "html"
{
    std::string fullpath=request.getFullPath();
    std::string tmp=top.get_value<std::string>();
    unsigned    noc=top.size();//czy ma elementy składowe?
    bool        html=request.asHTML();

    if(html)//TYPE HEADER AND HTML HEADER
    {
         o+=ipc::string(EXT_PRE)+"htm\n";
         o+=getHtmlHeaderDefaults(fullpath)+"\n";
         if(top.data()=="")
         {
             //Podmienić ścieżkę i wartość domyślną
             std::string ReadyForm=Form;
             boost::replace_all(ReadyForm,"$proc",procName);
             boost::replace_all(ReadyForm,"$fullpath",fullpath);        //ptree::iterator
             boost::replace_all(ReadyForm,"$path",request["&path"]);
             o+=ReadyForm;
         }
         else
         {
             o+="<H2>WARNING!</H2><P>Only not-leaf type nodes could be modified by '"+procName+"'</P>";
         }
         o+=getHtmlClosure(_compiled);
    }
    else
    {
        o+=ipc::string(EXT_PRE)+"txt\n";
        o+="FOR ren USE: "+fullpath+"?!"+procName+"&name=[NEW_NAME]&value=[NEW_VALUE]";
    }
}

using boost::property_tree::ptree;

void processor_ren::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    bool succes=false;
    std::string fullpath;

    std::string old_name=request["old_name"];
    if(old_name=="")
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT RENAME CHILD WITHOUT NAME!"));

    std::string new_name=request["new_name"];
    if(new_name=="")
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT RENAME CHILD INTO EMPTY NAME!"));

    bool html=request.asHTML();

    if(html)
    {
        fullpath=request.getFullPath();
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(fullpath)+"\n<P>";
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";

    try{
        //top.ren_child(old_name,new_name);//NIE MA CZEGOŚ TAKIEGO        //ptree::iterator

        ptree::iterator ne = top.to_iterator(top.find(new_name));
        if(ne!=top.end())
          throw(tree_processor_exception("PTREE PROCESSOR '"+procName
                                         +"' CANNOT RENAME INTO EXISTING '"+new_name+"'!"));

        ptree::iterator it = top.to_iterator(top.find(old_name));
        if(it==top.end())
          throw(tree_processor_exception("PTREE PROCESSOR '"+procName
                                         +"' CANNOT FIND CHILD WITH NAME '"+old_name+"'!"));        //ptree::iterator

        top.insert(it, make_pair(new_name, it->second));
        top.erase(it);
        succes=true;
    }
    catch(tree_processor_exception& ex)
    {
        o+=std::string("\n")+ex.what()+"\n";
    }
    catch(...)
    {
        std::cerr <<procName<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        o+="\nEXCEPTION IN PTREE PROCESSOR '"+procName+"': "
           +boost::current_exception_diagnostic_information()+"\n";
    }

    if(succes)
        if(html)
        {
           o+="DONE <B class=fasada_path>'"+old_name+"'</B>&rarr;<B>'"+new_name+"'</B>";
           o+="\n"+getActionLink(fullpath+"?ls&long&html",HTMLBack,"Go back");
           o+="</P>";
        }
        else o+="DONE '"+old_name+"'-->'"+new_name+"'";

    if(html)  o+="</P>"+getHtmlClosure(_compiled);
}


}//namespace "fasada"
