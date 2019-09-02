#include "processor_get.h"

namespace facjata
{

static processor_get GET;//Istnieje conajmniej jedna taka zmienna. I wystarczy, chyba że ktoś chce robić aliasy

processor_get::processor_get(const char* name):
    tree_processor(READER,name)
{

}

processor_get::~processor_get()
{}

void processor_get::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    std::string tmp=top.get_value<std::string>();
    if(request["verbose"]=="true")
        o+="'"+request["&path"] + "' = '" + tmp + "'";
    else
        o+=tmp;
}


void processor_get::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace facjata
