#include "processor_dfs.h"


namespace fasada
{

static processor_dfs DFS;//Istnieje conajmniej jedna taka zmienna. I wystarczy, chyba że ktoś chce robić aliasy

processor_dfs::processor_dfs(const char* name):
    tree_processor(READER,name)
{

}

processor_dfs::~processor_dfs()
{}

void processor_dfs::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    o+=(procName+" NOT IMPLEMENTED");
}


void processor_dfs::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
