/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// @file "commwithfasada.cpp" - implementation of connection between fasada server & wwwserver
///
/// See licence file!
///
/// FASADA is a simple interface for communicating any user side application
/// thru WWW . Mostly based on boost examples.
/// This version work correctly with txt & html input from fasada
///

#include "request.hpp"
#include "reply.hpp"
#include "tree_types.h"
#include "memory_pool.h"
#include "mime_types.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>
#include <string>
#include <cctype>

using namespace fasada;

namespace http { //TO DO KASACJI?
namespace server { //DO ZAMIANY NA "fasada"?

const unsigned MILLISECONDS_BW=100;

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
        std::cerr<<"Sending '"<<Msg<<"' to fasada"<<std::endl;
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

static fasada::MemoryPool& do_when_first_time() //DZIWNA KONSTRUKCJA, ALE TYMCZASOWA BO BĘDZIE WIĘCEJ KOLEJEK i PROGRAMÓW Z Fasadą
{
    std::cout<<"Initialize connection with TREESERVER"<<std::endl;
    static fasada::MemoryPool MyHiddenPool; //To jest klient! Konstruktor bez parametru (NA RAZIE)
    FasadaConnection=std::shared_ptr<FasadaKeepAndExit>(new FasadaKeepAndExit(MyHiddenPool));
    return MyHiddenPool;
}

static bool findExt(const ShmString* resp,string& extension,string::size_type& extpos)
{
    extpos=resp->find(fasada::EXT_PRE,0);
    //std::cout<<'{'<<extpos<<'}'<<std::endl;

    if(extpos!=string::npos)
    {
        //std::cout<<'['<<extpos<<']';
        extension.clear(); extension.reserve(12);
        extpos+=strlen(fasada::EXT_PRE);

        const char* ext=resp->c_str()+extpos;
        while(!isblank(*ext) && !iscntrl(*ext))
        {
            //std::cout<<(*ext);
            extension+=(*ext);
            ext++;
        }

        extpos+=extension.size();
        //std::cout<<std::endl;
        return true;
    }
    else
        return false;
}

static void read_answer(reply& repl,ShmString* resp,const string& uri)
{
    string  extension="txt";//Plain/text is default MIME type
    string::size_type endpos=string::npos;
    string::size_type extpos=0;
    string::size_type begpos=0;

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
      //std::cout<<lastpos;//DEBUG

      if(replays!=0)//Jesli trzeba poczekac
      {
        begpos+=strlen(lastpos);//Przesuwamy o to co juz "wydrukowane"
        //https://stackoverflow.com/questions/4184468/sleep-for-milliseconds/10613664#10613664?newreg=6841aea0490b47baa3c6a7ea2bebaa30
        std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_BW));
      }

    }while(endpos==string::npos && replays<20);

    if(replays!=0)//Nie doczekal sie
    {
       std::cerr<<"\n"<<FasadaConnection->Name()<<" waiting "<<replays*MILLISECONDS_BW<<"ms for continuation"<<std::endl;
       std::cerr<<"Shared memory for '"<<uri<<"' potentially remain available for reread"<<std::endl;
       (*resp)+="\n\nAnswer incomplete! Reload the same request after some time!\n";
    }

    if( findExt(resp,extension,extpos) )
    {
        extpos++;//Tym razem znak separatora też
        std::cout<<"("<<extpos<<") '"<<extension<<"'"<<std::endl;
    }
    else
        extpos=0;

    // Fill out the reply to be sent to the client.
    repl.status = reply::ok;
    repl.content.assign(resp->c_str()+extpos,resp->size()-extpos);//Czy takie assign jest wrazliwe na koniec c-stringa?
                                                    //Tak bezpieczniej? Wszystkie dane, nawet jak są w srodku znaki \0?

    repl.headers.resize(2);
    repl.headers[0].name = "Content-Length";
    repl.headers[0].value = std::to_string(repl.content.size());
    repl.headers[1].name = "Content-Type";
    repl.headers[1].value = mime_types::extension_to_type(extension.c_str()).c_str();

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
        static fasada::MemoryPool& MyMemPool=do_when_first_time();

        if(curr_request.uri=="/!!!!")//SPECIAL!!!
        {
            FasadaConnection=nullptr;//deallocation means close connection with treeserver!
            exit(-9999);//DEBUG exit of wwwserver
        }

        //Właściwa obsługa zapytania
        string req_uri="http://"+host+":"+port;//DEBUG
        req_uri+=curr_request.uri.c_str();

        if( req_uri.find("!",0)!=req_uri.npos )//marker że "zapis" to ! przed nazwą processora
            MyMemPool.send_request(req_uri,MemoryPool::ContentType::Write);
        else
            MyMemPool.send_request(req_uri,MemoryPool::ContentType::Read);

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
                         //bo ma co wysyłać, więc może kontynuować LANG?
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
