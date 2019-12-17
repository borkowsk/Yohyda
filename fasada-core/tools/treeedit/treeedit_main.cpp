/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///
#define UNIT_IDENTIFIER "treeedit"
//Local tree console editor for "fasada"
//
#include "memory_pool.h"
#include <boost/lexical_cast.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>

using namespace fasada;

string MyName("TREEEDIT-");
const char* defaultPrefix="http://localhost:8000/";
string Prefix(defaultPrefix);
bool   verbose=false;
bool   finish=false;

bool do_local_processing(const string& data,fasada::MemoryPool& MyPool)
{
    string::size_type outsize=0;
    if((outsize=data.find("verbose",0,7))==0)
    {
        verbose=!verbose;
        return true;
    }
    else
    if((outsize=data.find("inspect:",0,8))==0)
    {
        const char* what=(data.c_str()+8);
        std::cout<<what<<":";
        std::pair<ShmString*, managed_shared_memory::size_type> res = MyPool->find<ShmString>(what);
        if(res.second!=0)
            std::cout<<res.first<<"  '"<<*res.first<<"'"<<std::endl;
        else
            std::cout<<" NOT FOUND"<<std::endl;
        return true;
    }
    else
    if((outsize=data.find("delete:",0,7))==0)
    {
        const char* what=(data.c_str()+7);
        MyPool.free_data(what);
        return true;
    }
    else
    if((outsize=data.find("prefix",0,6))==0)
    {
        //std::cerr<<outsize<<"!"<<data<<std::endl;
        if(data[6]=='=')
        {
            Prefix=(data.c_str()+7);
            if(Prefix=="default")
               Prefix=defaultPrefix;
        }
        else if(data[6]=='+')
        {
            Prefix+=(data.c_str()+7);
        }
        if(data[6]=='-')
        {
            Prefix.erase(Prefix.rfind('/'));
        }

        std::cout<<"prefix='"<<Prefix<<"'"<<std::endl;
        return true;
     }
    else
    if((outsize=data.find("help",0,4))==0)
    {
        //std::cerr<<outsize<<"?"<<data<<std::endl;
        std::cout<<"Type 'done' or 'exit' to finish. Other commands are:"<<std::endl;
        std::cout<<"prefix=/path/ or default for set prefix;"<<std::endl;
        std::cout<<"prefix+path/ or prefix- for change prefix;"<<std::endl;
        std::cout<<"inspect:/path/ for see remains in shared memory;"<<std::endl;
        std::cout<<"delete:/path/ for erase remains in shared memory."<<std::endl;
        std::cout<<"Anything other will be send to server"<<std::endl;
        return true;
    }
    else
    if((outsize=data.find("done",0,4))==0)
    {
        finish=true;
        return true;
    }
    else
    if((outsize=data.find("exit",0,4))==0)
    {
        finish=true;
        return true;
    }

    //std::cerr<<outsize<<":"<<data<<std::endl;
    return false;
}

void do_work(fasada::MemoryPool& MyPool)//Real work to do
{
    string iline;
    do{
        try
        {
            std::cout<<"\n"<<Prefix<<">";
            std::cout.flush();
            std::cin>>iline;
            if(verbose) std::cerr<<MyName<<" get input:'"<<iline<<"'"<<std::endl;

            if(!do_local_processing(iline.c_str(),MyPool) )
            {
                if(verbose) std::cerr<<MyName<<" sending..."<<std::endl;
                string req=Prefix+iline;

                if( req.find("!",0)!=req.npos )//marker że "zapis" to ! przed nazwą
                    MyPool.send_request(req,MemoryPool::ContentType::Write);
                else
                    MyPool.send_request(req,MemoryPool::ContentType::Read);

                if(verbose) std::cerr<<MyName<<" waiting for response..."<<std::endl;

                ShmString* response=MyPool.wait_for_data(req);

                if(response==nullptr)
                {
                    std::cout<<MyName<<": Response unavailable"<<std::endl;
                }
                else
                {
                    string::size_type begpos=0;
                    string::size_type endpos=string::npos;
                    unsigned replays=0;
                    do
                    {
                      //endpos=response->find(string_view(fasada::MEM_END),begpos);//C++17
                      endpos=response->find(fasada::MEM_END,begpos,strlen(fasada::MEM_END));
                      if(endpos!=string::npos)
                      {
                          //std::cout<<"("<<endpos<<")"<<std::endl;
                          //std::cout<<"("<<response->c_str()+endpos<<")"<<std::endl;
                          response->erase(endpos);
                          replays=0;
                      }
                      else
                      {
                          replays++;
                      }

                      const char* lastpos=response->c_str()+begpos;
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
                       std::cerr<<"\n"<<MyName<<" waiting "<<replays*200<<"ms for continuation"<<std::endl;
                       std::cerr<<"Shared memory for '"<<req<<"' potentially remain available for inspection"<<std::endl;
                    }
                    else
                        MyPool.free_data(req);//Tylko wtedy mozna usunac obszar!
                }
            }
        }
        catch(...)
        {
            std::cerr <<MyName<<
                ": Unexpected exception, diagnostic information follows:\n" <<
                boost::current_exception_diagnostic_information();
        }
    }while(!std::cin.eof() && !finish);
}

int main(int argc, char* argv[])
{
        MyName+=boost::lexical_cast<string>(getpid());
        std::cerr<<"\n"<<MyName<<":"<<std::endl;

        try{
            std::cerr<<"Looking for treeserver"<<std::endl;

            fasada::MemoryPool MyMemPool; //To jest klient! Konstruktor bez parametru

            //The first data may be used for checking if it is a valid treeserver
            std::pair<ShmString*, managed_shared_memory::size_type> res;
            try{
                res = MyMemPool->find<ShmString>("TreeServerEmp");
                if(res.first==nullptr) throw interprocess_exception("A proper TREESERVER not found!");
                std::cerr<<"Recived "<<res.second<<" string(s), the 1st is: \n\t'"<<*res.first<<"'"<<std::endl;
                if(res.second!=1)  throw interprocess_exception("Incompatible TREESERVER found!");//Nie spodziewa się tablicy!
            }
            catch(const interprocess_exception& exc)
            {
                std::cerr<<MyName<<" can not countinue because '"<<exc.what()<<"'"<<std::endl;
                return -1;
            }

            //Send Hello to server
            string Msg("HelloFrom:");
            Msg+=MyName;
            MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message

            do_work(MyMemPool);//Real work to do

            //Send BYE to server
            Msg="ByeFrom:"+MyName;
            MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message
            std::cerr<<MyName<<": bye, bye!"<<std::endl;
            return 0;
        }
        catch(const interprocess_exception& exc)
        {
            std::cerr<<MyName<<" recive an unexpected communication exception:'"<<exc.what()<<"'"<<std::endl;
            return -2;
        }
        catch(...)
        {
            std::cerr <<MyName<<
                ": Unexpected exception, diagnostic information follows:\n" <<
                boost::current_exception_diagnostic_information();
        }

        return -9999;//Nie powinien tu trafić
}
