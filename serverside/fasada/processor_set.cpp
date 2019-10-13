#include "tree_types.h"
#include "tree_consts.h"
#include "processor_set.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{


processor_set::processor_set(const char* name):
    tree_processor(Category(WRITER+READER),name) //READER because is able to create its own FORM
{
}

processor_set::~processor_set()
{}

void processor_set::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
//Implement_read WRITER'a powinno przygotować FORM jeśli jest to format "html"
{
    std::string tmp=top.get_value<std::string>();
    unsigned    noc=top.size();
    bool html=request["html"]=="true";
    if(html)//TYPE HEADER AND HTML HEADER
    {
         o+=ipc::string(EXT_PRE)+"htm\n<HTML>";
         o+="<HEAD>"+getHtmlHeaderDefaults()+"<TITLE>'"+request["&path"]+"'</TITLE></HEAD><BODY><P>";
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";

    if(request["verbose"]=="true" || request["long"]=="true")
    {
        o+="'"+request["&path"]+ "' = '" + tmp + "'";
        if(noc && html)
        {
           std::string fullpath=request["&protocol"]+"://"+request["&domain"]+':'+request["&port"]+request["&path"];
           o+=" [ <a href=\""+fullpath+"?ls&long&html\">"
            +boost::lexical_cast<std::string>(noc)
            +"</a> ]";
        }
        else
            o+=" ["+boost::lexical_cast<std::string>(noc)+"]";
    }
    else
        o+=tmp;

    if(html) o+="</P></BODY></HTML>";
}


void processor_set::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
//Implement_write WRITER'a powinno zmienić wartości na powstawie FORMularza z method==GET
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS NOT IMPLEMENTED YET!"));
}

}//namespace "fasada"


