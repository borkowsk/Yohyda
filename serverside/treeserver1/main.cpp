//Local server of "fasada" resources stored in json files
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
#include "tree_processor.h"
#include "URLparserLib/URLparser.hpp"
#include <boost/lexical_cast.hpp>
//#include <boost/interprocess/streams/vectorstream.hpp>//to jednak nie tak dziala jakbym chcial
#include "PTREEWalker/ptree_foreach.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>

using namespace fasada;

string MyName("TREESERVER-");//Process name

const char debug_path[]="/data/wb/SCC/working_copies/facies/private/TimelineOfTheEarth/posts/posts.json";

// Short alias for this namespace


// Create a root of the tree
pt::ptree root;

// Control if it is more to do
unsigned NumberOfClients=0;

inline URLparser split_request(const string& request)//May throw exceptions
{
    URLparser URL(request.c_str());
    //... Test or add some extra work
    return URL;
}

void do_reader_request(const string& request,fasada::MemoryPool& MyPool)//May throw exceptions
{
    ShmCharAllocator charallocator(MyPool->get_segment_manager());
    ShmString *stringToShare = nullptr;
    try
    {
        stringToShare = MyPool->construct<ShmString>(request.c_str())(charallocator);
    }
    catch(...)
    {
        std::cerr <<MyName<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        stringToShare=nullptr;
    }
    if(stringToShare==nullptr)
        throw( interprocess_exception("Server was not able to construct shared memory string!") );

    URLparser URL=split_request(request);//May throw exceptions
    if(URL.find("&path")!= URL.end())//Sciezka musi byc zawsze
    {
        //ShmStream outstr(*stringToShare);charallocator? //to jednak nie tak dziala jakbym chcial
        //outstr << URL["protocol"] << '\n'<< URL["domain"] << '\n'<< URL["path"] << '\n'<< URL["processor"] << '\n'<< URL["query"] << std::endl;
        for(auto p:URL)
        {
            (*stringToShare)+=p.first.c_str();
            (*stringToShare)+=" = ";
            (*stringToShare)+=p.second.c_str();
            (*stringToShare)+="\n";
        }

        try{
            tree_processor& TheProcessor=tree_processor::getReadProcessor( URL["&processor"] );
            val_string& path=URL["&path"];
            //(*stringToShare)+="path:"+path+"\t";
            //Pierszy slash ścieżki stanowi problem...
            pt::ptree& branch =( path=="/" ? root : root.get_child(pt::ptree::path_type{path.c_str()+1, '/'}) );
            //(*stringToShare)+="==\n";
            TheProcessor.read_tree((*stringToShare),branch,URL);
        }
        catch(const pt::ptree_error& exc)
        {
            *stringToShare+=exc.what();
            *stringToShare+=MEM_END;
        }
        catch(const std::runtime_error& exc)
        {
            *stringToShare+=exc.what();
            *stringToShare+=MEM_END;
        }
        catch(...)
        {
             *stringToShare+=("UNEXPECTED ERROR @" + boost::lexical_cast<std::string>( __LINE__ )
                              +MEM_END);
        }
    }
    else
    {
        if(stringToShare!=nullptr)
        {
            *stringToShare=(MyName+": invalid request, split failed").c_str();
            *stringToShare+=MEM_END;
        }
    }
}

void do_writer_request(const string& request,fasada::MemoryPool& MyPool)//May throw exceptions
{   //SUBJECT TO CHANGE! TODO
    ShmCharAllocator charallocator(MyPool->get_segment_manager());
    ShmString *stringToShare = MyPool->construct<ShmString>(request.c_str())(charallocator);
    if(stringToShare==nullptr)
        throw( interprocess_exception("You cannot construct shared memory string!") );

    URLparser URL=split_request(request);//May throw exceptions
    if(URL.find("&path")!= URL.end())//Sciezka musi byc zawsze
    {
        *stringToShare=(URL["&protocol"]+'\n'
                        +URL["&domain"]+'\n'
                        +URL["&path"]+'\n'
                        +URL["&processor"]+'\n'
                        +URL["&query"]).c_str();
        *stringToShare+=MEM_END;
    }
    else //a wyjątki? TODO!?!?!
    {
        if(stringToShare!=nullptr)
        {
            *stringToShare=(MyName+": invalid request, split failed").c_str();
            *stringToShare+=MEM_END;
        }
    }
}

