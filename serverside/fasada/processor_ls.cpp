#include "tree_types.h"
#include "processor_ls.h"
#include <iostream>

namespace fasada
{

static processor_ls LS;//Istnieje conajmniej jedna taka zmienna. I wystarczy, chyba że ktoś chce robić aliasy

processor_ls::processor_ls(const char* name):
    tree_processor(READER,name)
{

}

processor_ls::~processor_ls()
{}
//TODO - problem indeksowania tablic JSONów w ptree - kłopotliwy bardzo
//https://stackoverflow.com/questions/48407925/boostproperty-treeptree-accessing-arrays-first-complex-element
void processor_ls::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    bool longformat=(request.find("long")!=request.end()?true:false);
    o+="ls:\n";
    for(auto p:top)
    {
        std::cerr<<p.first.data()<<":"<<p.second.data()<<std::endl;
        o+=std::string(p.first.data())+std::string(":")+std::string(p.second.data());
        if(longformat)
            o+=";\n";
        else
            o+=";\t";
    }
    o+="\nOK";
}


void processor_ls::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"