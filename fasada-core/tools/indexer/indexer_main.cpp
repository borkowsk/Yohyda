/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

// This is a directory indexer for user data downloaded from Facebook.
// (author: Wojciech Borkowski)
// base on: https://stackoverflow.com/questions/20923456/boost-directory-iterator-example-how-to-list-directory-files-not-recursive
// and: https://theboostcpplibraries.com/boost.propertytree
//
// Compilation and testing
// g++ -std=c++11 -Os -Wall -pedantic indexer_main.cpp [-libmagic] -lboost_system -lboost_filesystem && ./a.out ./ [--all]
// 
// What about using libmagic ? See: https://github.com/file/file , https://gist.github.com/vivithemage/9489378
//
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <cstring>

#ifdef USE_MAGIC
#include <magic.h>
class magicFileTester
//Restricted wraper for libmagic
{
    magic_t magic_cookie;
public:
    magicFileTester()
    {
        /* MAGIC_MIME tells magic to return a mime of the file, 
           but you can specify different things	*/

        magic_cookie = magic_open(MAGIC_MIME);
	
        if (magic_cookie == NULL) 
        {
                std::cerr << "\nUnable to initialize magic library" << std::endl;
                exit( 1 );
        }
	
        if (magic_load(magic_cookie, NULL) != 0) 
        {
                std::cerr << "\ncCannot load magic database - " << magic_error(magic_cookie) << std::endl;
                magic_close(magic_cookie);
                exit( 2 );
        }
    }

    ~magicFileTester()
    {
        magic_close(magic_cookie); 
    }

    const char* getFileType(const std::string& p)
    {
        return magic_file(magic_cookie, p.c_str() );
    }

} file_tester;
#endif

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

bool isHiddenOrDots(const fs::path &p) 
//https://stackoverflow.com/questions/12735634/how-do-i-ignore-hidden-files-and-files-in-hidden-directories-with-boost-filesy
{
    std::string name = p.filename().string();
    if(name == ".." ||
       name == "."  ||
       name.at(0) == '.')
    {
       return true;
    }

    return false;
}

class StrTempVal
{
    std::string& Var;
    std::string  RVal;
public:
    StrTempVal(std::string& iVar,std::string iVal):Var(iVar),RVal(Var){Var=iVal;}
    ~StrTempVal(){Var=RVal;}
};

bool allFiles=false;

