/// This file is a part of Facebook handling library for Fasada 
/// ------------------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
///
/// See licence file!
///
// ./serverside/facebookspec/facebookspec.cpp
// ./serverside/facebookspec/facebookspec.h
// ./serverside/facebookspec/processor_facebookjson.cpp
// ./serverside/facebookspec/processor_facebookjson.h

#ifndef PROCESSOR_FACEBOOKJSON_H
#define PROCESSOR_FACEBOOKJSON_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "loader_processor.h"

using namespace fasada;

namespace facebook
{

class loader_facebookJson : public loader_processor //WRITER
{
public:
    loader_facebookJson(const char* name="FacebookJson");
    ~loader_facebookJson();

protected://implementation
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

} //namespace "facebook"

#endif // PROCESSOR_FACEBOOKJSON_H
