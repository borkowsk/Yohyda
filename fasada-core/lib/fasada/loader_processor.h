/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_generic_loader_H
#define PROCESSOR_generic_loader_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "form_processor.h"

namespace fasada
{

class loader_processor : public form_processor //Category READER_WRITER as all form processors
{
protected:
    static std::string Form;//default HTML form TEMPLATE for this processor
    virtual
    const std::string& _get_form_template(){return Form;}//MUST BE REIMPLEMENTED!

public:
    loader_processor(const char* name="generic_loader");
    ~loader_processor();

protected://implementation
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request)=0;//Loaders have to reimplement this!
    virtual
    void _implement_magic_file_test(ShmString& o,pt::ptree& top,std::string Path,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_generic_loader_H
