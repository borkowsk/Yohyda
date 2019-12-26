/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_FIND_H
#define PROCESSOR_FIND_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "form_processor.h"

namespace fasada
{

class processor_find : public form_processor //Really it is a READER, but only !actions can be used for <FORM>
{
protected:
    static std::string Form;//default HTML form TEMPLATE for this processor
    virtual
    const std::string& _get_form_template(){return Form;}//MUST BE REIMPLEMENTED!

public:
    processor_find(const char* name="find");
    ~processor_find();

protected:
    //"Must be" implementations
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);

    //Two alternative implementation for _implememt_write()
    virtual
    void _implement_substring_find(ShmString& o,const pt::ptree& top,URLparser& request);//Called in _implement_write

    virtual
    void _implement_regex_find(ShmString& o,const pt::ptree& top,URLparser& request);//Called in _implement_write

};

}//namespace "fasada"

#endif // PROCESSOR_FIND_H
