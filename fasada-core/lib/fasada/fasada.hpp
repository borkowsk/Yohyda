/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef FASADA_HPP
#define FASADA_HPP

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#define FASADA_VERSION (0.01)
#define FASADA_VERSION_STR "0.01"

#include "tree_types.h"
#include "tree_consts.h"
#include "tree_processor.h"

namespace fasada
{
extern const float _version;//=FASADA_VERSION;
extern const char* _version_str;//=FASADA_VERSION_STR;
static const char* _compiled="compiled " __DATE__ ", " __TIME__ ";";//Diferent for any compilation unit

void init(bool WithWriting);//INITIALIZE THE FASADA LIBRARY
void register_processors(bool WithWriters); //Function for registering default set of tree_processors
                            //located in fasada.a/lib/so
bool writing_enabled();     //Is writing to tree allowed?

}//namespace "fasada"

#endif // FASADA_HPP
