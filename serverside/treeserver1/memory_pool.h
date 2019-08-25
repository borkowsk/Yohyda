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
#include <chrono>
#include <thread>
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

    bool is_server() const
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

    ShmString* wait_for_data(const string& ReqContent,
                             int DelayMs=10,int Repeat=200)//Wait some time until particular respond will be ready
    {
        if(is_server()) throw( interprocess_exception("Only client can wait for data prepared in server!") );
        try{
            for(int i=0;i<Repeat;i++)
            {
                std::pair<ShmString*, managed_shared_memory::size_type> res;
                res = segment.find<ShmString>(ReqContent.c_str());
                if(res.first!=nullptr)
                    return res.first;
                std::this_thread::sleep_for(std::chrono::milliseconds(DelayMs));//https://stackoverflow.com/questions/4184468/sleep-for-milliseconds/10613664#10613664?newreg=6841aea0490b47baa3c6a7ea2bebaa30
            }
        }
        catch(const interprocess_exception& exc)
        {
            std::cerr<<"MEMORY_POOL: Unavailable response about '"<<ReqContent<<"' because '"<<exc.what()<<"'"<<std::endl;
            return nullptr;
        }

        std::cerr<<"MEMORY_POOL: Time limit for response about '"<<ReqContent<<"'"<<std::endl;
        return nullptr;
    }

    void free_data(const char* Request)
    {
        segment.destroy<ShmString>(Request);
    }

    void free_data(string& Request)
    {
        segment.destroy<ShmString>(Request.c_str());
    }

//server_only:
    string receive(ContentType& Type)//Recive request on server side only
    {
        if(!is_server()) throw( interprocess_exception("You cannot recive from server queue!"));

        message_queue::size_type recvd_size;
        unsigned int               priority; //can be used to distinguish messages: control, write or just read
        string                     data;
        data.resize(facjata::MSG_MAX_LEN);
        request_queue.receive(&data[0],facjata::MSG_MAX_LEN,recvd_size,priority);        assert(recvd_size<facjata::MSG_MAX_LEN);
                                                                                         assert(priority<ContentType::Maxim);
        Type=ContentType(priority);
        return data;
    }

    void do_writer_request(const string& request)
    {
        if(!is_server()) throw( interprocess_exception("Only server can prepare answer!"));
        ShmCharAllocator charallocator(segment.get_segment_manager());
        ShmString *stringToShare = segment.construct<ShmString>(request.c_str())(charallocator);
        string path;
        string processor;
        string parameters;
        string proto;
        if(split_request(request,proto,path,processor,parameters))
        {
            //...
        }
        else //a wyjątki? TODO!?!?!
        {
            if(stringToShare!=nullptr)
            {
                *stringToShare="MEMORY_POOL - invalid request, split failed";
            }
        }
    }

    void do_reader_request(const string& request)
    {
        if(!is_server()) throw( interprocess_exception("Only server can prepare answer!"));
        ShmCharAllocator charallocator(segment.get_segment_manager());
        ShmString *stringToShare = segment.construct<ShmString>(request.c_str())(charallocator);
        string proto;
        string path;
        string processor;
        string parameters;

        if(split_request(request,proto,path,processor,parameters))
        {
            //...
        }
        else //a wyjątki? TODO!?!?!
        {
            if(stringToShare!=nullptr)
            {
                *stringToShare="MEMORY_POOL - invalid request, split failed";
            }
        }
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

    bool split_request(const string& request,string& proto,string& path,string& processor,string& parameters)
    {
        return false;
    }

};



}

#endif //FACJATA_MEMORY_POOL

