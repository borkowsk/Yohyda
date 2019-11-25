#include "fasada.hpp"
#include "tree_processor.h"
#include "loader_csv.h"

namespace fasada
{

loader_csv::loader_csv(const char* name):
    tree_processor(WRITER,name) //also may be READER if it should create its own FORM
{

}

}
