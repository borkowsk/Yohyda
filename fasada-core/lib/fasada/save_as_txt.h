/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_saveAsTxt_H
#define PROCESSOR_saveAsTxt_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"
#include "saver_processor.h"

namespace fasada
{

class save_as_txt : public saver_processor //Category READER/&/WRITER
{
    public:
        save_as_txt(const char* name="saveAsTxt");
       ~save_as_txt();
    protected://implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_saveAsTxt_H
