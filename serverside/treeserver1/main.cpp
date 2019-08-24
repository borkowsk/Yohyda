//Local server of "facies/facjata" resources stored in json files
//
//Based on examples from: 
//      http://techgate.fr/boost-property-tree/
//      https://stackoverflow.com/questions/12346787/send-complex-data-structure-via-boost-message-queue
//And docs:
//      https://www.boost.org/doc/libs/1_71_0/doc/html/boost/interprocess/message_queue_t.html
//
/// Treeserver, which loads the json file in a ptree and serve it's content
///
///
#include "memory_pool.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace facjata;

string MyName("TREESERVER-");//Process name

const char debug_path[]="/data/wb/SCC/working_copies/facies/private/TimelineOfTheEarth/posts/posts.json";

// Short alias for this namespace
namespace pt = boost::property_tree;

// Create a root of the tree
pt::ptree root;

// Control if it is more to do
unsigned NumberOfClients=0;

void process_request(string data, MemoryPool::ContentType msgType,facjata::MemoryPool& MyPool)
{
    switch(msgType)
    {
    case MemoryPool::ContentType::Control://There are no responses for control messages
    if(data.find_first_of("HelloFromPID")==0)
    {
        int PIDpos=data.find_first_of(":");
        if(PIDpos>0)
            std::cerr<<MyName<<" registered client PID"<<data.c_str()+PIDpos<<std::endl;
        NumberOfClients++;
    }
    else if(data.find_first_of("ByeFromPID")==0)
    {
        int PIDpos=data.find_first_of(":");
        if(PIDpos>0)
            std::cerr<<MyName<<" unregistered client PID"<<data.c_str()+PIDpos<<std::endl;
        NumberOfClients--;
    }
        break;
    case MemoryPool::ContentType::Write:
        break;
    case MemoryPool::ContentType::Read:
        break;
    default:
        std::cerr<<MyName<<" recived message of unexpected type "<<msgType<<", with content '"<<data<<"'"<<std::endl;
        break;
    }
}

int main(int argc, char* argv[])
{
        MyName+=boost::lexical_cast<string>(getpid());
        std::cerr<<"\n"<<MyName<<":"<<std::endl;

        try{
            std::cerr<<"Making communication pool & request queue"<<std::endl;//To jest serwer odpowiedzialny za ten obszar pamięci
            facjata::MemoryPool TestPool;//Próbuje się podłączyć jako klient
            //Jesli się uda to znaczy że server już działa
            std::cerr<<"Only one TREESERVER is alloved for single name"<<std::endl;
            return 1;
        }
        catch(const interprocess_exception& exc)//Raczej spodziewamy się błędu
        {
            //To exception jest OK
            exc.get_error_code();
        }

        //Teraz dopiero uznaje że może być serwerem
        try{
            facjata::MemoryPool MyMemPool(MemoryPool::IsServer::True);                  assert(MyMemPool.is_server());

            pt::read_json(debug_path, root);//Czyta podstawowe dane - jakiś całkiem spory plik json

            ShmCharAllocator     charallocator(MyMemPool.segm().get_segment_manager());
            //do{
            ShmString *stringToShare = MyMemPool->construct<ShmString>("TreeServerEmp")(charallocator);
            *stringToShare=
                    (
                        string("Facies/Facjata treeserver version 0.001; PID:")
                            +boost::lexical_cast<string>(getpid())
                        ).c_str();

            //Reciving request
            do{
                std::cerr<<MyName<<" reciving..."<<std::endl;
                string data;
                MemoryPool::ContentType msgType;
                data=MyMemPool.receive(msgType);//Tu poczeka na pierwszego klienta przynajmniej jakiś czas
                std::cerr<<MyName<<" recivided '"<<data<<"'"<<std::endl;
                process_request(data,msgType,MyMemPool);
                sleep(1);
            }while(NumberOfClients>0);

            MyMemPool->destroy<ShmString>("TreeServerEmp");
            std::cerr<<MyName<<": I'm finished."<<std::endl;
            return 0;
        }
        catch(const interprocess_exception& exc)
        {
            std::cerr<<MyName<<" recive an unexpected communication exception:'"<<exc.what()<<"'"<<std::endl;
            return -2;
        }

        return -9999;//Nie powinien tu trafić

}

