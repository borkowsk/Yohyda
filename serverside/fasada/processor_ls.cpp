#include "tree_types.h"
#include "tree_consts.h"
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

//TODO - problem indeksowania tablic JSONów w ptree - kłopotliwy bardzo
//https://stackoverflow.com/questions/48407925/boostproperty-treeptree-accessing-arrays-first-complex-element
//Do rozwiązania na poziomie serwera fasady za pomoca wstawienia kolejnych liczb
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
        for(auto p:top)
        {
            //std::cerr<<p.first.data()<<":"<<p.second.data()<<std::endl;
            if(html)
            {
                std::string fullpath=request["&protocol"]+"://"+request["&domain"]+':'+request["&port"]+request["&path"]+std::string(p.first.data());
                o+=std::string(longformat?"<LI>":"")
                        +" <a href=\""+fullpath+"/?"+request["&query"]+"\">"
                        +std::string(p.first.data())
                        +"</a>"
                        +" <a href=\""+fullpath+"/?get&html&long\">:"
                        +std::string(p.second.data())
                        +"</a>"
                        +" <a href=\""+fullpath+"/?dfs&html\">*</a>"
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
            std::string fullpath=request.getFullPath();
            o+="<BR>[ <a href=\""+fullpath+"?add&html\">add!</a> ]";
        }
    }
    else
    {
        if(html)
        {
            std::string fullpath=request.getFullPath();
            o+=std::string(longformat?"<LI>":"")
             + "'" + request["&path"] + "' ";
             if(top.size()==0) o+="[ <a href=\""+fullpath+"?set&html\">change!</a> ]";
             if(top.data()=="") o+="[ <a href=\""+fullpath+"?add&html\">add!</a> ]";
        }
        else
        o+=" NO SUBNODES ";
    }

    if(longformat & html) o+="</UL>";
    if(html) o+=getHtmlClosure();
}


void processor_ls::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
