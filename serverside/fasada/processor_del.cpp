#include "processor_del.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c

namespace fasada
{

processor_del::processor_del(const char* name):
    tree_processor(WRITER_READER,name) //also READER because is able to create its own FORM
{
}

processor_del::~processor_del()
{}

}//namespace "fasada"
