/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_saveAsJson_H
#define PROCESSOR_saveAsJson_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"
#include "saver_processor.h"

namespace fasada
{

class save_as_json : public saver_processor //Category READER/&/WRITER
{
    public:
        save_as_json(const char* name="saveAsJson");
       ~save_as_json();
    protected://implementation
//    virtual
//        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_saveAsJson_H
