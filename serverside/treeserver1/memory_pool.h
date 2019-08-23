#ifndef FACJATA_MEMORY_POOL
#define FACJATA_MEMORY_POOL (1)

#include <boost/interprocess/ipc/message_queue.hpp>
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
const size_t    MEM_POOL_SIZE=0xffffff;
const char*     MSGQ_NAME="FacjataMemoryPoolQ";
const size_t    MSGQ_SIZE=1000;
const size_t    MSG_MAX_LEN=0xfff;

class MemoryPool
{
    bool I_am_tree_server; //Wazne przy destrukcji
    managed_shared_memory segment; //Segment pamięci
    message_queue  request_queue; //Kolejka zleceń

public:
    MemoryPool(bool TreeServerSide):
                I_am_tree_server((shared_memory_object::remove(MEM_POOL_NAME),message_queue::remove(MSGQ_NAME),true)),
                segment(create_only, MEM_POOL_NAME,
                        MEM_POOL_SIZE
                        ),
                request_queue(create_only, MSGQ_NAME, //open_or_create
                        MSGQ_SIZE,MSG_MAX_LEN
                        )
    {    
         static int mono=0;//Only one instance per process!
         mono++;                                                assert(mono==1);
    }

    //Konstruktor bezparametrowy dla klienta
    MemoryPool():
        I_am_tree_server(false),segment(open_only,MEM_POOL_NAME),request_queue(open_only,MSGQ_NAME)
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

    //Use the request queue
    message_queue& rq()
    {
        return request_queue;
    }

    message_queue& operator () ()
    {
        return request_queue;
    }

    //Use the managed_shared_memory segment
    managed_shared_memory& segm()
    {
        return segment;
    }

    managed_shared_memory* operator -> ()
    {
        return &segment;
    }

};



}

#endif //FACJATA_MEMORY_POOL

