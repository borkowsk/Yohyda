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
#include "loader_json.h"
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include "tree/ptree_foreach.hpp"

namespace fasada
{

loader_json::loader_json(const char* name):
        loader_processor(name) //also may be READER if it should create its own FORM
{}

void loader_json::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    bool        html=request.asHTML();
    if(html)
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request.getFullPath())+"\n<PRE>\n";
    }

    std::string discPath=request["&private_directory"]+request["&path"];
    boost::replace_all(discPath,"//","/");

    if(top.size()!=0  && request["force"]!="true" ) //Jak już jest zawartość to trzeba być pewnym
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT LOAD NOT-LEAF NODE!\nUse &force=true if You are sure."));
    }

    o+="OPENING:";
    o+=discPath;
    std::ifstream source(discPath);
    o+="\n";
    o+="CHECKING:";
    auto Character=source.get();
    source.unget();
    o+=std::string(">>")+char(Character)+std::string("<<\n");
    char line[LINE_MAX]="";
    if(Character!='{' && Character!='[')
    {
        source.getline(line,LINE_MAX,'=');
        o+=std::string(">>")+line+std::string("<<\n");
    }
    o+="LOADING:";
    pt::read_json(source, top);
    o+="\n";

    o+="inserting consecutive numbers as table indexes...\n";
    insert_ids(top,"id");
    insert_ids(top,"ID");
    insert_numbers(top);

    //Jeśli nie ma wyjątku to nazwę procesora likwidujemy
    top.data()="";

    // Most important properties is "_source", "loader", "viewer", "saver", "alternative_savers", "oth_actions":
    insert_property(top,"_lead",line);
    insert_property(top,"_source",discPath+" ");//SPACE is intentional. It blocs the next CHECKs.
    insert_property(top,"_loadtimestamp",std::to_string(time(NULL)));
    insert_property(top,"loader", procName);
    insert_property(top,"saver", "saveJson");
    insert_property(top,"alternative_savers.xml","saveAsXml");
    insert_property(top,"oth_actions.Check_All_Locals","runTree&action=checkFile");
    insert_property(top,"oth_actions.Load_All_Jsons","runTree&action=Json");

    o+="DONE";

    if(html)
    {
        o+="\n</PRE>\n";
        o+=getActionLink(request.getFullPath()+"?ls&html&long","LSL","List as long content of "+request["&path"])+"&nbsp;&nbsp; ";
        o+=getHtmlClosure(_compiled);
    }
}

}
