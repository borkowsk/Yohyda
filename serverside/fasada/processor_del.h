/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_DEL_H
#define PROCESSOR_DEL_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#include "tree_processor.h"

namespace fasada
{

class processor_del : public tree_processor //READER&WRITER
{
    static std::string Form;//default HTML form for this processor
public:
    processor_del(const char* name="del");
    ~processor_del();
 protected://implementation
 virtual
     void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
 virtual
     void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

} //namespace "fasada"

#endif // PROCESSOR_DEL_H
