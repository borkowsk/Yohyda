//
// directory indexer
// base on: https://stackoverflow.com/questions/20923456/boost-directory-iterator-example-how-to-list-directory-files-not-recursive
//
// g++ -std=c++11 -Os -Wall -pedantic indexer_main.cpp -lboost_system -lboost_filesystem && ./a.out .
// 
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>

using namespace boost::filesystem;

void list_directory(const path& p)
{
    for(boost::filesystem::directory_entry& entry : boost::make_iterator_range( directory_iterator(p), {}))
    {
        if(is_directory(entry))
        {
            std::cout <<"#"<< entry.path() << "/\n";
            list_directory(entry.path());
        }
        else
        if(is_symlink(entry))
            std::cout <<"#"<< entry << "-> SYMLINKS ARE NOT FOLLOWED!!!\n";
        else
        if(is_regular_file(entry))
        {
            if(entry.path().extension()==".json")
                std::cout << entry << "  is a JSON!\n";
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
    path p(argc>1? argv[1] : ".");

    if(is_directory(p)) 
    {
        std::cout << p << " is a directory containing:\n";
        list_directory(p);
        return 0;
    }
    else
    {
        std::cout << p << " is not a directory.\n";
        return 1;
    }
}