void do_local_processing(string& request, MemoryPool::ContentType msgType,fasada::MemoryPool& MyPool)//May throw exceptions
{
    switch(msgType)
    {
    case MemoryPool::ContentType::Control://There are no responses for control messages
        if(request.find("HelloFromPID",0)==0)
        {
            int PIDpos=request.find_first_of(":");
            if(PIDpos>0)
                std::cerr<<MyName<<" registered client PID"<<request.c_str()+PIDpos<<std::endl;
            NumberOfClients++;
        }
        else if(request.find("ByeFromPID",0)==0)
        {
            int PIDpos=request.find_first_of(":");
            if(PIDpos>0)
                std::cerr<<MyName<<" unregistered client PID"<<request.c_str()+PIDpos<<std::endl;
            NumberOfClients--;
        }
        break;
    case MemoryPool::ContentType::Write:
        do_writer_request(request,MyPool);
        break;
    case MemoryPool::ContentType::Read:
        do_reader_request(request,MyPool);
        break;
    default: //Exception?
        std::cerr<<MyName<<" recived message of unexpected type "<<msgType
                <<", with content '"<<request<<"'"<<std::endl;
        break;
    }
}

int main(int argc, char* argv[])
{
        MyName+=boost::lexical_cast<string>(getpid());
        std::cerr<<"\n"<<MyName<<":"<<std::endl;

        if(argc<2 || (string("--force"))!=argv[1])
        try{
            fasada::MemoryPool TestPool;//Próbuje się podłączyć jako klient
            //Jesli się uda to znaczy że server już działa
            std::cerr<<"Only one TREESERVER is alloved!\nKill the not responding server and start again with --force"<<std::endl;
            return 1;
        }
        catch(const interprocess_exception& exc)//Raczej spodziewamy się błędu
        {
            //To exception jest OK
            exc.get_error_code();
        }

        //Teraz dopiero uznaje że może być serwerem
        try{
            std::cerr<<"Making communication pool & request queue"<<std::endl;//To jest serwer odpowiedzialny za ten obszar pamięci
            fasada::MemoryPool MyMemPool(MemoryPool::IsServer::True);                  assert(MyMemPool.is_server());

            pt::read_json(debug_path, root);//Czyta podstawowe dane - jakiś całkiem spory plik json

            ShmCharAllocator charallocator(MyMemPool.segm().get_segment_manager());
            //do{
            ShmString *stringToShare = MyMemPool->construct<ShmString>("TreeServerEmp")(charallocator);
            *stringToShare=
                    (
                        string("FASADA treeserver version 0.007; PID:")
                            +boost::lexical_cast<string>(getpid())
                        ).c_str();

            //receive & process the request!
            do{
                std::cerr<<MyName<<" receiving..."<<std::endl;
                string data;
                MemoryPool::ContentType msgType;
                try{
                    data=MyMemPool.receive(msgType);//Tu poczeka na pierwszego klienta przynajmniej jakiś czas
                    std::cerr<<MyName<<" received '"<<data<<"'"<<std::endl;
                    do_local_processing(data,msgType,MyMemPool);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));//https://stackoverflow.com/questions/4184468/sleep-for-milliseconds/10613664#10613664?newreg=6841aea0490b47baa3c6a7ea2bebaa30
                }
                catch(const interprocess_exception& exc)
                {
                    std::cerr<<MyName<<": in loop communication exception:'"<<exc.what()<<"'"<<std::endl;
                }
           }while(NumberOfClients>0);

            MyMemPool.free_data("TreeServerEmp");
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

