#include "tree_types.h"
#include "tree_consts.h"
#include "processor_dfs.h"
#include "tree/ptree_foreach.hpp"
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <algorithm>

namespace fasada
{

processor_dfs::processor_dfs(const char* name):
    tree_processor(READER,name)
{}

processor_dfs::~processor_dfs()
{}

void processor_dfs::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

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
    [&o,defret,html,&request](const ptree& t,std::string k)
    {
        o+="[";
        std::string pathk=k;
        std::replace( pathk.begin(), pathk.end(),'.','/');
        if(html) o+="<B class=fasada_path><A HREF=\""+request.getFullPath()+"/"+pathk+"?ls&html&long\">";
        o+=pathk;
        if(html) o+="</A></B>] : <I>'";
        else o+="] : '";
        o+=t.data();
        o+="'";
        if(html) o+="</I><BR>\n";
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

}//namespace "fasada"
