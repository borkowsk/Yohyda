#ifndef PROCESSOR_FIND_H
#define PROCESSOR_FIND_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif
#include "tree_processor.h"


namespace fasada
{

class processor_find : public tree_processor //Category READER!
{
    public:
        processor_find(const char* name="find");
       ~processor_find();
    protected://implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}//namespace "fasada"

#endif // PROCESSOR_FIND_H