void list_directory(const fs::path& p,pt::ptree& curr,unsigned plen)
{
    static std::string ArchiveSource="";
    try
    {
            unsigned counter=0;
            auto iterator = boost::make_iterator_range( fs::directory_iterator(p), {});

            for(fs::directory_entry& entry : iterator )
            {
                counter++;
                if(is_directory(entry) && !isHiddenOrDots(entry.path())  ) //Ale nie ukryte ani specjalne!?
                {
                    std::cout <<"#"<< entry.path() <<std::endl;

                    if( strncasecmp( entry.path().filename().c_str(),"facebook",8)==0)
                    {
                        StrTempVal Tmp(ArchiveSource,"Facebook");
                        list_directory(entry.path(),curr,plen);//REKURENCJA Z USTAWIONYM ArchiveSource
                    }
                    else
                    if( strncasecmp( entry.path().filename().c_str(),"twitter",7)==0)
                    {
                        StrTempVal Tmp(ArchiveSource,"Twitter");
                        list_directory(entry.path(),curr,plen);//REKURENCJA Z USTAWIONYM ArchiveSource
                    }
                    else
                    {
                        list_directory(entry.path(),curr,plen);//REKURENCJA!!!
                    }

                }
                else
                if(is_symlink(entry))
                {
                    std::cout <<"#"<< entry << "-> SYMLINKS ARE NOT FOLLOWED!!!"<<std::endl;
                }
                else
                if(is_regular_file(entry))
                {
                    if(entry.path().extension()==".json")
                    {
                        const char* lpath=(entry.path().c_str());

                        lpath+=plen+1;
                        std::cout <<"'"<< lpath <<"' is considered as a "<<ArchiveSource<<" JSON!";

                        curr.put(pt::ptree::path_type{lpath, '/'},"!"+ArchiveSource+"Json");
#                       ifdef USE_MAGIC
                        auto theType=file_tester.getFileType( entry.path().string() );
                        std::cout <<" MIME: "<<theType;
                        curr.put( pt::ptree::path_type{lpath + std::string(".mime"), '/'}, theType );
#                       endif
                        std::cout<<std::endl;
                    }
                    else
                        if(entry.path().extension()==".js") //Maybe JavaScript
                        {
                            const char* lpath=(entry.path().c_str());
                            lpath+=plen+1;
                            
                            if(ArchiveSource.at(0)=='T' && ArchiveSource=="Twitter" )
                            {
                              std::cout <<"'"<< lpath <<"' is considered as a Twitter JSon!";
                              curr.put(pt::ptree::path_type{lpath, '/'},"!TwitterJson");
                            }
                            else
                            {
                              std::cout <<"'"<< lpath <<"' is considered as JavaScript code!";
                              curr.put(pt::ptree::path_type{lpath, '/'},"!JavaScript");
                            }

#                           ifdef USE_MAGIC
                            auto theType=file_tester.getFileType( entry.path().string() );
                            std::cout <<" MIME: "<<theType;
                            curr.put( pt::ptree::path_type{lpath + std::string(".mime"), '/'}, theType );
#                           endif
                            std::cout<<std::endl;                            
                        }
                        else
                            if(entry.path().extension()==".xml")
                            {
                                const char* lpath=(entry.path().c_str());
                                lpath+=plen+1;
                                std::cout <<"'"<< lpath <<"' is a XML!";

                                curr.put(pt::ptree::path_type{lpath, '/'},"!Xml");

#                               ifdef USE_MAGIC
                                auto theType=file_tester.getFileType( entry.path().string() );
                                std::cout <<" MIME: "<<theType;
                                curr.put( pt::ptree::path_type{lpath + std::string(".mime"), '/'}, theType );
#                               endif
                                std::cout<<std::endl;      
                            }
                            else
                            if(entry.path().extension()==".csv")
                            {
                                const char* lpath=(entry.path().c_str());
                                lpath+=plen+1;
                                std::cout <<"'"<< lpath <<"' is a CSV!";

                                curr.put(pt::ptree::path_type{lpath, '/'},"!Csv");

#                               ifdef USE_MAGIC
                                auto theType=file_tester.getFileType( entry.path().string() );
                                std::cout <<" MIME: "<<theType;
                                curr.put( pt::ptree::path_type{lpath + std::string(".mime"), '/'}, theType );
#                               endif
                                std::cout<<std::endl; 
                            }
                            else
                                if(entry.path().extension()==".txt")
                                {
                                    const char* lpath=(entry.path().c_str());
                                    lpath+=plen+1;
                                    std::cout <<"'"<< lpath <<"' is a TXT!";

                                    curr.put(pt::ptree::path_type{lpath, '/'},"!Txt");

#                                   ifdef USE_MAGIC
                                    auto theType=file_tester.getFileType( entry.path().string() );
                                    std::cout <<" MIME: "<<theType;
                                    curr.put( pt::ptree::path_type{lpath + std::string(".mime"), '/'}, theType );
#                                   endif
                                    std::cout<<std::endl; 
                                }
                                else
                                    if(allFiles)
                                    {
                                       std::cout<<entry.path()<<":"<<std::endl;
                                       std::string fpath=entry.path().parent_path().string()
                                               +"/files/_" //XML does not accepted numbers as names of TAGs
                                               +boost::lexical_cast<std::string>(counter);

                                       const char* lpath=fpath.c_str();
                                       lpath+=plen+1;

                                       std::cout <<"'"<< lpath << "' is a file, type '"<<entry.path().extension()
                                                 <<"', in directory "<<p.string()<<" ; ";

                                       curr.add(pt::ptree::path_type
                                                { lpath , '/'},
                                                entry.path().filename().string());

#                                      ifdef USE_MAGIC
                                       auto theType=file_tester.getFileType( entry.path().string() );
                                       std::cout <<" MIME: "<<theType;
                                       curr.put( pt::ptree::path_type{lpath + std::string(".mime"), '/'}, theType );
#                                      endif
                                       std::cout<<std::endl; 
                                    }

                }
                else
                if(is_other(entry))
                {
                    std::cout <<"#"<< entry << "?\n";
                }
                else
                    ;//std::cout <<"#"<< entry << "???\n";
            }
    }
    catch (const fs::filesystem_error& ex)
    {
            std::cerr << ex.what() << " ; Index may be incomplete!" <<std::endl;
    }
}


int main(int argc, char *argv[]) 
{
    pt::ptree top;
    bool flaga=false;
    std::cout<<"DIRECTORY INDEXER FOR \"fasada\" "
               "(see: http://sites.google.com/view/fasada-cpp/ )"<<std::endl;
    //std::cout<<"Number of parameters: "<<argc<<std::endl;

    fs::path p( argc >1 ? argv[1] : ".");
    unsigned plen=p.string().length();
    if(p.string().rfind('/')==plen-1)
                plen--;

    std::cout<<"Path is "<<p<<"["<<plen<<"]"<<std::endl;

    if( argc > 2 && strcmp(argv[2],"--all")==0 )
        allFiles=true;

    if(fs::is_directory(p))
    {
        std::cout << p << " is a directory containing:\n";
        try
        {
            list_directory(p,top,plen);
        }
        catch (const fs::filesystem_error& ex)
        {
            flaga=true;
            std::cerr << ex.what() <<std::endl;
        }
        catch (const pt::ptree_error& ex)
        {
            flaga=true;
            std::cerr << ex.what() <<std::endl;
        }
        catch (const std::exception& ex)
        {
            flaga=true;
            std::cerr << ex.what() <<std::endl;
        }
        catch ( ... )
        {
            flaga=true;
            std::cerr << "Very unexpected error!" <<std::endl;;
        }
        if(flaga)
            std::cerr << "Because of serious error index.json not saved.\a\b\a\b" <<std::endl;
        else
            pt::write_json("index.json",top);

        return 0;
    }
    else
    {
        std::cerr<< p << " is not a directory.\n";
        return 1;
    }
}

