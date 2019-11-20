//Procesory danych json w postaci zmiennych w main.
#include "fasada.hpp"
#include "processor_get.h"
#include "processor_set.h"
#include "processor_add.h"
#include "processor_del.h"
#include "processor_ren.h"
#include "processor_ls.h"
#include "processor_dfs.h"
#include "processor_find.h"
#include "processor_check_file.h"

namespace fasada
{

void register_processors(bool WithWriters)
{
    static fasada::processor_get  GET;//Istnieje co najmniej jedna zmienna kazdego typu
    static fasada::processor_dfs  DFS;//--//---
    static fasada::processor_ls   LST; //--//---
    static fasada::processor_find FIND;//--//---
    static fasada::processor_check_file CHECK;//--//---

    static fasada::processor_ls  Def("default");//ale mozna robić aliasy
    static fasada::processor_ls  Dir("dir");
    static fasada::processor_ls  Lst("lst");
    static fasada::processor_dfs Tree("tree");

    if(WithWriters)
    {
        static fasada::processor_set  SET;//Istnieje co najmniej jedna zmienna kazdego typu
        static fasada::processor_add  ADD;//--//---
        static fasada::processor_del  DEL;//--//---
        static fasada::processor_ren  REN;//--//---
    }
}

}
