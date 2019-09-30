#include "tree_types.h"
#include "tree_consts.h"
#include "processor_ls.h"
#include <iostream>

namespace fasada
{

processor_ls::processor_ls(const char* name):
    tree_processor(READER,name)
{

}

processor_ls::~processor_ls()
{}

//TODO - problem indeksowania tablic JSONów w ptree - kłopotliwy bardzo
//https://stackoverflow.com/questions/48407925/boostproperty-treeptree-accessing-arrays-first-complex-element
//Do rozwiązania na poziomie serwera fasady za pomoca wstawienia kolejnych liczb
void processor_ls::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    bool longformat=(request.find("long")!=request.end()?true:false);

    o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    for(auto p:top)
    {
        //std::cerr<<p.first.data()<<":"<<p.second.data()<<std::endl;
        o+=std::string(p.first.data())+std::string(":")+std::string(p.second.data());
        if(longformat)
            o+=";\n";
        else
            o+=";\t";
    }

    //o+="\nTheEnd";
}


void processor_ls::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
