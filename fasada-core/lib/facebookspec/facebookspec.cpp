/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

#include "facebookspec.h"
#include "loader_facebookjson.h"
#include "tree/ptree_foreach.hpp"
#include <boost/locale.hpp>

using namespace fasada;

namespace facebook
{

time_t find_minimal_timestamp(const pt::ptree& start)
{
    time_t min=0;
    for_true_branches(start,"",
        [&min](const ptree& t,std::string k)
        {
            if(k.find("timestamp",0)!=k.npos)
            {
               try
               {
                   time_t timestamp=boost::lexical_cast<time_t>(t.data());
                   if(timestamp>0)
                       if(timestamp<min || min==0)
                       {
                           min=timestamp;
                           //std::cout<<k<<":'"<<t.data()<<"'"<<std::endl;
                       }
               }catch(...)
               {/*JAK JAKIS ERROR TO NIE ROBI NIC*/
                    std::cout<<k<<":'"<<t.data()<<"' IGNORED CONVERSION ERROR!"<<std::endl;
               }
            }

            return true;//zchodzi w dół zawsze
        }
        );
    return min;
}

void insert_mintimestamps(pt::ptree& start)
{
    start.add_child("_used_decoders.insert_mintimestamps",ptree{});

    for_true_branches(start,"",
        [](ptree& t,std::string k)
        {
            unsigned id=1;//Trzeba nadać id węzłom o pustych nazwach, nie mającym timestampów
            bool flag=true;

            while(1)
            {
                //base on https://stackoverflow.com/questions/45262602/c-boost-ptree-rename-key
                auto fp=t.find("");
                if(fp==t.not_found())
                            break;//KONIEC PETLI

                ptree::iterator it = t.to_iterator(fp);
                time_t ident=find_minimal_timestamp(it->second);
                std::string newname;
                if(ident!=0)
                {
                    newname=boost::lexical_cast< std::string >(ident);
                    if(t.find(newname)!=t.not_found())//Już taki był!!!
                        newname=newname+"."+boost::lexical_cast< std::string >(id++);//uzupelniamy o index

                    flag=false; //tylko pierszy poziom tablic ma takie indeksy
                }
                else
                    newname=boost::lexical_cast< std::string >(id++);

                t.insert(it, make_pair(newname, it->second));
                t.erase(it);
            }

            return flag;//Jak już powpisywał to nie schodzi w dół!
        }
    );
}

void recode_facebook_timestamps(pt::ptree& start)
{
     start.add_child("_used_decoders.recode_facebook_timestamps",ptree{});

     foreach_node(start,"",always,always,
         [](ptree& t,std::string k)
         {
             if(k.find("timestamp",0)!=k.npos)
             {
                std::cout<<k<<":'"<<t.data()<<"'"<<std::endl;
                try
                {
                    struct tm  *timeinfo;
                    time_t timestamp=boost::lexical_cast<time_t>(t.data());
                    std::string tmp=t.data();
                    t.clear();
                    t.put("unix",tmp);
                    timeinfo = localtime (&timestamp);//http://www.cplusplus.com/reference/ctime/localtime/
                    std::string timeString=asctime(timeinfo);//alt.: http://www.cplusplus.com/reference/ctime/strftime/ ?
                    for(char& c:timeString) if(c=='\n') c=';';
                    t.put("asci",timeString);
                }catch(...)
                {/*JAK JAKIS ERROR TO NIE ROBI NIC*/}
             }
             return false;//nieistotne
         }
         );
}

void recode_facebook_pl_to_utf8(pt::ptree& start)
{
    start.add_child("_used_decoders.recode_facebook_pl_to_utf8",ptree{});
    foreach_node(start,"",always,
        [](ptree& t,std::string k)
        {
            //std::cout<< k <<":\t";
            std::string latin1_string=boost::locale::conv::from_utf(t.data(),"Latin1");
            t.data()=latin1_string;
            //std::cout<< t.data() <<" ; "<<std::endl;
            return false;//blokuje wywołanie "after", które i tak jest "never"
        }
        );
}

void call_recoders(pt::ptree& start,bool PL)
{   //na o+= a nie na cerr!!! TODO!!!
    std::cerr<<"inserting timestamps as table indexes for top level tables"<<std::endl;
    insert_mintimestamps(start);
    //else
    std::cerr<<"inserting consecutive numbers as table indexes..."<<std::endl;
    insert_numbers(start);
    std::cerr<<"recoding timestamps..."<<std::endl;
    recode_facebook_timestamps(start);
    if(PL)
    {
        std::cerr<<"recoding facebook pl codes to utf8..."<<std::endl;
        recode_facebook_pl_to_utf8(start);
    }
}

void register_processors()
{
    static loader_facebookJson FacebookJson;
}

} //namespace facebook

