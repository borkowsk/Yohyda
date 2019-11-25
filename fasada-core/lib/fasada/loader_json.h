#ifndef PROCESSOR_JSON_H
#define PROCESSOR_JSON_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "fasada.hpp"
#include "tree_processor.h"

namespace fasada
{

class loader_json : public tree_processor
{
public:
    loader_json(const char* name="Json");
    ~loader_json(){}
protected://implementation
virtual
    void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request);
virtual
    void _implement_write(ShmString& o,pt::ptree& top,URLparser& request);
};

}
#endif // PROCESSOR_JSON_H
