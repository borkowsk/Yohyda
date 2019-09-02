#include "processor_ls.h"

namespace facjata
{

static processor_ls LS;//Istnieje conajmniej jedna taka zmienna. I wystarczy, chyba że ktoś chce robić aliasy

processor_ls::processor_ls(const char* name):
    tree_processor(READER,name)
{

}

processor_ls::~processor_ls()
{}

void processor_ls::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{

    o+=(procName+"OK").c_str();
}


void processor_ls::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace facjata
