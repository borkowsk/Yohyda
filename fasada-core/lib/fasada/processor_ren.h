/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_REN_H
#define PROCESSOR_REN_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "form_processor.h"

namespace fasada
{

class processor_ren : public form_processor //READER&WRITER
{
    static std::string Form;//default HTML form TEMPLATE for this processor
    virtual
    const std::string& _get_form_template(){return Form;}//MUST BE REIMPLEMENTED!

public:
    processor_ren(const char* name="ren");
    ~processor_ren();

protected://implementation
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

} //namespace "fasada"

#endif // PROCESSOR_REN_H
