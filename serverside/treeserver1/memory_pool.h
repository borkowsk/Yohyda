#ifndef FACJATA_MEMORY_POOL
#define FACJATA_MEMORY_POOL (1)

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <cassert>

using namespace boost::interprocess;

//Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
//This allocator will allow placing containers in the segment
//typedef allocator<int,  managed_shared_memory::segment_manager>         ShmIntAllocator;
//...
//From: https://www.boost.org/doc/libs/1_38_0/doc/html/interprocess/allocators_containers.html
typedef allocator<char, managed_shared_memory::segment_manager>          ShmCharAllocator;
typedef basic_string<char, std::char_traits<char>,ShmCharAllocator>      ShmString;
typedef allocator<ShmString, managed_shared_memory::segment_manager>     ShmStringAllocator;  
typedef vector<ShmString, ShmStringAllocator>                            ShmVectorOfString;

namespace facjata {

const char*     MEM_POOL_NAME="FacjataMemoryPool";
const size_t    MEM_POOL_SIZE=0xffff;

class MemoryPool
{
    bool I_am_tree_server;
    managed_shared_memory segment; 
public:
    MemoryPool(bool TreeServerSide):
                I_am_tree_server((shared_memory_object::remove(MEM_POOL_NAME),true)),
                segment(create_only,MEM_POOL_NAME,MEM_POOL_SIZE)  
    {    
         static int mono=0;//Only one instance per process!   
         mono++; assert(mono==1);
    }
    
    //Konstruktor bezparametrowy dla klienta
    MemoryPool():I_am_tree_server(false),segment(open_only,MEM_POOL_NAME)
    {
         static int mono=0;//Only one instance per process!   
         mono++; assert(mono==1);
    }
    
   ~MemoryPool()
    {
        if(I_am_tree_server)
        {
            //Remove shared memory on destruction
            shared_memory_object::remove(MEM_POOL_NAME);    
        }
    }  

    bool is_server()
    {
        return I_am_tree_server;
    }

    //Use as managed_shared_memory
    managed_shared_memory& segm()
    {
        return segment;
    }
};



}

#endif //FACJATA_MEMORY_POOL

