/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
/// See CURRENT licence file!
///

#ifndef processor_info_H
#define processor_info_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"

namespace fasada
{

class processor_info : public tree_processor //Category READER!
{
public:
    processor_info(const char* name="info");
    ~processor_info();

protected://implementation
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // processor_info_H
