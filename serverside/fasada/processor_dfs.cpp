#include "fasada.hpp"
#include "processor_dfs.h"
#include "tree/ptree_foreach.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <algorithm>
#include <string>

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
    unsigned counter=0;
    bool defret=(request["return"]!="false");
    bool longformat=(request.find("long")!=request.end()?true:false);
    bool html=request["html"]!="false";
    auto fullpath=request.getFullPath();
    if( *(--fullpath.end())!='/' )
        fullpath+="/";

    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"])+(longformat?"<UL>\n":"\n");
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    foreach_node(top,"",
    [&o,defret,longformat,html,&request,fullpath,&counter](const ptree& t,std::string k)
        {
            counter++;
            o+=(longformat && html ? "<LI>":"");
            std::string pathk=k;
            if(html) o+="<B class=fasada_path><A HREF=\""+fullpath+pathk+"?ls&html&long\">";
            o+=pathk;
            if(html) o+="</A></B> : <I class=\"fasada_val\">'";
            else o+=" : '";
            if(longformat)
                o+=t.data();
            else
            {
                auto sub=t.data().substr(0,40);
                if(sub.size()==40) sub+="...";
                o+=sub;
            }
            o+="'";
            if(html) o+="</I><BR>\n";
            else o+="\n";
            return defret;//wynik nie blokuje, ale "before" jest "never" więc nie ma "after"
        },never,never,"/"
    );

    if(html)
    {
        if(longformat) o+="</UL>\n";
        o+=boost::lexical_cast<val_string>(counter)+"<BR>\n";
        o+=getActionLink(fullpath+"?find&html","FIND")+"&nbsp;&nbsp;";
        o+=getActionLink(fullpath+"?ls&html&long","LSL")+"&nbsp;&nbsp;";
        o+=getActionLink(fullpath+"?ls&html","LSS")+"&nbsp;&nbsp;";
        o+=getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack);
        o+=getHtmlClosure();
    }
}

}//namespace "fasada"
