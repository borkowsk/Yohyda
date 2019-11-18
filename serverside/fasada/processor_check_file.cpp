#include "processor_check_file.h"
#include <boost/filesystem.hpp>

namespace fasada
{

processor_check_file::processor_check_file(const char* name):
    tree_processor(WRITER_READER,name)
{

}

processor_check_file::~processor_check_file()
{}


void processor_check_file::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    o+="\nChecking file path in "+request["&path"]+"\n";//Nagłówek

    unsigned    noc=top.size();//czy ma jakieś elementy składowe? Wtedy chryja ;-)
    if(noc!=0)
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT CHECK NOT-LEAF NODE!"));
    }

    o+="\ndata: "+top.data()
     +"\nprivate_directory: "+request["&private_directory"]
     +"\npublic_directory: "+request["&public_directory"]
     +"\npath: "+request["&path"]
     +"\n\n";

    std::string link;

    //First try - as full path
    link=request["&private_directory"]+request["&path"]+"/"+top.data();
    o+=link+" ... "; if ( !boost::filesystem::exists( link ) ) o+= "Can't find the file!\n";
    else {o+= "EXIST!\n"; request["file_found"]="./"+top.data();return;}

    //Trying to cut the fullpath
    link=request["&path"];
    do{
        auto lastSlash=link.rfind('/');

        if(lastSlash==link.npos) break;

        link=link.substr(0,lastSlash);
        std::string file=request["&private_directory"];
        file+=link+"/"+top.data();
        o+=file+" ... ";

        if ( !boost::filesystem::exists( file ) ) o+= "Can't find the file!\n";
        else { o+= "EXIST!\n"; request["file_found"]=link+"/"+top.data();return;}

    }while(1);//Albo znajdzie albo skończą się '/'

    //Maybe from the top of private directory?
    link=request["&private_directory"]+"/"+top.data();
    o+=link+" ... ";if ( !boost::filesystem::exists( link ) ) o+= "Can't find the file!\n";
    else { o+= "EXIST!\n"; request["file_found"]="/"+top.data();return;}

    //Searching directory tree below "private"
    //...

}

void processor_check_file::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    _implement_read(o,top,request);

    o+="\n RESULT: '"+ request["file_found"] + "'\n\n";

    //Setting new path if found before
    auto orig=top.data()+" ";//SPACE intentionaly. It blocs the next level CHECK.
    top.data()="";
    top.add("local_uri",request["file_found"]);
    top.add("orig",orig);
}

}//namespace "fasada"
