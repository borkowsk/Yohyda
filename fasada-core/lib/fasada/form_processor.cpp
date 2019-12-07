/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

#include "fasada.hpp"
#include "form_processor.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

form_processor::form_processor(const char* name):
    tree_processor(WRITER_READER,name)
{

}

form_processor::~form_processor()
{}

void form_processor::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A READER!"));
}


void form_processor::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"
