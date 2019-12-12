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
#include "save_as_txt.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

save_as_txt::save_as_txt(const char* name):
    saver_processor(name)
{}

save_as_txt::~save_as_txt()
{}

void save_as_txt::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)    //TODO
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED YET!"));
}

}//namespace "fasada"
