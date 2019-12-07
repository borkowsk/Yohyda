/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_BASE_OF_FORM_PROCESSORS_H
#define PROCESSOR_BASE_OF_FORM_PROCESSORS_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"
#include "tree_processor.h"

namespace fasada
{

class form_processor : public tree_processor //Category READER/^/&/WRITER ?
{
    public:
        form_processor(const char* name="base_of_form_processor");
       ~form_processor();
    protected://Subclass specific
    virtual
         const std::string& _get_form_template()=0;//MUST BE REIMPLEMENTED!
    protected://implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_BASE_OF_FORM_PROCESSOR_H
