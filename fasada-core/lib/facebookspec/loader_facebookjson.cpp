#include "facebookspec.h"
#include "loader_facebookjson.h"
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace pt = boost::property_tree;

namespace facebook
{

//default HTML form for this processor
std::string loader_facebookJson::Form="";
//        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">\n"
//        "\n<input name=\"html\"   type=\"hidden\" >"
//        "FOR <B class=fasada_path>'$path'</B><BR>\n"
//        "<input type=\"submit\" value=\"OK\">\n"
//        "</form>";

loader_facebookJson::loader_facebookJson(const char* name):
    tree_processor(WRITER,name) //also may be READER if it should create its own FORM
{
}

loader_facebookJson::~loader_facebookJson()
{}


void loader_facebookJson::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("READ FUNCTION FOR PTREE PROCESSOR "+procName+" NOT IMPLEMENTED!"));
}

void loader_facebookJson::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    std::string discPath=request["&private_directory"]+request["&path"];
    boost::replace_all(discPath,"//","/");
    unsigned    noc=top.size();//czy ma elementy składowe?

    if(noc!=0)
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT LOAD NOT-LEAF NODE!"));
    }

    //....
    o+="LOADING:";
    o+=discPath;
    o+="\n";
    pt::read_json(discPath, top);

    // Most important properties is "source", "loader", "viever", "saver", "alternative_savers", "oth_actions":
    insert_property(top,"_source",discPath);
    insert_property(top,"loader",procName);
    insert_property(top,"saver","saveJson");
    insert_property(top,"alternative_savers.xml","saveAsXml");
    insert_property(top,"oth_actions.Check_All_Files","runTree&action=checkFile");

    call_recoders(top,true/*PL*/);

    //Jeśli nie ma wyjątku to nazwę procesora likwidujemy
    top.data()="";
    o+="DONE";
}

}//namespace Facebook
