//treeedit main.cpp
#include "memory_pool.h"
#include <iostream>

int main(int argc, char* argv[])
{
        std::cerr<<"\nTREEDIT:"<<std::endl;
        std::cerr<<"Looking for treeserver"<<std::endl;

        //To tylko klient!!!
        facjata::MemoryPool MyMemPool;                              assert(MyMemPool.is_server()==false);

        std::pair<ShmString*, managed_shared_memory::size_type> res;

        //The first message may be used for checking if it is a valid treeserver
        res = MyMemPool->find<ShmString>("TreeServerEmp");          assert(res.first!=nullptr);
        std::cerr<<"Recived "<<res.second<<" string(s), the 1st is: \n\t'"<<*res.first<<"'"<<std::endl;
                                                                    assert(res.second==1);//Nie spodziewa się tablicy!
        //MyMemPool->destroy<ShmString>("TreeServerEmp");

        //Send a request
        string Msg("Hello! I'a your client.");
        MyMemPool().send(Msg.c_str(),Msg.length(),0);//Control message

        sleep(1);

        std::cerr<<"TREEEDIT: bye, bye!"<<std::endl;
        return 0;
}
