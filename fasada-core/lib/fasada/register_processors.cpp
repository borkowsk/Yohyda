/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
/// See CURRENT licence file!
///

//Procesory danych json w postaci STATYCZNYCH zmiennych lokalnych autorejestrowanych w konstruktorach
#include "fasada.hpp"
#include "processor_get.h"
#include "processor_set.h"
#include "processor_add.h"
#include "processor_del.h"
#include "processor_ren.h"
#include "processor_ls.h"
#include "processor_dfs.h"
#include "processor_find.h"
#include "processor_info.h"
#include "processor_check_file.h"
#include "loader_txt.h"
#include "loader_csv.h"
#include "loader_json.h"
#include "view_csv.h"
#include "save_as_csv.h"
#include "save_as_txt.h"
#include "save_as_json.h"
#include "save_as_xml.h"

namespace fasada
{

void register_processors(bool WithWriters)
{
    static fasada::processor_info     INFO;//Istnieje co najmniej jedna zmienna każdego typu
    static fasada::processor_get      GET; //--//---
    static fasada::processor_dfs      DFS; //--//---
    static fasada::processor_ls       LST; //--//---
    static fasada::processor_find     FIND;//--//---
    static fasada::view_csv           VIEW_CSV;
    static fasada::processor_check_file CHECK_FILE;//--//---
    //static fasada::save_as_txt        SAVE_TXT;//still TODO
    //static fasada::save_as_csv        SAVE_CSV;//still TODO
    static fasada::save_as_json       SAVE_JSON;
    static fasada::save_as_xml        SAVE_XML;
                                                            //ale można też robić aliasy
    static fasada::processor_ls       Def("default");
    static fasada::processor_ls       Dir("dir");
    static fasada::processor_ls       Lst("lst");
    static fasada::processor_dfs     Tree("tree");
    static fasada::processor_info    FasadaInfo("fasada_info");
    static fasada::save_as_json      saveJson("saveJson");

    if(WithWriters)
    {
        static fasada::processor_set  SET;//Istnieje co najmniej jedna zmienna każdego typu
        static fasada::processor_add  ADD;//--//---
        static fasada::processor_del  DEL;//--//---
        static fasada::processor_ren  REN;//--//---
        static fasada::loader_txt     TXT;//--//---
        static fasada::loader_csv     CSV;//--//---
        static fasada::loader_json   JSON;//--//---
                                                                //ale można zrobić aliasy
        static fasada::loader_txt     Txt("Txt");//--//---
        static fasada::loader_csv     Csv("Csv");//--//---
        static fasada::loader_json   Json("Json");//--//---
        static fasada::loader_csv    LinkedInCsv("LinkedInCsv");
        static fasada::loader_json   TwitterJson("TwitterJson");//Json'y z Twittera mają takie cosie na początku "window.YTD.tweet.part0 ="
                                                                //które wywalają boostowy parser, ale już je usuwamy i wpisujemy jako atrybut _lead
    }
}

}
