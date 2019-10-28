#include "fasada.hpp"
#include "processor_find.h"
#include "tree/ptree_foreach.hpp"
#include <iostream>

namespace fasada
{

processor_find::processor_find(const char* name):
    tree_processor(READER,name)
{}

processor_find::~processor_find()
{}

void processor_find::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

void processor_find::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS NOT IMPLEMENTED YET!"));
}


}//namespace "fasada"
