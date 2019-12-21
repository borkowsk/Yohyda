/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///
#define UNIT_IDENTIFIER "processor_ls"
#include "fasada.hpp"
#include "processor_ls.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <iostream>

namespace fasada
{

processor_ls::processor_ls(const char* name):
    tree_processor(READER,name)
{

}

processor_ls::~processor_ls()
{}

void processor_ls::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+"IS REALLY NOT A WRITER!"));
}

void processor_ls::_implement_action_panel(ShmString& o,URLparser& request)
//Górny i dolny panel akcji dotyczących całej listy
{
    std::string fullpath=request.getFullPath();
    o+="\n";//For cleaner HTML code

    if(writing_enabled())
    {
        o+=getActionLink(fullpath+"?add&html","ADD!","?add&html");
        o+=+"&nbsp;&nbsp; "+getActionLink(fullpath+"?ren&html","REN!","Rename subnode");
        o+=+"&nbsp;&nbsp; "+getActionLink(fullpath+"?del&html","DEL!","Delete subnode")+"&nbsp;&nbsp; ";
    }
    o+=getActionLink(fullpath+"?find&html","FIND","Find")+"&nbsp;&nbsp; ";

    if(longformat)
    {
        o+=getActionLink(fullpath+"?dfs&html&long","TREE","Print as tree")+"&nbsp;&nbsp; ";
        o+=getActionLink(fullpath+"?ls&html","LSS","List as short content of "+request["&path"])+"&nbsp;&nbsp; ";
    }
    else
    {
        o+=getActionLink(fullpath+"?dfs&html","TREE","Print as tree")+"&nbsp;&nbsp; ";
        o+=getActionLink(fullpath+"?ls&html&long","LSL","List as long content of "+request["&path"])+"&nbsp;&nbsp; ";
    }

    o+=getActionLink(request.getParentPath()+"?"+request["&query"],HTMLBack,"Go back")+"&nbsp;&nbsp; ";

    if(request["&path"]=="/")
        o+=getActionLink("http://"+request["&domain"]+":"+request["&port"]+"/!!!!","SHUTDOWN!!!!","Close this www server!");
    else
        o+=getActionLink("http://"+request["&domain"]+":"+request["&port"]+"/?ls&html&long","TOP","Top of the tree");
    o+="\n";//For cleaner HTML code
}

void processor_ls::_implement_node_panel(ShmString& o,const pt::ptree& node,const std::string& fullpath,URLparser& request)
//Panel akcji dotyczących danego węzła
{
    //const std::string& data=node.data();
    o+=getNodePanel(node,fullpath,request);
    // something more?
    //...
}

//Problem indeksowania tablic JSONów w ptree - kłopotliwy bardzo
//https://stackoverflow.com/questions/48407925/boostproperty-treeptree-accessing-arrays-first-complex-element
//Rozwiązany na poziomie serwera fasady za pomoca wstawienia kolejnych liczb po wczytaniu
void processor_ls::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    unsigned counter=0;
    bool     html=request.asHTML();

    longformat=request.asLONG();//longformat jest prywatne

    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getPageHeader(request["&path"]);
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    if(top.size()>0)
    {
        if(html) _implement_action_panel(o,request);
        o+="<BR><BR>\n";
        std::string parentpath=request.getFullPath();
        if( *(--parentpath.end())!='/' )
            parentpath+="/";

        o+=(longformat & html?"\n<UL>\n":"\n");

        for(auto p:top)
        {
            if(html)
            {
                std::string fullpath=parentpath+std::string(p.first.data());
                o+=std::string(longformat?"<LI>":"")
                        +" <A href=\""+fullpath+"?"+request["&query"]+"\">"
                        +"<B class=fasada_path>'"
                        +std::string(p.first.data())
                        +"'</B></A>"
                        +" <A href=\""+fullpath+"?get&html&long\"> : <I class=\"fasada_val\">'"
                        +std::string(p.second.data())
                        +"' </I></A> ";
                _implement_node_panel(o,p.second,fullpath,request);
                if(longformat)
                    _implement_attributes(o,p.second,request,p.first.data());
            }
            else
                o+=std::string(p.first.data())+std::string(" : ")+std::string(p.second.data());

            counter++;

            if(longformat)
                o+=(html?"\n":";\n");
            else
                o+=(html?"&nbsp; ":";\t");
        }

        if(longformat & html) o+="</UL>\n";
            else o+="\n";

        o+=boost::lexical_cast<val_string>(counter);

        if(html && counter>10)
        {
            o+="<BR><BR>\n";
            _implement_action_panel(o,request);
            o+="\n";//For cleaner HTML code
        }
    }
    else
    {
        if(html)
        {
            std::string fullpath=request.getFullPath();
            o+=std::string(longformat?"<LI>":"")
                    + "<B class=fasada_path>'" + request["&path"] + "'</B> : "
                   // + "<I class=\"fasada_val\">'"+top.data()+"'</I> ";
                    +" <A href=\""+fullpath+"?get&html&long\"> : <I class=\"fasada_val\">'"
                    +top.data()
                    +"'</I></A> ";
            _implement_node_panel(o,top,fullpath,request);
            if(longformat)
                _implement_attributes(o,top,request);
            o+="&nbsp;&nbsp; "+getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack,"Go back");
            o+="\n";//For cleaner HTML code
        }
        else
        o+=" NO SUBNODES ";
    }
    if(html) o+=getPageClosure(_compiled);
}

//<LI>
//<A href="http://localhost:8000/TimelineOfTheEarth/photos_and_videos/album/27.json/photos/1/uri?ls&html&long">
//<B class=fasada_path>'uri'</B></A>
//<A href="http://localhost:8000/TimelineOfTheEarth/photos_and_videos/album/27.json/photos/1/uri?get&html&long"> :
//<I class="fasada_val">'photos_and_videos/Permianmammallikereptiles_hdLt_Gppqg/534246_556509764412644_1704957149_n_556509764412644.jpg'</I></A>
//&nbsp;
//<A HREF="./photos_and_videos/Permianmammallikereptiles_hdLt_Gppqg/534246_556509764412644_1704957149_n_556509764412644.jpg"
//
//   http://localhost:8000/TimelineOfTheEarth/photos_and_videos/Permianmammallikereptiles_hdLt_Gppqg/
//
//class="fasada_action">
//see</A>&nbsp;
//<A HREF="http://localhost:8000/TimelineOfTheEarth/photos_and_videos/album/27.json/photos/1/uri?set&html" class="fasada_action">=</A>



}//namespace "fasada"
