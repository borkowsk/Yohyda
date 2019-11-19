/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_SET_H
#define PROCESSOR_SET_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"

namespace fasada
{

class processor_set : public tree_processor //READER&WRITER
{
protected:
    static std::string Form;//default HTML form for this processor

public:
    processor_set(const char* name="set");
   ~processor_set();
 protected://implementation
 virtual
     void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
 virtual
     void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

} //namespace "fasada"

#endif // PROCESSOR_SET_H
