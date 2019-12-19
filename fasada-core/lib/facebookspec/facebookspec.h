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

namespace pt = boost::property_tree;

//Call all recoders needed for JSON from facebook
void call_recoders(pt::ptree& start,bool PL);

//Standard library initialisation for fasada
void register_processors();

}

#endif //FACEBOOKSPEC_H

