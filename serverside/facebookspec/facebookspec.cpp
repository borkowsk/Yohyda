#include "facebookspec.h"
#include "processor_facebookjson.h"
#include "tree/ptree_foreach.hpp"
#include <boost/locale.hpp>

using namespace fasada;

namespace facebook
{


void recode_facebook_timestamps(pt::ptree& start)
{
     foreach_node(start,"root",always,always,
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
    foreach_node(start,"root",always,
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
{
    std::cerr<<"inserting numbers..."<<std::endl;
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
    static processor_facebookJson FacebookJson;
}

} //namespace facebook

