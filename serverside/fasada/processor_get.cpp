#include "tree_types.h"
#include "tree_consts.h"
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

    o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    if(request["verbose"]=="true")
        o+="'"+request["&path"] + "' = '" + tmp + "' ["+boost::lexical_cast<std::string>(noc)+"]";
    else
        o+=tmp;
}


void processor_get::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
