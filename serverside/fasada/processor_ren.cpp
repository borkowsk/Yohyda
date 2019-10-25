#include "processor_ren.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

namespace fasada
{

processor_ren::processor_ren(const char* name):
    tree_processor(WRITER_READER,name) //also READER because is able to create its own FORM
{
}

processor_ren::~processor_ren()
{}

}//namespace "fasada"
