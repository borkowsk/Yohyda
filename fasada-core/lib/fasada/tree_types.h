/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef TREE_TYPES_H
#define TREE_TYPES_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <map>
//#include <string_view> //string_view? https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map

//Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
//This allocator will allow placing containers in the segment
//typedef allocator<int,  managed_shared_memory::segment_manager>         ShmIntAllocator;
//...
//From: https://www.boost.org/doc/libs/1_38_0/doc/html/interprocess/allocators_containers.html
namespace fasada
{
    namespace ipc = boost::interprocess;

#if(0)
    using  ShmCharAllocator   = ipc::allocator<char, ipc::managed_shared_memory::segment_manager>;
    using  ShmString          = ipc::basic_string<char, std::char_traits<char>,ShmCharAllocator>;
    using  ShmStringAllocator = ipc::allocator<ShmString, ipc::managed_shared_memory::segment_manager>;
    using  ShmVectorOfString  = ipc::vector<ShmString, ShmStringAllocator>;
    using  key_string=std::string;//string_view? https://stackoverflow.com/questions/50608392/using-const-char-as-key-for-map-unordered-map
    using  val_string=std::string;
    using  processors_map=std::map<key_string,tree_processor*> ;    //Type of processors maps - this type may change!
#else
    typedef  ipc::allocator<char, ipc::managed_shared_memory::segment_manager>          ShmCharAllocator;
    typedef  ipc::basic_string<char, std::char_traits<char>,ShmCharAllocator>           ShmString;
    typedef  ipc::allocator<ShmString, ipc::managed_shared_memory::segment_manager>     ShmStringAllocator;
    typedef  ipc::vector<ShmString, ShmStringAllocator>                                 ShmVectorOfString;
    typedef  std::string                                                                key_string;
    typedef  std::string                                                                val_string;

    class    tree_processor;//Type processors_map may change!!!
    typedef  std::map<key_string,tree_processor*>                                       processors_map;
#define nullptr (NULL)
#endif

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

}//namespace "fasada"
#endif // TREE_TYPES_H
