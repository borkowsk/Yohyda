/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///
#define UNIT_IDENTIFIER "run_on_tree"
#include "fasada.hpp"
#include "run_on_tree.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

run_on_tree::run_on_tree(const char* name):
    tree_processor(WRITER_READER/*READER? WRITER?*/,name)
{

}

run_on_tree::~run_on_tree()
{}

void run_on_tree::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)     //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED YET!"));
}

void run_on_tree::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)     //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"
