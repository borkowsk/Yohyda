#include "fasada.hpp"
#include "tree_processor.h"
#include "loader_json.h"

namespace fasada
{

loader_json::loader_json(const char* name):
        tree_processor(WRITER,name) //also may be READER if it should create its own FORM
{

}

}
