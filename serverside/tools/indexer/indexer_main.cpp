//
// directory indexer
// base on: https://stackoverflow.com/questions/20923456/boost-directory-iterator-example-how-to-list-directory-files-not-recursive
//
// g++ -std=c++11 -Os -Wall -pedantic indexer_main.cpp -lboost_system -lboost_filesystem && ./a.out .
// 
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

void list_directory(const fs::path& p,pt::ptree& curr)
{
    for(fs::directory_entry& entry : boost::make_iterator_range( fs::directory_iterator(p), {}))
    {
        if(is_directory(entry))
        {
            std::cout <<"#"<< entry.path() << "/\n";
            list_directory(entry.path(),curr);
        }
        else
        if(is_symlink(entry))
            std::cout <<"#"<< entry << "-> SYMLINKS ARE NOT FOLLOWED!!!\n";
        else
        if(is_regular_file(entry))
        {
            if(entry.path().extension()==".json")
            {
                std::cout << entry << "  is a JSON!\n";
                curr.put(pt::ptree::path_type{entry.path().c_str(), '/'},"!FacebookJson");
                //pt::ptree& c=curr.add_child(entry.path().c_str(),"");
            }
            //std::cout << entry << " is a "<<entry.path().extension()<<"\n";
        }
        else
        if(is_other(entry))
            std::cout <<"#"<< entry << "?\n";
        else
            std::cout <<"#"<< entry << "???\n";
    }
}


int main(int argc, char *argv[]) 
{
    pt::ptree top;

    fs::path p(argc>1? argv[1] : ".");

    if(fs::is_directory(p))
    {
        std::cout << p << " is a directory containing:\n";
        list_directory(p,top);
        pt::write_json("index.json",top);
        return 0;
    }
    else
    {
        std::cout << p << " is not a directory.\n";
        return 1;
    }
}

