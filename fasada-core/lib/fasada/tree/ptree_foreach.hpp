#ifndef PTREE_FOREACH_HPP
#define PTREE_FOREACH_HPP

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <string>
#include <queue>
#include <stack>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/numeric/conversion/cast.hpp> /// Konwersja na void*
#include <boost/lexical_cast.hpp>

namespace fasada
{

using boost::property_tree::ptree;

/// Typ predykatowy:
/// Ma do dyspozycji pełną ścieżkę do aktualnego węzła/liścia std::string key/k
/// oraz sam węzeł ptree& tree/t
/// No więc i dane węzła tree.data() i jak konieczne to wszystko co poniżej.

using pred  = const std::function<bool (ptree& tree,std::string key)>;//Typ predykatu
using predc = const std::function<bool (const ptree& tree,std::string key)>;//Typ predykatu dla stałego ptree

/// Użyteczne predykaty
inline bool  never(const ptree& t,std::string k){ return false; }//Predykat "zawsze nie"
inline bool always(const ptree& t,std::string k){ return true;  }//Predykat "zawsze tak"
inline bool  print(const ptree& t,std::string k){ //Drukuj i zwracaj true - Pomocny predykat testowy
                                                 std::cout<< k <<":\t";
                                                 std::cout<< t.data() <<" ; "<<std::endl;
                                                 return true; }

/// Funkcja odwiedzająca takie gałęzie, w których "filter" zwraca true
/// Jeśli zwróci false to cała gałąź jest pomijana "od góry"
inline
void for_true_branches(const ptree &tree,//Korzen drzewa w wersji const!
                         std::string key,  //Sciezka do węzła - niekonieczna niby, ale zwykle przydatna
                         predc& filter,    //Predykat filtrujący
                         std::string separator="." //Separator składowych klucza
              );

/// Wersja powyższej umożliwiająca w funkcji filtra modyfikacje drzewa
inline
void for_true_branches(ptree &tree,//Korzen drzewa w wersji const!
                         std::string key,  //Sciezka do węzła - niekonieczna niby, ale zwykle przydatna
                         pred& filter,    //Predykat filtrujący
                         std::string separator="." //Separator składowych klucza
              );

/// Funkcja ODWIEDZAJĄCA wszystkie wezly i liscie drzewa ptree
/// Przechodzi wszystkie elementy, wykonując "filter", ale
///     jeśli "filter" zwróci "true" to
///          wykonuje "before"
///             potem schodzi rekurencyjnie, a po powrocie z rekurencji
///               jeśli "before" zwróciło "true" to
///                      wykonuje "after"
///
inline              //TODO - a może for_each_node ???
void foreach_node(const ptree &tree,//Korzen drzewa w wersji const!
                    std::string key,//Sciezka do węzła - niekonieczna niby, ale zwykle przydatna
                    predc& filter,   //Predykat wykonywany zawsze
                    predc& before=never,//Predykat wykonywany gdy pierwszy zwróci true - przed iteracją dzieci
                    predc& after=never,//Predykat wykonywany gdy poprzedni zwróci true - po iteracji dzieci
                    std::string separator="." //Separator składowych klucza
            );

/// Tu są możliwe zmiany w drzewie, ale...
/// UWAGA!!!
/// W miarę bezpiecznie można to zrobić tylko w funkcji „after”,
/// w funkcji "filter" i "before" tylko pod warunkiem  
/// starannego posprzątania po sobie. Zwłaszcza uważac na usuwanie!
///
inline
void foreach_node(ptree &tree,      //Korzen drzewa
                    std::string key,//Sciezka do węzła - niekonieczna niby ale zwykle przydatna
                    pred& filter,   //Predykat wykonywany zawsze
                    pred& before=never,//Predykat wykonywany gdy pierwszy zwróci true - przed iteracją dzieci
                    pred& after=never,//Predykat wykonywany gdy poprzedni zwróci true - po iteracji dzieci
                    std::string separator="." //Separator składowych klucza
            );

/// Numeracja nienazwanych "dzieci"
/// Funkcja nadaje kolejne numery wszystkim nienazwanym
/// dzieciom każdego węzła
///
inline void   insert_numbers(ptree& pt);

/// OBSŁUGA "LINKU SYMBOLICZNEGO" DO RODZICA
inline void   insert_ups(ptree& pt);//wstawia do ptree symboliczne linki do rodziców
inline void   delete_ups(ptree& pt);//usuwa z ptree symboliczne linki do rodziców
inline ptree* get_parent(ptree& pt);//daje rodzica danego wezla ptree
                                    //Odczytuje i konwertuje

/// Funkcja użytkowa - drukowanie wszystkiego na strumień
inline void print_all_to(const ptree& pt,
                         std::ostream& out=std::cout,
                         std::string Separator="." //Separator składowych klucza
            ); //Drukuje wszystko

//IMPLEMENTACJA:
//=====================

inline void for_true_branches(const ptree &tree,//Korzen drzewa w wersji const!
                         std::string key, //Sciezka do węzła - niekonieczna niby, ale zwykle przydatna
                         predc& filter,
                         std::string separator)  //Predykat filtrujący
{
    std::string nkey;

    if (!key.empty())
    {
        nkey = key + separator; // Selected separator
    }

    if(filter(tree,key))// FILTR
    {
        auto end = tree.end();// REKURENCJA
        for (auto it = tree.begin(); it != end; ++it)
        {
            for_true_branches(it->second, nkey + it->first ,filter,separator);
        }
    }
}

inline void for_true_branches(ptree &tree,//Korzen drzewa w wersji const!
                         std::string key, //Sciezka do węzła - niekonieczna niby, ale zwykle przydatna
                         pred& filter,
                         std::string separator)  //Predykat filtrujący
{
    std::string nkey;

    //std::cerr<<separator;
    if (!key.empty())
    {
        nkey = key + separator; // Selected separator
    }

    if(filter(tree,key))// FILTR
    {
        auto end = tree.end();// REKURENCJA
        for (auto it = tree.begin(); it != end; ++it)
        {
            for_true_branches(it->second, nkey + it->first ,filter,separator);
        }
    }
}

// foreach_node(...) to ogólny algorytm robienia czegoś z drzewami ptree
// NIEZALEŻNIE OD FILTRU ODWIEDZA WSZYSTKIE ELEMENTY DRZEWA
// (Dobry do lambd, ale jeszcze nie jako template)
void foreach_node(const ptree &tree, std::string key, predc& filter, predc& before/*=never*/, predc& after/*=never*/,std::string separator)
{
    std::string nkey;
    bool do_after=false;
    //std::cerr<<separator;
    if (!key.empty())
    {
        nkey = key + separator; // Selected separator
    }

    if(filter(tree,key))// FILTR ZAWSZE
        do_after=before(tree,key);// Jak filtr OK czyli 'true'

    auto end = tree.end();// REKURENCJA TEŻ ZAWSZE
    for (auto it = tree.begin(); it != end; ++it)
    {
        foreach_node(it->second, nkey + it->first ,filter,before,after,separator);
    }

    if(do_after)// Jak before zwróciło "true"
        after(tree,key);
}

// Wersja foreach_node z możliwością modyfikacji
void foreach_node(ptree &tree, std::string key, pred& filter, pred& before/*=never*/, pred& after/*=never*/,std::string separator)
{
    std::string nkey;
    bool do_after=false;
    //std::cerr<<separator;
    if (!key.empty())
    {
        nkey = key + separator; // Selected separator
    }

    if(filter(tree,key))
        do_after=before(tree,key);

    auto end = tree.end();
    for (auto it = tree.begin(); it != end; ++it)
    {
        foreach_node(it->second, nkey + it->first ,filter,before,after,separator);
    }

    if(do_after)
        after(tree,key);
}

// Brak nazw węzłów pochodzących z tablic json'a jest bardzo
// niewygodny, gdy trzeba się do nich dostać pojedynczo.
//
void insert_numbers(ptree& pt)
// Funkcja numeruje wszystkie nienazwane dzieci każdego węzła
{
    foreach_node(pt,"",
        [](ptree& t,std::string k)
        {
            unsigned id=1;//Trzeba nadać id węzłom o pustych nazwach?

            while(1)
            {
                //base on https://stackoverflow.com/questions/45262602/c-boost-ptree-rename-key
                auto fp=t.find("");
                if(fp==t.not_found()) break;
                ptree::iterator it = t.to_iterator(fp);
                auto newname=boost::lexical_cast< std::string >(id++);
                t.insert(it, make_pair(newname, it->second));
                t.erase(it);
            }
            //return id>1?true:false;//DEBUG
            return true;
        }
    );
}

// Potrzebujemy prostej metody znajdowania rodzica wezla.
// Niestety ptree tego nie przewiduje:
// https://stackoverflow.com/questions/45366768/getting-boost-property-tree-parent-node
// https://stackoverflow.com/questions/45255463/c-boost-ptree-relative-key
// Pomysl jest taki zeby do listy dzieci wezla wstawiac wezel rodzica, tak jak to sie
// robi w strukturze katalogow filesystemu ("..")
// Ale raczej to nie może być normalny wskaźnik do 'node' bo się drzewo "scyklizuje"(?)
//
void insert_ups(ptree& pt)
//wstawia symboliczne linki do rodziców
{
    std::stack<ptree*> pointers;//Jak sensownie zachować adresy rodzica? stack? TODO?
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

void delete_ups(ptree& pt)
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
//daje "zdeszyfrowany" link do rodzica
{
    boost::optional<ptree&> uplink=pt.get_child_optional("^");
    if(uplink)
    {
        std::string sptr=uplink->data();
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


        return (ptree*)result;//WIEMY ZE TO JEEEST TO!
    }
    else
    return nullptr;
}

void print_all_to(const ptree& pt,std::ostream& out/*=std::cout*/,std::string Separator/*="."*/)
// drukuje wszystko na zadany strumień
{
    foreach_node(pt,"root",always,
            [&out](const ptree& t,std::string k)
            {
                out<< k <<":\t";
                out<< t.data() <<" ; "<<std::endl;
                return false;//blokuje wywołanie "after", które i tak jest "never"
            },
            never,Separator
        );
}

}//namespace "fasada"

#endif // PTREE_FOREACH_HPP
