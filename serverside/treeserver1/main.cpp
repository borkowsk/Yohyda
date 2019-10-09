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

#include "ptree_foreach.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/locale.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>
#include <time.h>       /* time_t, struct tm, time, localtime */

//Procesory danych json w postaci zmiennych w main.
#include "processor_ls.h"
#include "processor_get.h"
#include "processor_dfs.h"

using namespace fasada;

string MyName("TREESERVER-");//Process name

const char debug_path[]="/data/wb/SCC/working_copies/facies/private/pages/Memetyka/posts/posts_1.json";
                      ///"/data/wb/SCC/working_copies/facies/private1/TimelineOfTheEarth/posts/posts_x.json";

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
    int position;//Pomoocnicza pozycja
    switch(msgType)
    {
    case MemoryPool::ContentType::Control://There are no responses for control messages
        if((position=request.find("HelloFrom",0))==0)
        {
            int position=request.find("-",7);
            if(position!=request.npos)
            {
                NumberOfClients++;
                std::cout<<MyName<<" has registered client PID"<<request.c_str()+position<<std::endl;
            }
        }
        else if(request.find("ByeFrom",0)==0)
        {
            int position=request.find("-",7);
            if(position!=request.npos)
            {
                NumberOfClients--;
                std::cout<<MyName<<" has unregistered client PID"<<request.c_str()+position<<std::endl;
            }
        }
        else
        {
            std::cerr<<MyName<<" has recivied invalid control request "<<request.c_str()+position<<std::endl;
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

void recode_timestamps(pt::ptree& start)
{
/*
     pt::ptree& paren=start.get_child("1");
     pt::ptree& child=start.get_child("1.timestamp");
     time_t timestamp=boost::lexical_cast<time_t>(child.data());
     struct tm * timeinfo;
     timeinfo = localtime (&timestamp);
     std::string timeString=asctime(timeinfo);//"Mon";//"Mon Oct  7 22-45-46 2019";//asctime(timeinfo);
     for(char& c:timeString) if(c=='\n') c=';';
     std::cout<<timestamp<<"="<<timeString<<std::endl;
     paren.put("timestamp_asc",timeString.c_str());
*/
     foreach_node(start,"root",always,always,
         [](ptree& t,std::string k)
         {
             if(k.find("timestamp",0)!=k.npos)
             {
                std::cout<<k<<":'"<<t.data()<<"'"<<std::endl;
                try
                {
                    struct tm  *timeinfo;
                    time_t timestamp=boost::lexical_cast<time_t>(t.data());
                    std::string tmp=t.data();
                    t.clear();
                    t.put("unix",tmp);
                    timeinfo = localtime (&timestamp);//http://www.cplusplus.com/reference/ctime/localtime/
                    std::string timeString=asctime(timeinfo);//alt.: http://www.cplusplus.com/reference/ctime/strftime/ ?
                    for(char& c:timeString) if(c=='\n') c=';';
                    t.put("asci",timeString);
                }catch(...)
                {/*JAK JAKIS ERROR TO NIE ROBI NIC*/}
             }
             return false;//nieistotne
         }
         );

}

void recode_facebook_pl_to_utf8(pt::ptree& start)
{
/*
    pt::ptree& child=start.get_child("1.data.1");
    for(auto& c:child)
    {
        std::string latin1_string=boost::locale::conv::from_utf(c.second.data(),"Latin1");
        //std::string utf8_string=boost::locale::conv::to_utf<char>(latin1_string,"Latin1");
        std::cout<<c.first.data() <<":"<<latin1_string<<std::endl;
        c.second.data()=latin1_string;
    }
*/
    foreach_node(start,"root",always,
        [](ptree& t,std::string k)
        {
            //std::cout<< k <<":\t";
            std::string latin1_string=boost::locale::conv::from_utf(t.data(),"Latin1");
            t.data()=latin1_string;
            //std::cout<< t.data() <<" ; "<<std::endl;
            return false;//blokuje wywołanie "after", które i tak jest "never"
        }
        );
}

int main(int argc, char* argv[])
{
    //Dopiero w main jest pewność że wewnętrzne struktury static zostały zainicjalizowane. Ale to słabe...
    fasada::processor_get GET;//Istnieje conajmniej jedna taka zmienna.
    fasada::processor_dfs DFS;//--//---
    fasada::processor_ls  LS; //--//---
    fasada::processor_ls  Def("default");//chyba że ktoś chce robić aliasy
    fasada::processor_dfs Tree("tree");

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
        insert_numbers(root);
        recode_timestamps(root);
        recode_facebook_pl_to_utf8(root);


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
        pt::write_json("output.json",root);
        //pt::write_xml("output.xml",root);// --> https://stackoverflow.com/questions/18875437/writing-more-complex-than-trivial-xml-with-boost-property-tree
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

