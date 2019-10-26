#include "tree_types.h"
#include "tree_consts.h"
#include "processor_ren.h"
//include <boost/process/io.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

namespace fasada
{

//default HTML form for this processor
std::string processor_ren::Form=
        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">\n"
        "OLD NAME: "
        "<input type=\"text\" name=\"old_name\" size=\"" STR_DEFAULT_LEN_OF_NAME "\"><br>\n"
        "NEW NAME: "
        "<input type=\"text\" name=\"new_name\" size=\"" STR_DEFAULT_LEN_OF_NAME "\"><br>\n"
        "will be RENamed in <B class=fasada_path>'$path'</B> <br>"
        "<input type=\"submit\" value=\"OK\">"
        "</form>";


processor_ren::processor_ren(const char* name):
    tree_processor(WRITER_READER,name) //also READER because is able to create its own FORM
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
    bool html=request["html"]!="false";

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
        o+="FOR ren USE: "+fullpath+"?!"+procName+"&name=[NEW_NAME]&value=[NEW_VALUE]";
    }
}

using boost::property_tree::ptree;

void processor_ren::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{

    std::string fullpath;

    std::string old_name=request["old_name"];
    if(old_name=="")
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT RENAME CHILD WITHOUT NAME!"));

    std::string new_name=request["new_name"];
    if(new_name=="")
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT RENAME CHILD INTO EMPTY NAME!"));

    bool html=request["html"]!="false";

    if(html)
    {
        fullpath=request.getFullPath();
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(fullpath)+"\n<P>";
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";

    //top.ren_child(old_name,new_name);//NIE MA CZEGOŚ TAKIEGO
    //ptree::iterator
    ptree::iterator ne = top.to_iterator(top.find(new_name));
    //if(ne!=top.not_found())
      //  throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT RENAME INTO EXISTING '"+new_name+"'!"));

    ptree::iterator it = top.to_iterator(top.find(old_name));
    //if(it==top.not_found())
      //  throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' CANNOT FIND CHILD WITH NAME '"+old_name+"'!"));

    top.insert(it, make_pair(new_name, it->second));
    top.erase(it);

    if(html)
    {
       o+="DONE <B class=fasada_path>'"+old_name+"'</B>--><B>'"+new_name+"'</B>";
       o+="\n"+getActionLink(fullpath+"?ls&long&html",HTMLBack);
       o+="</P>";
       o+=getHtmlClosure();
    }
    else o+="DONE '"+old_name+"'-->'"+new_name+"'";
}


}//namespace "fasada"
