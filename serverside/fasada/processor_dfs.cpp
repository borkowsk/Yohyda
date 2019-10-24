#include "tree_types.h"
#include "tree_consts.h"
#include "processor_dfs.h"
#include "tree/ptree_foreach.hpp"


namespace fasada
{

processor_dfs::processor_dfs(const char* name):
    tree_processor(READER,name)
{

}

processor_dfs::~processor_dfs()
{}

void processor_dfs::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    bool defret=(request["return"]!="false");
    bool longformat=(request.find("long")!=request.end()?true:false);
    bool html=request["html"]!="false";

    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"])+(longformat?"<UL>\n":"\n");
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    foreach_node(top,"",
    [&o,defret,html](const ptree& t,std::string k)
    {
        o+="[";
        if(html) o+="<B>";
        o+=k;
        if(html) o+="</B>";
        o+="] : '";
        o+=t.data();
        o+="'";
        if(html) o+="<BR>\n";
        else o+="\n";
        return defret;//wynik nie blokuje, ale "before" jest "never" więc nie ma "after"
    }
    );

    if(html)
    {
        if(longformat) o+="</UL>\n";
        o+=getHtmlClosure();
    }
}


void processor_dfs::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
