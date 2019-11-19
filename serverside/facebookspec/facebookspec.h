/// This file is a part of Facebook handling library for Fasada 
/// ------------------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
///
/// See licence file!
///
// ./serverside/facebookspec/facebookspec.cpp
// ./serverside/facebookspec/facebookspec.h
// ./serverside/facebookspec/processor_facebookjson.cpp
// ./serverside/facebookspec/processor_facebookjson.h

#ifndef FACEBOOKSPEC_H
#define FACEBOOKSPEC_H

#include "fasada.hpp"
#include <boost/property_tree/ptree.hpp>

namespace facebook
{

void call_recoders(pt::ptree& start,bool PL);
void register_processors();

}

#endif //FACEBOOKSPEC_H

