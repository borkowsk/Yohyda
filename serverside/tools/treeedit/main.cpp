//Local tree console editor for facies/facjata
//
#include "memory_pool.h"
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace facjata;

string MyName("TREEEDIT-");

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
            Msg+=boost::lexical_cast<string>(getpid());
            MyMemPool.send_request(Msg,MemoryPool::ContentType::Control);//Control message

            sleep(5);//Real job ;-)

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
