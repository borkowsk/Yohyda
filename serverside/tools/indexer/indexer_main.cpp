// This is a directory indexer for user data downloaded from Facebook.
// (author: Wojciech Borkowski)
// base on: https://stackoverflow.com/questions/20923456/boost-directory-iterator-example-how-to-list-directory-files-not-recursive
// and: https://theboostcpplibraries.com/boost.propertytree
//
// Compilation:
// g++ -std=c++11 -Os -Wall -pedantic indexer_main.cpp -lboost_system -lboost_filesystem && ./a.out .
// 
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

void list_directory(const fs::path& p,pt::ptree& curr,unsigned plen)
{
    for(fs::directory_entry& entry : boost::make_iterator_range( fs::directory_iterator(p), {}))
    {
        if(is_directory(entry))
        {
            std::cout <<"#"<< entry.path() << "/\n";
            list_directory(entry.path(),curr,plen);
        }
        else
        if(is_symlink(entry))
            std::cout <<"#"<< entry << "-> SYMLINKS ARE NOT FOLLOWED!!!\n";
        else
        if(is_regular_file(entry))
        {
            if(entry.path().extension()==".json")
            {
                const char* lpath=(entry.path().c_str());
                lpath+=plen+1;
                std::cout <<"'"<< lpath <<"' is a JSON!\n";
                curr.put(pt::ptree::path_type{lpath, '/'},"!FacebookJson");
            }
            else
                if(entry.path().extension()==".js")
                {
                    const char* lpath=(entry.path().c_str());
                    lpath+=plen+1;
                    std::cout <<"'"<< lpath <<"' is a JSon!\n";
                    curr.put(pt::ptree::path_type{lpath, '/'},"!TwitterJson");
                }
                else
                    if(entry.path().extension()==".csv")
                    {
                        const char* lpath=(entry.path().c_str());
                        lpath+=plen+1;
                        std::cout <<"'"<< lpath <<"' is a CSV!\n";
                        curr.put(pt::ptree::path_type{lpath, '/'},"!LinkedInCsv");
                    }
            //std::cout << entry << " is a "<<entry.path().extension()<<"\n";
        }
        else
        if(is_other(entry))
            std::cout <<"#"<< entry << "?\n";
        else
            ;//std::cout <<"#"<< entry << "???\n";
    }
}


int main(int argc, char *argv[]) 
{
    pt::ptree top;

    fs::path p(argc>1? argv[1] : ".");
    unsigned plen=p.string().length();
    std::cout<<"Path is "<<p<<"["<<plen<<"]"<<std::endl;

    if(fs::is_directory(p))
    {
        std::cout << p << " is a directory containing:\n";
        list_directory(p,top,plen);
        pt::write_json("index.json",top);
        return 0;
    }
    else
    {
        std::cerr<< p << " is not a directory.\n";
        return 1;
    }
}

