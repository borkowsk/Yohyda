/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_CHECK_FILE_H
#define PROCESSOR_CHECK_FILE_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#include "tree_processor.h"


namespace fasada
{

class processor_check_file : public tree_processor //Category READER&WRITER!
{
public:
    processor_check_file(const char* name="checkFile");
    ~processor_check_file();

protected://implementation
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_CHECK_FILE_H
