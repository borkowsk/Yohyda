//Base class for all json data processors working inside
//local server of "fasada" resources stored
//

#include "tree_consts.h"
#include "tree_processor.h"
#include <iostream>

namespace fasada
{
using processors_map=std::map<key_string,tree_processor*> ;
using processor_pair=std::pair<key_string,tree_processor*>;
static processors_map map_of_readers;
static processors_map map_of_writers;

tree_processor::tree_processor(Category cat,const char* name):
    procCategory(cat),procName(name)
{
    if(procCategory==WRITER)
        map_of_writers[procName]=this;
    else if(procCategory==READER)
        map_of_readers[procName]=this;
    else throw(tree_processor_exception("UNKNOWN CATEGORY OF PTREE PROCESSOR "+procName));
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
