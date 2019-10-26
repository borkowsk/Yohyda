//Procesory danych json w postaci zmiennych w main.
#include "processor_get.h"
#include "processor_set.h"
#include "processor_add.h"
#include "processor_del.h"
#include "processor_ren.h"
#include "processor_ls.h"
#include "processor_dfs.h"

namespace fasada
{

void register_processors()
{
    static fasada::processor_get GET;//Istnieje conajmniej jedna zmienna kazdego typu
    static fasada::processor_set SET;//--//---
    static fasada::processor_add ADD;//--//---
    static fasada::processor_del DEL;//--//---
    static fasada::processor_ren REN;//--//---
    static fasada::processor_dfs DFS;//--//---
    static fasada::processor_ls  LS; //--//---

    static fasada::processor_ls  Def("default");//ale mozna robić aliasy
    static fasada::processor_ls  Dir("dir");
    static fasada::processor_dfs Tree("tree");
}

}
