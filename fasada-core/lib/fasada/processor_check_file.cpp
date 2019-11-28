/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

#include "processor_check_file.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#define BOOST_FILESYSTEM_NO_DEPRECATED
using namespace boost::filesystem;


namespace fasada
{

processor_check_file::processor_check_file(const char* name):
    tree_processor(WRITER_READER,name)
{

}

processor_check_file::~processor_check_file()
{}

static bool find_file( const path & dir_path,  // in this directory,
                const std::string & file_name, // search for this name,
                path & path_found )            // placing path here if found
/// https://stackoverflow.com/questions/9366040/search-files-in-directory-and-subdirectory-using-boost-library-c
{
  if ( !exists( dir_path ) ) return false;
  directory_iterator end_itr; // default construction yields past-the-end
  for ( directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr )
  {
    if ( is_directory(itr->status()) )
    {
      if ( find_file( itr->path(), file_name, path_found ) ) return true;
    }
    else if ( itr->path().filename() == file_name ) // see below
    {
      path_found = itr->path();
      return true;
    }
  }
  return false;
}

void processor_check_file::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    o+="\nChecking file path in "+request["&path"]+"\n";//Nagłówek

    unsigned    noc=top.size();//czy ma jakieś elementy składowe? Wtedy chryja ;-)
    if(noc!=0)
    {
        //o+="Only leaf type nodes can be modified by "+procName+"\n";
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT CHECK NOT-LEAF NODE!"));
    }

    std::string private_directory=request["&private_directory"];

    o+="\ndata: "+top.data()
     +"\nprivate_directory: "+private_directory;
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
        std::string file=private_directory;
        file+=link+"/"+top.data();
        o+=file+" ... ";

        if ( !boost::filesystem::exists( file ) ) o+= "Can't find the file!\n";
        else { o+= "EXIST!\n"; request["file_found"]=link+"/"+top.data();return;}

    }while(1);//Albo znajdzie albo skończą się '/'

    //Maybe from the top of private directory?
    link=private_directory+"/"+top.data();
    o+=link+" ... ";if ( !boost::filesystem::exists( link ) ) o+= "Can't find the file!\n";
    else { o+= "EXIST!\n"; request["file_found"]="/"+top.data();return;}

    //Searching directory tree below "private"
    path myfound = "/";
    auto lastSlash=top.data().rfind('/');
    std::string filename=top.data().substr(lastSlash+1);
    o+="SEARCHING '"+filename+"' IN '"+private_directory+"'\n";
    if(find_file(private_directory,filename,myfound))
    {
        std::string nameOfFound=myfound.c_str();
        o+="FOUND! '"+nameOfFound+"'\n";
        nameOfFound=nameOfFound.substr(private_directory.length());
        request["file_found"]=nameOfFound;
        return;
    }
    else o+= "Can't find the file!\n";
}

void processor_check_file::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    _implement_read(o,top,request);

    if(request.find("file_found")!=request.end() )
    {
            o+="\n RESULT: '"+ request["file_found"] + "'\n\n";

            //Setting new path if found before
            auto orig=top.data()+" ";//SPACE intentionaly. It blocs the next level CHECK.
            top.data()="";
            top.add("local_uri",request["file_found"]);
            top.add("orig",orig);
    }
}

}//namespace "fasada"
