#include "fasada.hpp"
#include "tree_processor.h"
#include "loader_json.h"
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include "tree/ptree_foreach.hpp"

namespace fasada
{

loader_json::loader_json(const char* name):
        tree_processor(WRITER,name) //also may be READER if it should create its own FORM
{

}

void loader_json::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("READ FUNCTION FOR PTREE PROCESSOR "+procName+" NOT IMPLEMENTED!"));
}

void loader_json::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
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

    o+="inserting consecutive numbers as table indexes...\n";
    insert_numbers(top);

    //Jeśli nie ma wyjątku to nazwę procesora likwidujemy
    top.data()="";
    o+="DONE";
}

}
