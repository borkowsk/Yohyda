#include "fasada.hpp"
#include "processor_ls.h"
#include <boost/lexical_cast.hpp>
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

void processor_ls::_implement_action_panel(ShmString& o,URLparser& request)
//Górny i dolny panel akcji dotyczących całej listy
{
    std::string fullpath=request.getFullPath();
    if(writing_enabled())
    {
        o+=getActionLink(fullpath+"?add&html","ADD!");
        o+=+"&nbsp;&nbsp;"+getActionLink(fullpath+"?ren&html","REN!");
        o+=+"&nbsp;&nbsp;"+getActionLink(fullpath+"?del&html","DEL!")+"&nbsp;&nbsp;";
    }
    o+=getActionLink(fullpath+"?find&html","FIND")+"&nbsp;&nbsp;";
    if(longformat)
    {
        o+=getActionLink(fullpath+"?dfs&html&long","TREE")+"&nbsp;&nbsp;";
        o+=getActionLink(fullpath+"?ls&html","LSS")+"&nbsp;&nbsp;";
    }
    else
    {
        o+=getActionLink(fullpath+"?dfs&html","TREE")+"&nbsp;&nbsp;";
        o+=getActionLink(fullpath+"?ls&html&long","LSL")+"&nbsp;&nbsp;";
    }
    o+=getActionLink(request.getParentPath()+"?"+request["&query"],HTMLBack);
}

void processor_ls::_implement_node_panel(ShmString& o,const std::string& data,const std::string& fullpath)
//Panel akcji dotyczących danego węzła
{
    if(data=="")    //Czy to liść czy (potencjalny) węzeł?
    {
        o+="&nbsp;"+getActionLink(fullpath+"?dfs&html","&forall;");
        o+="&nbsp;"+getActionLink(fullpath+"?add&html","+");
        o+="&nbsp;"+getActionLink(fullpath+"?set&html","=");
    }
    else
    if(writing_enabled() && data.at(0)=='!')
    {
        o+="&nbsp;"+getActionLink(fullpath+data,"RUN!");
    }
    else
    if(data.at(0)=='?')
    {
        o+="&nbsp;"+getActionLink(fullpath+data,"run");
    }
    else
    if(writing_enabled())
        o+="&nbsp;"+getActionLink(fullpath+"?set&html","=");
}

//Problem indeksowania tablic JSONów w ptree - kłopotliwy bardzo
//https://stackoverflow.com/questions/48407925/boostproperty-treeptree-accessing-arrays-first-complex-element
//Rozwiązany na poziomie serwera fasady za pomoca wstawienia kolejnych liczb po wczytaniu
void processor_ls::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    unsigned counter=0;
    bool html=request["html"]!="false";
    longformat=(request.find("long")!=request.end()?true:false);//longformat jest prywatne


    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"]);
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    if(top.size()>0)
    {
        if(html) _implement_action_panel(o,request);
        o+="<BR><BR>\n";
        std::string parentpath=request.getFullPath();
        if( *(--parentpath.end())!='/' )
            parentpath+="/";
        o+=(longformat & html?"\n<UL>\n":"\n");
        for(auto p:top)
        {
            if(html)
            {
                std::string fullpath=parentpath+std::string(p.first.data());
                o+=std::string(longformat?"<LI>":"")
                        +" <A href=\""+fullpath+"?"+request["&query"]+"\"><B class=fasada_path>'"
                        +std::string(p.first.data())
                        +"'</B></A>"
                        +" <A href=\""+fullpath+"?get&html&long\"> : <I class=\"fasada_val\">'"
                        +std::string(p.second.data())
                        +"'</I></A> ";
                _implement_node_panel(o,p.second.data(),fullpath);
            }
            else
                o+=std::string(p.first.data())+std::string(" : ")+std::string(p.second.data());

            counter++;

            if(longformat)
                o+=(html?"\n":";\n");
            else
                o+=(html?"&nbsp; ":";\t");
        }

        if(longformat & html) o+="</UL>\n";
            else o+="\n";

        o+=boost::lexical_cast<val_string>(counter);

        if(html && counter>10)
        {
            o+="<BR>\n";
            _implement_action_panel(o,request);
        }
    }
    else
    {
        if(html)
        {
            std::string fullpath=request.getFullPath();
            o+=std::string(longformat?"<LI>":"")
                    + "<B class=fasada_path>'" + request["&path"] + "'</B> : "
                    + "<I class=\"fasada_val\">'"+top.data()+"'</I> ";

            _implement_node_panel(o,top.data(),fullpath);
            o+="&nbsp;&nbsp;"+getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack);
        }
        else
        o+=" NO SUBNODES ";
    }
    if(html) o+=getHtmlClosure();
}




}//namespace "fasada"
