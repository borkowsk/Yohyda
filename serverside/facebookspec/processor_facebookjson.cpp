#include "processor_facebookjson.h"

namespace facebook
{

//default HTML form for this processor
std::string processor_facebookJson::Form="";
//        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">\n"
//        "FOR <B class=fasada_path>'$path'</B><BR>\n"
//        "<input type=\"submit\" value=\"OK\">\n"
//        "</form>";

processor_facebookJson::processor_facebookJson(const char* name):
    tree_processor(WRITER,name) //also READER because is able to create its own FORM
{
}

processor_facebookJson::~processor_facebookJson()
{}


void processor_facebookJson::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("READ FUNCTION FOR PTREE PROCESSOR "+procName+" NOT IMPLEMENTED!"));
}

void processor_facebookJson::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    std::string fullpath;
    unsigned    noc=top.size();//czy ma elementy składowe?

    if(noc!=0)
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT LOAD NOT-LEAF NODE!"));
    }

    //....

}

}//namespace Facebook
