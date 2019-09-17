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
// W C++ dopiero w main jest pewność że wewnętrzne struktury static
// z innych źródeł i z bibliotek zostały zainicjalizowane. To słabe...
// Robimy sztuczkę...
// https://www.bfilipek.com/2018/02/static-vars-static-lib.html
// Czyli "lazy singleton pattern"
// https://stackoverflow.com/questions/6234791/how-to-wait-until-dynamic-initialization-of-static-variables-finishes


static
processors_map& map_of_readers()
{
   static processors_map _map_of_readers;
   return _map_of_readers;
}

static
processors_map& map_of_writers()
{
   static processors_map _map_of_writers;
   return _map_of_writers;
}

tree_processor::tree_processor(Category cat,const char* name):
    procCategory(cat),procName(name)
{
    if(cat>READER)
      throw(tree_processor_exception("UNKNOWN CATEGORY OF PTREE PROCESSOR "+procName));
    
    std::cerr<<"Registering "<<procName<<" processor ";

    try
    {
    if(procCategory==WRITER)
        map_of_writers()[procName]=this;
    else if(procCategory==READER)
        map_of_readers()[procName]=this;
    }
    catch(...)
    {
        std::cerr <<"tree_processor:"<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
    }

    std::cerr<<"DONE"<<std::endl;
}

tree_processor::~tree_processor()
{
    if(procCategory==WRITER)
        map_of_writers().erase(procName);
    else if(procCategory==READER)
        map_of_readers().erase(procName);
}

tree_processor& tree_processor::getReadProcessor (const char* name)//may throw
{
    std::cout<<name<<std::endl;
    tree_processor* tmp=map_of_readers()[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR ")+name+" NOT FOUND!"));
    return *tmp;
}


tree_processor& tree_processor::getWriteProcessor(const char* name)//may throw
{
    tree_processor* tmp=map_of_writers()[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR ")+name+" NOT FOUND!"));
    return *tmp;
}

//Do some work, call _implement_read, clean & return
void tree_processor::read_tree(ShmString& o,const pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory!=READER)
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT A READER"));

    _implement_read(o,top,request);
    o+=MEM_END;//"DONE" MARKER FOR PARAREL PROCESS
}

//Do some work, call _implement_write, clean & return
void tree_processor::write_tree(ShmString& o,pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory!=WRITER)
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS NOT A WRITER"));

    _implement_write(o,top,request);
    o+=MEM_END;//"DONE" MARKER FOR PARAREL PROCESS
}

}//namespace "fasada"
