//Base class for all json data processors working inside
//local server of "fasada" resources stored
//

#include "tree_consts.h"
#include "tree_processor.h"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>

namespace fasada
{
//Dopiero w main jest pewność że te
//wewnętrzne struktury static zostały zainicjalizowane.
//A to słabe...
tree_processor::processors_map tree_processor::map_of_readers;
tree_processor::processors_map tree_processor::map_of_writers;

tree_processor::tree_processor(Category cat,const char* name):
    procCategory(cat),procName(name)
{
    std::cerr<<"Registering "<<procName<<" processor ";
    try
    {
    if(procCategory==WRITER)
        map_of_writers[procName]=this;
    else if(procCategory==READER)
        map_of_readers[procName]=this;
    }
    catch(...)
    {
        std::cerr <<"tree_processor:"<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
    }
    //else throw(tree_processor_exception("UNKNOWN CATEGORY OF PTREE PROCESSOR "+procName));
    std::cerr<<"DONE"<<std::endl;
}

tree_processor::~tree_processor()
{
    if(procCategory==WRITER)
        map_of_writers.erase(procName);
    else if(procCategory==READER)
        map_of_readers.erase(procName);
}

//Do some work, call _implement_read, clean & return
void tree_processor::read_tree(ShmString& o,const pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory!=READER)
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT A READER"));
    _implement_read(o,top,request);
    o+=MEM_END;
}

//Do some work, call _implement_write, clean & return
void tree_processor::write_tree(ShmString& o,pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory!=WRITER)
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS NOT A WRITER"));
    _implement_write(o,top,request);
    o+=MEM_END;
}

tree_processor& tree_processor::getReadProcessor (const char* name)//may throw
{
    std::cout<<name<<std::endl;
    tree_processor* tmp=map_of_readers[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR ")+name+" NOT FOUND!"));
    return *tmp;
}


tree_processor& tree_processor::getWriteProcessor(const char* name)//may throw
{
    tree_processor* tmp=map_of_writers[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR ")+name+" NOT FOUND!"));
    return *tmp;
}

}//namespace "fasada"
