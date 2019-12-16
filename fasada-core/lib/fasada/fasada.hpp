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

/*
// constexpr FUNCTIONS for C++ preprocessor
//.//////////////////////////////////////////////////////////
//
/// compares two strings in compile time constant fashion
/// https://stackoverflow.com/questions/2335888/how-to-compare-strings-in-c-conditional-preprocessor-directives
// some compilers may require ((int)lhs[0] - (int)rhs[0])
constexpr int c_strcmp( char const* lhs, char const* rhs )
{
    return (('\0' == lhs[0]) && ('\0' == rhs[0])) ? 0
        :  (lhs[0] != rhs[0]) ? (lhs[0] - rhs[0])
        : c_strcmp( lhs+1, rhs+1 );
}

/// compares double for preprocesor 
constexpr int p_greater( double _1, double _2 )
{
    return (_1 >_2 ? 1: 0);
}
*/

#endif // FASADA_HPP
