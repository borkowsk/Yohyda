#include "fasada.hpp"
#include "processor_get.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

processor_get::processor_get(const char* name):
    tree_processor(READER,name)
{

}

processor_get::~processor_get()
{}

void processor_get::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    std::string tmp=top.get_value<std::string>();
    unsigned    noc=top.size();
    bool html=request["html"]!="false";
    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"])+"<P>";
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";

    if(request["verbose"]=="true" || request["long"]=="true")
    {
        if(html)
        {
           o+="<B class=fasada_path>'"+request["&path"]+ "'</B> = <I class=\"fasada_val\">'" + tmp + "'</I>";
           std::string fullpath=request.getFullPath();
           if(noc>0)
           {
               o+=" ["+getActionLink(fullpath+"?ls&long&html",
                    boost::lexical_cast<std::string>(noc) )+"]";
           }
           else
           {
               if(writing_enabled())
                   o+="&nbsp;&nbsp;"+getActionLink(fullpath+"?set&html","SET!");
               if(writing_enabled() && top.data()=="" )
                   o+="&nbsp;&nbsp;"+getActionLink(fullpath+"?add&html","ADD!");
               if(writing_enabled() && top.data().at(0)=='!')
                   o+="&nbsp;&nbsp;"+getActionLink(fullpath+top.data(),"RUN!");
               else  if(top.data().at(0)=='?')
                   o+="&nbsp;&nbsp;"+getActionLink(fullpath+top.data(),"RUN");
           }
           o+="&nbsp;&nbsp;"+getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack);
        }
        else
        {
            o+="'"+request["&path"]+ "' = '" + tmp + "'";
            o+=" ["+boost::lexical_cast<std::string>(noc)+"]";
        }
    }
    else
        o+=tmp;

    if(html) o+="</P>"+getHtmlClosure();
}


void processor_get::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
