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
#include "save_as_csv.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

save_as_csv::save_as_csv(const char* name):
    saver_processor(name)
{

}

save_as_csv::~save_as_csv()
{}

void save_as_csv::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)    //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}


void save_as_csv::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)    //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"
