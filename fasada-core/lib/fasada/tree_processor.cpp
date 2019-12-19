/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

//Base class for all json data processors working inside
//local server of "fasada" resources stored
//
#define UNIT_IDENTIFIER "Core_units"
#include "fasada.hpp"
#include "tree_processor.h"
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>
#include <string>
/// See:
/// https://en.cppreference.com/w/cpp/preprocessor/replace
/// __STDCPP_THREADS__ (C++11)
///  and others new macros

namespace fasada
{
// W C++ dopiero w main jest pewność że wewnętrzne struktury static
// z innych źródeł i z bibliotek zostały zainicjalizowane. To słabe, ale tak jest w C++
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
    if(cat>CONTROL)
      throw(tree_processor_exception("UNKNOWN CATEGORY OF PTREE PROCESSOR "+procName));

    try
    {
        //WRITER could be also a READER - for preparing a FORM//#include <regex>
        if((procCategory & WRITER) !=0 )
        {
            std::cout<<"Tree processor '"<<procName<<"' is registering as WRITER... ";
            map_of_writers()[procName]=this;
        }
        if((procCategory & READER) !=0 )
        {
            std::cout<<"Tree processor '"<<procName<<"' is registering as READER... ";
            map_of_readers()[procName]=this;
        }
    }
    catch(...)
    {
        std::cout<<std::endl;
        std::cerr<<"EXCEPTION occured when registering '"<<procName<<"' processor."<<std::endl;
        std::cerr <<"tree_processor:"<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
    }

    std::cerr<<"  DONE"<<std::endl;
}

tree_processor::~tree_processor()
{
    if(procCategory & WRITER !=0 )
        map_of_writers().erase(procName);
    if(procCategory & READER !=0 )
        map_of_readers().erase(procName);
}

tree_processor& tree_processor::getReadProcessor (const char* name)//may throw
{
    std::cout<<name<<std::endl;
    tree_processor* tmp=map_of_readers()[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR '")+name+"' NOT FOUND!"));
    return *tmp;
}

tree_processor& tree_processor::getWriteProcessor(const char* name)//may throw
{
    tree_processor* tmp=map_of_writers()[name];
    if(tmp==nullptr)
        throw(tree_processor_exception(std::string("PTREE PROCESSOR '")+name+"' NOT FOUND!"));
    return *tmp;
}

//Do some work, call _implement_read, clean & return
void tree_processor::read_tree(ShmString& o,const pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory & READER == 0 )
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' IS NOT A READER"));
    try{
        _implement_read(o,top,request);
    }
    catch(...)
    {
        std::cerr <<procName<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        o+="\n EXCEPTION OCCURED:"+boost::current_exception_diagnostic_information();
    }
    o+=MEM_END;//"DONE" MARKER FOR PARAREL PROCESS
}

//Do some work, call _implement_write, clean & return
void tree_processor::write_tree(ShmString& o,pt::ptree& top,URLparser& request)//may throw
{
    if(procCategory & WRITER == 0 )
        throw(tree_processor_exception("PTREE PROCESSOR '"+procName+"' IS NOT A WRITER"));

    try{
        _implement_write(o,top,request);
    }
    catch(...)
    {
        std::cerr <<procName<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        o+="\n EXCEPTION OCCURED:"+boost::current_exception_diagnostic_information();
    }
    o+=MEM_END;//"DONE" MARKER FOR PARAREL PROCESS
}

//Replacing ${variable_name} with variables from request
std::string tree_processor::replace_all_variables(std::string template_version,URLparser& request)
{
    request.getFullPath();//generuje też "fullpath" w request'cie jak jej nie bylo
    request["proc"]=procName;//Tego nie ma w request, albo moze byc nieprawidlowe

    //MODYFIKACJE ZMIENNYMI Z REQUEST I ODDANIE
    return substitute_variables(template_version,request);//Już zmodyfikowana
}


}//namespace "fasada"
