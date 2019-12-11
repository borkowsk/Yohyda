/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_view_processor_H
#define PROCESSOR_view_processor_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"

namespace fasada
{

class view_processor : public tree_processor //Category READER
{
public:
    view_processor(const char* name);//"generic_viewer"
    ~view_processor();

protected://implementation
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request)=0;//Need reimplementation
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);//This has an error message only.
};

}//namespace "fasada"

#endif // PROCESSOR_view_processor_H
