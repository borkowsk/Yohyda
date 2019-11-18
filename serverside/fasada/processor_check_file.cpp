#include "processor_check_file.h"

namespace fasada
{

processor_check_file::processor_check_file(const char* name):
    tree_processor(WRITER_READER,name)
{

}

processor_check_file::~processor_check_file()
{}


void processor_check_file::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED!"));
}

void processor_check_file::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED!!"));
}

}//namespace "fasada"
