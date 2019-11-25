#ifndef PROCESSOR_TXT_H
#define PROCESSOR_TXT_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "fasada.hpp"
#include "tree_processor.h"

namespace fasada
{

class loader_txt : public tree_processor
{
public:
    loader_txt(const char* name="txt");
   ~loader_txt(){}
};

}
#endif // PROCESSOR_TXT_H
