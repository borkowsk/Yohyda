/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///
#define UNIT_IDENTIFIER "processor_info"
#include "fasada.hpp"
#include "processor_info.h"
#include <boost/lexical_cast.hpp>

namespace fasada
{

processor_info::processor_info(const char* name):
    tree_processor(READER,name)
{

}

processor_info::~processor_info()
{}

void processor_info::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    unsigned    noc=top.size();
    bool     longer=request.asLONG();
    bool       html=request.asHTML();

    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"]);
        o+="<H2>INFO UNIT IN HTML MODE</H2><BR>\n";
        if(longer) o+="<P>";
    }
    else
    {
        if(longer) o+="\n\n INFO UNIT IN TXT MODE\n\n";
        else o+=ipc::string(EXT_PRE)+"txt\n";
    }

    std::string value=top.get_value<std::string>();
    std::string fullpath=request.getFullPath();
    std::string parepath=request.getParentPath();

    if(request["verbose"]=="true" || longer)
    {
        if(html)
        {
            if(longer)
            {
                value=preprocessIntoHtml(value);//Preprocess links and other markers into HTML tags.
            }

            o+=std::string("<B class=fasada_path>'")+request["&path"]+ "'</B> :\n<BR>"+"<I class=\"fasada_val\">'" + value + "'</I><BR>\n";
            o+=std::string("<B class=fasada_path>fullpath'</B> :\n<BR><I class=\"fasada_val\">'") + fullpath + "'</I><BR>\n";
            o+=std::string("<B class=fasada_path>parentpath'</B> :\n<BR><I class=\"fasada_val\">'") + parepath + "'</I><BR>\n";
        }
        else
        {
            o+="'"+request["&path"]+ "' : '" + value + "'"+" ["+boost::lexical_cast<std::string>(noc)+"]\n";
            o+="'fullpath'   : '" + fullpath + "'\n";
            o+="'parentpath' : '" + parepath + "'\n";
        }
    }
    else
    {
        if(html) o+="<PRE>\n";
        o+=request["&path"]+"\t:\t"+value + "\n";
        o+="fullpath\t:\t"   + fullpath   + "\n";
        o+="parentpath\t:\t" + parepath   + "\n";
        if(html) o+="</PRE>\n";
    }

    if(html) o+="<UL>";
    for(auto p:request)
    {
        if(html) o+="<LI>";
        o+=p.first.c_str();
        o+="\t:\t";
        o+=p.second.c_str();
        o+="\n";
    }
    if(html) o+="</UL>";

    if(html)
    {
        o+="<BR>"+getActionLink(parepath+"?ls&long&html",HTMLBack,"Go back");
        if(longer) o+="</P>"+getHtmlClosure(_compiled);
    }
}


void processor_info::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

}//namespace "fasada"
