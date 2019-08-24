//Local tree console editor for facies/facjata
//
#include "memory_pool.h"
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

using namespace facjata;

string MyName("TREEEDIT-");
string Prefix("/");
bool   finish=false;

bool do_local_processing(const string& data)
{
    string::size_type outsize=0;
    if((outsize=data.find("prefix",0,6))==0)
    {
        std::cerr<<outsize<<"!"<<data<<std::endl;
        int PIDpos=data.find_first_of(":=");
        if(PIDpos>0)
        {
            Prefix=(data.c_str()+PIDpos+1);
            std::cout<<"prefix='"<<Prefix<<"'"<<std::endl;
        }
        return true;
     }
    else
    if((outsize=data.find("help",0,4))==0)
    {
        std::cerr<<outsize<<"?"<<data<<std::endl;
        std::cout<<"Type 'done' to finish."<<std::endl;
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

    std::cerr<<outsize<<":"<<data<<std::endl;
    return false;
}

void do_work(facjata::MemoryPool& MyPool)//Real work to do
{
    string iline;
    do{
        std::cout<<">";
        std::cout.flush();
        //std::cin.getline(iline,'\n');
        std::cin>>iline;
        std::cerr<<MyName<<" get input:'"<<iline<<"'"<<std::endl;
        if(!do_local_processing(iline.c_str()))
        {
            std::cerr<<MyName<<" sending..."<<std::endl;
            MyPool.send_request(Prefix+iline,MemoryPool::ContentType::Read);
            std::cerr<<MyName<<" waiting for response..."<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));//https://stackoverflow.com/questions/4184468/sleep-for-milliseconds/10613664#10613664?newreg=6841aea0490b47baa3c6a7ea2bebaa30
        }
    }while(!std::cin.eof() && !finish);
}

int main(int argc, char* argv[])
{
        MyName+=boost::lexical_cast<string>(getpid());
        std::cerr<<"\n"<<MyName<<":"<<std::endl;

        try{
            std::cerr<<"Looking for treeserver"<<std::endl;

            facjata::MemoryPool MyMemPool; //To jest klient! Konstruktor bez parametru

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
            string Msg("HelloFromPID:");
            Msg+=boost::lexical_cast<string>(getpid())+" ТRЕЕЕDIT";
            MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message

            do_work(MyMemPool);//Real work to do

            //Send BYE to server
            Msg="ByeFromPID:"+boost::lexical_cast<string>(getpid());
            MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message
            std::cerr<<MyName<<": bye, bye!"<<std::endl;
            return 0;
        }
        catch(const interprocess_exception& exc)
        {
            std::cerr<<MyName<<" recive an unexpected communication exception:'"<<exc.what()<<"'"<<std::endl;
            return -2;
        }

        return -9999;//Nie powinien tu trafić
}
