#include "tree_types.h"
#include "processor_dfs.h"
#include "ptree_foreach.hpp"


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
    foreach_node(top,"",
    [&o,defret](const ptree& t,std::string k)
    {
        o+="[";
        o+=k;
        o+="] : '";
        o+=t.data();
        o+="'\n";
        return defret;//wynik nie blokuje, ale "before" jest never więc nie ma "after"
    }
    );
}


void processor_dfs::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
