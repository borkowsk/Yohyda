#ifndef FACJATA_MEMORY_POOL
#define FACJATA_MEMORY_POOL (1)

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <iostream>
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

const char*     MEM_POOL_NAME="FacjataDefMemoryPool";
const size_t    MEM_POOL_SIZE=0xffffff;
const char*     MSGQ_NAME="FacjataDefMemoryPoolQ";
const size_t    MSGQ_SIZE=1000;
const size_t    MSG_MAX_LEN=0xfff;

class MemoryPool
{
    bool I_am_tree_server; //Wazne przy destrukcji
    managed_shared_memory segment; //Segment pamięci
    message_queue  request_queue; //Kolejka zleceń

public:
    enum IsServer {True=1};//This enum is only for differentiate beetween constructors
    MemoryPool(IsServer Really):
                I_am_tree_server((shared_memory_object::remove(MEM_POOL_NAME),
                                  message_queue::remove(MSGQ_NAME),
                                  true)), //First time in life I have use this "," syntax! :-D
                segment(create_only,   MEM_POOL_NAME, //create only! Should not exist now!
                        MEM_POOL_SIZE
                        ),
                request_queue(create_only, MSGQ_NAME, //create only! Should not exist now!
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
    }
    
   ~MemoryPool()
    {
        if(I_am_tree_server)
        {
            //Remove shared memory & queue on destruction
            std::cerr<<"MEMORY_POOL removing system resources"<<std::endl;
            shared_memory_object::remove(MEM_POOL_NAME);
            message_queue::remove(MSGQ_NAME);
            std::cerr<<". DONE!"<<std::endl;
        }
    }  

    bool is_server()
    {
        return I_am_tree_server;
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

    //Use the request queue
    enum ContentType { Control=0, Write=1, Read=2, Maxim=3 };

    void send_request(string Content, ContentType  Type)//Send a request on client side only
    {
        if(is_server()) throw( interprocess_exception("You cannot send request to yourself!") );
        request_queue.send(Content.c_str(),Content.length(),int(Type));//Send text message
    }

    string receive(ContentType& Type)//Recive request on server side only
    {
        if(!is_server()) throw( interprocess_exception("You cannot recive from server queue!")) ;
        message_queue::size_type recvd_size;
        unsigned int               priority; //can be used to distinguish messages: control, write or just read
        string                     data;
        data.resize(facjata::MSG_MAX_LEN);
        request_queue.receive(&data[0],facjata::MSG_MAX_LEN,recvd_size,priority);        assert(recvd_size<facjata::MSG_MAX_LEN);
                                                                                         assert(priority<ContentType::Maxim);
        Type=ContentType(priority);
        return data;
    }

    ShmString* wait_for_data(string& ReqContent,ContentType& Type,
                             int DelayMs=10,int Repeat=100)//Wait some time until particular respond will be ready
    {
        return nullptr;
    }

protected:
    message_queue& rq() //Direct acces to queue
    {
        return request_queue;
    }

    message_queue& operator () () //Direct acces to queue
    {
        return request_queue;
    }

};



}

#endif //FACJATA_MEMORY_POOL

