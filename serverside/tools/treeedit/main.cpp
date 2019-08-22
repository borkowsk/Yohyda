//treeedit main.cpp
#include "memory_pool.h"
#include <iostream>

int main()
{
        std::cerr<<"Łącze ze wspólnym obszarem pamięci"<<std::endl;
        //To jest serwer odpowiedzialny za ten obszar pamięci
        facjata::MemoryPool MyMemPool;                            assert(MyMemPool.is_server()==false);

        std::pair<ShmString*, managed_shared_memory::size_type> res;

        res = MyMemPool.segm().find<ShmString>("TreeServerEmp");  assert(res.first!=nullptr);

        std::cerr<<"Recived: '"<<*res.first<<"' with size "<<res.second<<std::endl;  
              
        MyMemPool.segm().destroy<ShmString>("TreeServerEmp");
        std::cerr<<"TREEEDIT: Konczę prace"<<std::endl;
        return 0;
}
