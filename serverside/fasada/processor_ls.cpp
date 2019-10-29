#include "fasada.hpp"
#include "processor_ls.h"
#include <iostream>

namespace fasada
{

processor_ls::processor_ls(const char* name):
    tree_processor(READER,name)
{

}

processor_ls::~processor_ls()
{}

void processor_ls::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

//Problem indeksowania tablic JSONów w ptree - kłopotliwy bardzo
//https://stackoverflow.com/questions/48407925/boostproperty-treeptree-accessing-arrays-first-complex-element
//Rozwiązany na poziomie serwera fasady za pomoca wstawienia kolejnych liczb po wczytaniu
void processor_ls::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    bool longformat=(request.find("long")!=request.end()?true:false);
    bool html=request["html"]!="false";

    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"])+(longformat?"<UL>\n":"\n");
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    if(top.size()>0)
    {
        std::string parentpath=request.getFullPath();
        if( *(--parentpath.end())!='/' )
            parentpath+="/";

        for(auto p:top)
        {
            if(html)
            {
                std::string fullpath=parentpath+std::string(p.first.data());
                o+=std::string(longformat?"<LI>":"")
                        +" <A href=\""+fullpath+"?"+request["&query"]+"\"><B class=fasada_path>'"
                        +std::string(p.first.data())
                        +"'</B></A>"
                        +" <A href=\""+fullpath+"?get&html&long\"> : <I>'"
                        +std::string(p.second.data())
                        +"'</I></A> "
                        +getActionLink(fullpath+"?dfs&html","*")
                        ;
            }
            else
                o+=std::string(p.first.data())+std::string(":")+std::string(p.second.data());

            if(longformat)
                o+=(html?"\n":";\n");
            else
                o+=(html?"&nbsp; ":";\t");
        }

        if(html)
        {
            o+="\n<BR>";
            if(writing_enabled())
            {
                std::string fullpath=request.getFullPath();
                o+=getActionLink(fullpath+"?add&html","ADD!");
                o+=+"&nbsp;&nbsp;"+getActionLink(fullpath+"?ren&html","REN!");
                o+=+"&nbsp;&nbsp;"+getActionLink(fullpath+"?del&html","DEL!")+"&nbsp;&nbsp;";
            }
            o+=getActionLink(request.getFullPath()+"?find&html","FIND")+"&nbsp;&nbsp;";
            o+=getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack);
        }
    }
    else
    {
        if(html)
        {
            std::string fullpath=request.getFullPath();
            o+=std::string(longformat?"<LI>":"")
                    + "<B class=fasada_path>'" + request["&path"] + "'</B> : "
                    + "<I>'"+top.data()+"'</I> ";
            if(writing_enabled())
            {
                if(top.size()==0 ) o+=getActionLink(fullpath+"?set&html","SET!");
                if(top.data()=="") o+=" "+getActionLink(fullpath+"?add&html","ADD!");
            }
        }
        else
        o+=" NO SUBNODES ";
    }

    if(longformat & html) o+="</UL>";
    if(html) o+=getHtmlClosure();
}




}//namespace "fasada"
