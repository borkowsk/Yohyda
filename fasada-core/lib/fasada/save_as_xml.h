/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_saveAsXml_H
#define PROCESSOR_saveAsXml_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"
#include "saver_processor.h"

namespace fasada
{

class save_as_xml : public saver_processor //Category READER/&/WRITER
{
    public:
        save_as_xml(const char* name="saveAsXml");
       ~save_as_xml();
    protected://implementation
//    virtual
//        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_saveAsXml_H
