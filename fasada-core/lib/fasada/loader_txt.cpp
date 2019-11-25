#include "fasada.hpp"
#include "tree_processor.h"
#include "loader_txt.h"

namespace fasada
{

loader_txt::loader_txt(const char* name):
    tree_processor(WRITER,name) //also may be READER if it should create its own FORM
{

}

}
