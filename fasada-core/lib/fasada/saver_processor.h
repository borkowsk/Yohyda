/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_saver_processor_H
#define PROCESSOR_saver_processor_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"

namespace fasada
{

class saver_processor : public tree_processor //Category READER/&/WRITER
{
    public:
        saver_processor(const char* name="generic_saver");
       ~saver_processor();
    protected://implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_saver_processor_H
