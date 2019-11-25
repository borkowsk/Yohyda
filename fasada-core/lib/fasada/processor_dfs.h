/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef PROCESSOR_DFS_H
#define PROCESSOR_DFS_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#include "tree_processor.h"


namespace fasada
{

class processor_dfs : public tree_processor //Category READER!
{
    public:
        processor_dfs(const char* name="dfs");
       ~processor_dfs();
    protected://implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
    virtual
        void _implement_action_panel(ShmString& o,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_DFS_H
