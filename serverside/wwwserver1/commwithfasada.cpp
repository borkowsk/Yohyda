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
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>
#include <string>

using namespace fasada;

namespace http {
namespace server {

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
        string Msg("ByeFrom:");
        Msg+=MyName;
        MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message
        std::cerr<<MyName<<": bye, bye!"<<std::endl;
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

static std::shared_ptr<FasadaKeepAndExit> FasadaConnection;

static void do_when_first_time(fasada::MemoryPool& MyMemPool)
{
    std::cout<<"Initialize connection with TREESERVER"<<std::endl;
    FasadaConnection=std::shared_ptr<FasadaKeepAndExit>(new FasadaKeepAndExit(MyMemPool));
}

bool communicate_with_fasada(const request& req, reply& rep)//TMP IMPLEMENTATION
{
    try
    {
        static bool firsttime=true;
        static fasada::MemoryPool MyMemPool; //To jest klient! Konstruktor bez parametru

        if(firsttime)
        {
            do_when_first_time(MyMemPool);
            firsttime=false;
        }

        //Właściwa obsługa zapytania
        //...
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
}

} // namespace server
} // namespace http
