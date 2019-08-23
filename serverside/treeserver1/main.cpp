//Local server of "facies/facjata" resources stored in json files
//
//Based on examples from: 
//      http://techgate.fr/boost-property-tree/
//      https://stackoverflow.com/questions/12346787/send-complex-data-structure-via-boost-message-queue
//And docs:
//      https://www.boost.org/doc/libs/1_71_0/doc/html/boost/interprocess/message_queue_t.html
//
#include "memory_pool.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

const char debug_path[]="/data/wb/SCC/working_copies/facies/private/TimelineOfTheEarth/posts/posts.json";

// Short alias for this namespace
namespace pt = boost::property_tree;

// Create a root
pt::ptree root;

// Load the json file in this ptree and serve it's content
int main(int argc, char* argv[])
{
        std::cerr<<"\nTREESERVER:"<<std::endl;
        std::cerr<<"Making communication pool & request queue"<<std::endl;//To jest serwer odpowiedzialny za ten obszar pamięci

        facjata::MemoryPool MyMemPool(true/*SERVER*/);                  assert(MyMemPool.is_server());

        pt::read_json(debug_path, root);//Czyta podstawowe dane - jakiś całkiem spory plik json

        ShmCharAllocator     charallocator(MyMemPool.segm().get_segment_manager());
        //do{
        ShmString *stringToShare = MyMemPool->construct<ShmString>("TreeServerEmp")
                                                                        (charallocator);
        *stringToShare="Facies treeserver version 0.00";

        //Reciving request
        message_queue::size_type recvd_size;
        unsigned int               priority; //can be used to distinguish messages: control, write or just read
        string                     data;
        data.resize(facjata::MSG_MAX_LEN);
        std::cerr<<"TREESERVER reciving..."<<std::endl;
        MyMemPool().receive(&data[0],facjata::MSG_MAX_LEN,recvd_size,priority);
                                                                        assert(recvd_size<facjata::MSG_MAX_LEN);
        //
        //      sleep(1);
        //}while(1);
        std::cerr<<"TREESERVER recivided '"<<data<<"'"<<std::endl;

        sleep(10);
        
        std::cerr<<"TREESERVER: I'm finished."<<std::endl;
        return 0;
}

