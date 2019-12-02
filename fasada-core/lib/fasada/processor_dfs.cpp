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
#include "processor_dfs.h"
#include "tree/ptree_foreach.hpp"
#include <boost/lexical_cast.hpp>
//#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
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

void processor_dfs::_implement_action_panel(ShmString& o,URLparser& request)
//Górny i dolny panel akcji dotyczących całej listy
{
    std::string fullpath=request.getFullPath();
    o+="\n";//For cleaner HTML code
    o+=getActionLink(fullpath+"?dfs&html"+(!request.asLONG()?"&long":""),"TREE",
                    (request.asLONG()?"View tree in short format":"View tree in long format"))
                    +"&nbsp;&nbsp; ";
    o+=getActionLink(fullpath+"?find&html","FIND","Find nodes")+"&nbsp;&nbsp; ";
    o+=getActionLink(fullpath+"?ls&html&long","LSL","List long")+"&nbsp;&nbsp; ";
    o+=getActionLink(fullpath+"?ls&html","LSS","List short")+"&nbsp;&nbsp; ";
    o+=getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack,"Go back");
    o+="\n";//For cleaner HTML code
}

void processor_dfs::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    unsigned counter=0;
    bool     defret=(request["return"]!="false");
    bool     longformat=request.asLONG();
    bool     html=request.asHTML();

    auto     fullpath=request.getFullPath();
    if( *(--fullpath.end())!='/' )
        fullpath+="/";

    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"]);
         _implement_action_panel(o,request);
        o+=(longformat?"<BR><UL>\n":"<BR>\n");
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
            {
                if(html)
                    o+="<A HREF=\""+fullpath+pathk+"?get&html&long\">";
                o+=t.data();
                if(html)
                    o+="</A>'</I>&nbsp; "
                            +getNodePanel(t.data(),fullpath+pathk,request)+"\n";
                else o+="'\n";
            }
            else
            {
                auto sub=t.data().substr(0,40);
                if(sub.size()==40) sub+="...";

                o+=sub;

                if(html)
                    o+="'</I><BR>\n";
                else
                    o+="'\n";
            }

            return defret;//wynik nie blokuje, ale "before" jest "never" więc nie ma "after"
        },never,never,"/"
    );

    if(html)
    {
        if(longformat) o+="</UL>\n";
        o+=boost::lexical_cast<val_string>(counter)+"<BR>\n";
        if(counter>10) _implement_action_panel(o,request);
        o+="\n"+getHtmlClosure();
    }
}

}//namespace "fasada"
