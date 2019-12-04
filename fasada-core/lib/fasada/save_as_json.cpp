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
#include "save_as_json.h"
#include "saver_processor.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

save_as_json::save_as_json(const char* name):
    saver_processor(name)
{

}

save_as_json::~save_as_json()
{}

void save_as_json::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)    //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}


void save_as_json::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)    //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"
