#ifndef PTREE_FOREACH_HPP
#define PTREE_FOREACH_HPP

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <string>
#include <queue>
#include <stack>
#include <boost/property_tree/ptree.hpp>
#include <boost/numeric/conversion/cast.hpp> /// Konwersja na void*

namespace fasada
{

using boost::property_tree::ptree;

/// Typ predykatowy:
/// Ma do dyspozycji pełną ścieżkę do aktualnego węzła/liścia std::string key/k
/// oraz sam węzeł ptree& tree/t
/// No więc i dane węzła tree.data() i wszystko co poniżej.

using pred = const std::function<bool (ptree& tree,std::string key)>;//Typ predykatu

inline
bool  never(ptree& t,std::string k){ return true; }//Predykat "zawsze nie"
inline
bool always(ptree& t,std::string k){ return true; }//Predykat "zawsze tak"

///Funkcja odwiedza wszytkie wezly i liscie drzewa ptree
/// UWAGA NA MODYFIKACJE DRZEWA!!!
/// W miarę bezpiecznie można to zrobić tylko w funkcji „after”
///
inline
void foreach_node(ptree &tree,      //Korzen drzewa
                    std::string key,//Sciezka do węzła - niekonieczna niby ale zwykle przydatna
                    pred& filter,   //Predykat wykonywany zawsze
                    pred& before=never,//Predykat wykonywany gdy pierwszy zwróci true - przed iteracją dzieci
                    pred& after=never);//Predykat wykonywany gdy poprzedni zwróci true - po iteracji dzieci

///Pomocny predykat testowy
bool  print(ptree& t,std::string k){ //Drukuj i zwracaj true
                                 std::cout<< k <<":\t";
                                 std::cout<< t.data() <<" ; "<<std::endl;
                                 return true; }

/// Funkcja testowa - drukowanie na stdout
int print_all(ptree& pt); //Drukuje wszystko

/// OBSŁUGA "LINKU SYMBOLICZNEGO" DO RODZICA
int insert_ups(ptree& pt);//wstawia do ptree symboliczne linki do rodziców
int delete_ups(ptree& pt);//usuwa z ptree symboliczne linki do rodziców
ptree* get_parent(ptree& pt);//daje rodzica danego wezla ptree
                            //Odczytuje i konwertuje

//IMPLEMENTACJA:
/// foreach_node(...) to ogólny algorytm robienia czegoś z elementami ptree
/// (Dobry do lambd, ale jeszcze nie jako template)
/// Przechodzi wszystkie elementy, wykonując "filter", ale
///     jeśli "filter" zwróci "true" to
///          wykonuje "before"
///             potem schodzi rekurencyjnie, a po powrocie z rekurencji
///               jeśli "before" zwróciło "true" to
///                      wykonuje "after"

void foreach_node(ptree &tree, std::string key, pred& filter, pred& before/*=never*/, pred& after/*=never*/)
{
    std::string nkey;
    bool do_after=false;

    if (!key.empty())
    {
      nkey = key + ".";  // separator!!! TODO?
    }

    if(filter(tree,key))
        do_after=before(tree,key);

    auto end = tree.end();
    for (auto it = tree.begin(); it != end; ++it)
    {
      foreach_node(it->second, nkey + it->first ,filter,before,after);
    }

    if(do_after)
        after(tree,key);
}

//Potrzebujemy prostej metody znajdowania rodzica wezla.
//Niestety ptree tego nie przewiduje:
// https://stackoverflow.com/questions/45366768/getting-boost-property-tree-parent-node
// https://stackoverflow.com/questions/45255463/c-boost-ptree-relative-key
//Pomysl jest taki zeby do listy dzieci wezla wstawiac wezel rodzica, tak jak to sie
//robi w strukturze katalogow filesystemu ("..")
//Ale raczej to nie moze byc normalny wskaznik   do node bo sie drzewo "scyklizuje"(?)
//
int insert_ups(ptree& pt)
//wstawia symboliczne linki do rodziców
{
    std::stack<ptree*> pointers;//Jak sensownie zachować adresy rodzica? stack? TODO!!!
    pointers.push(nullptr); //Root tez cos musi sobie wstawic.
                            //NULL symbolizuje brak wyzszej instancji

    foreach_node(pt,"//",
        [&pointers](ptree& t,std::string k)
        {
            pointers.push(&t);// Wklada swoj adres dla swoich dzieci
            return true;//Kontynuacja
        }
        ,
        always, //bedzie "after"
        [&pointers](ptree& t,std::string k)//Dodawanie dozwolone tylko po iteracji
        {
            pointers.pop();//Zdejmuje swoj adres
            t.put("^", pointers.top() );//Wstawia sobie adres rodzica jako TEKST 0xXXXXXXXX
            return true;
        }
    );

}

int delete_ups(ptree& pt)
//usuwa symboliczne linki do rodziców
{
    foreach_node(pt,"//",always,always,
        [](ptree& t,std::string k) //Usuwanie dozwolone tylko po iteracji
        {
            t.erase("^");
            return true;
        }
    );
}

ptree* get_parent(ptree& pt)
{
    boost::optional<ptree&> uplink=pt.get_child_optional("^");
    if(uplink)
    {
        std::string sptr=uplink->data();
        //void* vptr=(void*)boost::numeric_cast<unsigned long>(sptr);//TTTTo sie sypie przy kompilacji :-/
        /* Convert the provided value to a decimal long long */
        unsigned long long result;         
                                                                static_assert(sizeof(result)>=sizeof(ptree*),
                                                                "Nie spelnione:sizeof(result)>sizeof(ptree*)");
        char *eptr;

        result = strtoull(sptr.c_str(), &eptr, 16);

        if (result == 0)//Tymczasowa obsluga bledow
          {
              /* If a conversion error occurred, display a message and exit */
              if (errno == EINVAL)
              {
                  std::cerr<<"Conversion error occurred: "<< errno <<std::endl;
              }

              /* If the value provided was out of range, display a warning message */
              if (errno == ERANGE)
                  std::cerr<<"The value provided was out of range"<< errno <<std::endl;

             return nullptr;
          }


        return (ptree*)result;//WIEMY ZE TO JEEEST TOOO!
    }
    else
    return nullptr;
}

int print_all(ptree& pt)
{
    foreach_node(pt,"root",always,[](ptree& t,std::string k)
    {
        std::cout<< k <<":\t";
        std::cout<< t.data() <<" ; "<<std::endl;
        return false;//blokuje wywołanie "after"
    });
    return 1;
}

}//namespace "fasada"

#endif // PTREE_FOREACH_HPP
