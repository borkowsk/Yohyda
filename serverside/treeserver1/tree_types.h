#ifndef TREE_TYPES_H
#define TREE_TYPES_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>

//Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
//This allocator will allow placing containers in the segment
//typedef allocator<int,  managed_shared_memory::segment_manager>         ShmIntAllocator;
//...
//From: https://www.boost.org/doc/libs/1_38_0/doc/html/interprocess/allocators_containers.html
namespace facjata
{
    namespace ipc = boost::interprocess;

    using ShmCharAllocator   = ipc::allocator<char, ipc::managed_shared_memory::segment_manager>    ;
    using ShmString          = ipc::basic_string<char, std::char_traits<char>,ShmCharAllocator>;
    using ShmStringAllocator = ipc::allocator<ShmString, ipc::managed_shared_memory::segment_manager>;
    using ShmVectorOfString  = ipc::vector<ShmString, ShmStringAllocator>;


    inline
    ShmString& operator += (ShmString& t,const std::string& s)
    {
        return t+=s.c_str();
    }

    inline
    ShmString& operator += (ShmString& t,const ipc::string& s)
    {
        return t+=s.c_str();
    }

}//namespace facjata
#endif // TREE_TYPES_H
