/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_LS_H
#define PROCESSOR_LS_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_processor.h"

namespace fasada
{

class processor_ls : public tree_processor //Category READER!
{
    bool longformat;
public:
    processor_ls(const char* name="ls");
    ~processor_ls();

protected://implementation
    virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
    virtual
    void _implement_action_panel(ShmString& o,URLparser& request);
    virtual
    void _implement_node_panel(ShmString& o,const pt::ptree& node,const std::string& fullpath,URLparser& request);
};

}//namespace "fasada"
#endif // PROCESSOR_LS_H
