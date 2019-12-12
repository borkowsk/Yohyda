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

#include "form_processor.h"

namespace fasada
{

class saver_processor : public form_processor //Category READER_WRITER
{
protected:
    static std::string Form;//default HTML form TEMPLATE for this processor
    virtual
    const std::string& _get_form_template(){return Form;}//MUST BE REIMPLEMENTED!

public:
    saver_processor(const char* name="generic_saver");
    ~saver_processor();

protected://implementation
    virtual
    std::string _make_path(const std::string& ext,URLparser& request);
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request)=0;//SAVERS MUST REIMPLEMENT THIS!
};

}//namespace "fasada"

#endif // PROCESSOR_saver_processor_H
