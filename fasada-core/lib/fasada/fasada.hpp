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

#include "tree_types.h"
#include "tree_consts.h"
#include "tree_processor.h"

namespace fasada
{

void init(bool WithWriting);//INITIALIZE THE FASADA LIBRARY
void register_processors(bool WithWriters); //Function for registering default set of tree_processors
                            //located in fasada.a/lib/so
bool writing_enabled();     //Is writing to tree allowed?

}//namespace "fasada"

#endif // FASADA_HPP
