// FASADA is a simple interface for communicating any user side application
// thru WWW . Mostly based on boost examples.
//
// Copyright (c) 2019, Wojciech Borkowski
// (wborkowsk MAŁPA gmail KROPA com , wborkowski MAŁPA uw KROPA edu KROPA pl)
//
#include "URLparser.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "tree_types.h"
#include "memory_pool.h"
#include "mime_types.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>
#include <string>

using namespace fasada;

namespace http {
namespace server {

extern "C" //Te dwie funcje do eksportowania jako gole nazwy
{
    void set_host_and_port_for_fasada(const char* iHost,const char* iPort);
    bool communicate_with_fasada(const request& curr_request, reply& curr_reply);
}

class FasadaKeepAndExit
{
    string  MyName;
    fasada::MemoryPool& MyMemPool;//Udostępnialna kopia zabezpieczanej zmiennej
public:
    FasadaKeepAndExit(fasada::MemoryPool& MyPool):
        MyMemPool(MyPool),MyName("WWWSERVER1-")
    {
        MyName+=boost::lexical_cast<string>(getpid());
        //The first data may be used for checking if it is a valid treeserver
        std::pair<ShmString*, managed_shared_memory::size_type> res;
        res = MyMemPool->find<ShmString>("TreeServerEmp");

        if(res.first==nullptr)
            throw interprocess_exception("A proper TREESERVER not found!");

        if(res.second!=1)
            throw interprocess_exception("Incompatible TREESERVER found!");//Nie spodziewa się tablicy!

        //Send Hello to server
        string Msg("HelloFrom:");
        Msg+=MyName.c_str();
        MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message
    }
    ~FasadaKeepAndExit()
    {
        //Send BYE to server
        std::cerr<<MyName<<": bye, bye!"<<std::endl;
        string Msg("ByeFrom:");
        Msg+=MyName;
        MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message
    }
    const string& Name()
    {
        return MyName;
    }
    fasada::MemoryPool& Pool()
    {
        return MyMemPool;
    }
};


static ipc::string host="localhost";
static ipc::string port="8000";
static std::shared_ptr<FasadaKeepAndExit> FasadaConnection;

static void do_when_first_time(fasada::MemoryPool& MyMemPool)
{
    std::cout<<"Initialize connection with TREESERVER"<<std::endl;
    FasadaConnection=std::shared_ptr<FasadaKeepAndExit>(new FasadaKeepAndExit(MyMemPool));
}

static void read_answer(reply& repl,ShmString* resp,const string& uri)
{
    std::string  extension="txt";//Plain/text is default MIME type
    string::size_type begpos=0;
    string::size_type endpos=string::npos;

    unsigned replays=0;
    do
    {
      //endpos=resp->find(string_view(fasada::MEM_END),begpos);//C++17
      endpos=resp->find(fasada::MEM_END,begpos,strlen(fasada::MEM_END));

      if(endpos!=string::npos)
      {
          //std::cout<<"("<<endpos<<")"<<std::endl;
          //std::cout<<"("<<resp->c_str()+endpos<<")"<<std::endl;
          resp->erase(endpos);
          replays=0;
      }
      else
      {
          replays++;
      }

      const char* lastpos=resp->c_str()+begpos;
      std::cout<<lastpos;

      if(replays!=0)//Jesli trzeba poczekac
      {
        begpos+=strlen(lastpos);//Przesuwamy o to co juz wydrukowane
        //std::cout<<".";std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));//https://stackoverflow.com/questions/4184468/sleep-for-milliseconds/10613664#10613664?newreg=6841aea0490b47baa3c6a7ea2bebaa30
      }

    }while(endpos==string::npos && replays<20);

    if(replays!=0)//Nie doczekal sie
    {
       std::cerr<<"\n"<<FasadaConnection->Name()<<" waiting "<<replays*200<<"ms for continuation"<<std::endl;
       std::cerr<<"Shared memory for '"<<uri<<"' potentially remain available for reread"<<std::endl;
       (*resp)+="\n\nAnswer incomplete! Reload the same request after some time!\n";
    }

    // Fill out the reply to be sent to the client.
    repl.status = reply::ok;
    repl.content.assign(resp->c_str(),resp->size());//Czy takie assign jest wrazliwe na koniec c-stringa?
                                                    //Tak bezpieczniej? Wszystkie dane, nawet jak są w srodku znaki \0?

    repl.headers.resize(2);
    repl.headers[0].name = "Content-Length";
    repl.headers[0].value = std::to_string(repl.content.size());
    repl.headers[1].name = "Content-Type";
    repl.headers[1].value = mime_types::extension_to_type(extension).c_str();

    std::cout<<"\nRequest: "<<uri<<" done."<<std::endl;
    std::cout<<repl.headers[0].name<<" : "<<repl.headers[0].value<<std::endl;
    std::cout<<repl.headers[1].name<<" : "<<repl.headers[1].value<<std::endl;

    //Mozna ewentualnie posprzatac
    if(replays==0) //Bylo dokonczone, mozna usunac blok komunikacyjny
        FasadaConnection->Pool().free_data(uri.c_str());//Tylko wtedy mozna usunac obszar gdy produkcja została zakończona
}

//Only these 2 functions are globally visible:

bool communicate_with_fasada(const request& curr_request, reply& curr_reply) // extern "C"
{
    try
    {
        static bool firsttime=true;
        static fasada::MemoryPool MyMemPool; //To jest klient! Konstruktor bez parametru (NA RAZIE)

        if(curr_request.uri=="/!!!!")
        {
            FasadaConnection=nullptr;//deallocation means close connection with treeserver!
            exit(-9999);//DEBUG exit of wwwserver
        }

        if(firsttime)
        {
            do_when_first_time(MyMemPool);
            firsttime=false;
        }

        //Właściwa obsługa zapytania
        string req_uri="http://"+host+":"+port;//DEBUG
        req_uri+=curr_request.uri.c_str();

        if(1)//How to check this?
            MyMemPool.send_request(req_uri,MemoryPool::ContentType::Read);
        else
            MyMemPool.send_request(req_uri,MemoryPool::ContentType::Write);

        //if(verbose)
            std::cerr<<FasadaConnection->Name()<<" waiting for response from 'fasada'..."<<std::endl;

        ShmString* response=MyMemPool.wait_for_data(req_uri);//Odebranie odpowiedzi z pamięci dzielonej

        if(response==nullptr)
        {
            std::cout<<FasadaConnection->Name()<<": Response unavailable"<<std::endl;
            return false;//TOTALLY FAILED
        }
        else
        {
            read_answer(curr_reply,response,req_uri);
            return true; //COŚ ZOSTAŁO ODEBRANE - Z PUNKTU WIDZENIA SERWERA OK
                         //bo ma co wysyłać, więc może kontynuować
        }
    }
    catch(const interprocess_exception& exc)
    {
        std::cerr<<FasadaConnection->Name()<<" communication failed because '"<<exc.what()<<"'"<<std::endl;
        return false;//TOTALLY FAILED
    }
    catch(...)
    {
        std::cerr <<FasadaConnection->Name()<<
            ": Unexpected exception, diagnostic information follows:\n" <<
            boost::current_exception_diagnostic_information();
        return false;//TOTALLY FAILED
    }
    return false;//SHOULD NEVER BE USED!
}

void set_host_and_port_for_fasada(const char* iHost,const char* iPort)// extern "C"
{
    host=iHost;
    port=iPort;
}

} // namespace server
} // namespace http
