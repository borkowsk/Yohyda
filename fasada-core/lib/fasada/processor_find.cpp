/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

#include "fasada.hpp"
#include "processor_find.h"
#include "tree/ptree_foreach.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <iostream>

namespace fasada
{

processor_find::processor_find(const char* name):
    tree_processor(WRITER_READER,name)
{

}

processor_find::~processor_find()
{}

//default HTML form for this processor
std::string processor_find::Form=
        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">"
        "\n<input name=\"html\"   type=\"hidden\" >"
        "\n<input name=\"ready\"   type=\"hidden\"   value=\"$is_ready\" >"
        "\n<BR>SUBPATH:     "
        "   <input name=\"subpath\" type=\"$input_of_subpath\"   size=\"$size_of_subpath\"   value=\"$subpath\">"
        "\n<BR>FIELD NAME: "
        "   <input name=\"field\"   type=\"$input_of_field\"   size=\"$size_of_field\"   value=\"$field\">"
        "\n<BR>FIELD VALUE: "
        "   <input name=\"value\"   type=\"$input_of_value\"   size=\"$size_of_value\"   value=\"$value\">"
        "\n<BR>WILL BE FIND IN <B class=fasada_path>'$path'</B> "
        "   <input type=\"submit\" value=\"OK\">"
        "\n<BR><a class=\"fasada_action\" href=\"$fullpath?ls&html&long\">LSL</A>&nbsp;&nbsp; "
        "\n<a class=\"fasada_action\" href=\"$fullpath?dfs&html&long\">TREE</A>&nbsp;&nbsp; \n" ///TODO Not in the form!
        "</form>";

void processor_find::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    bool        html=request.asHTML();
    unsigned    noc=top.size();//czy ma jakieś elementy składowe?
    std::string fullpath=request.getFullPath();

    if(noc<1)
    {
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" HAS NOTHING TO DO WITH NODE "+request["&path"]));
    }

    if(request["ready"]=="true")//<FORM> is filled completelly
    {
        _implement_substring_find(o,top,request);
    }
    else
    {
        if(!html)
            throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT PREPARE FORM OTHER THAN HTML!"));

        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(fullpath)+"\n";
        std::string ReadyForm=Form;
        boost::replace_all(ReadyForm,"$proc",procName);
        boost::replace_all(ReadyForm,"$fullpath",fullpath);
        boost::replace_all(ReadyForm,"$path",request["&path"]);

        if( (request.find("subpath") != request.end() && request["subpath"]!="" )
        &&  (request.find("field") != request.end() && request["field"]!="" )
        &&  (request.find("value") != request.end() && request["value"]!="" )
                )
        {
            boost::replace_all(ReadyForm,"$is_ready","true");
        }

        ///<input name="subpath" type="$input_of_subpath"  value="$subpath" size="$size_of_subpath">
        if( request.find("subpath") == request.end() || request["subpath"]=="" )
        {
            boost::replace_all(ReadyForm,"$input_of_subpath","text");
            boost::replace_all(ReadyForm,"$subpath",STR_DEFAULT_FILTER);
            boost::replace_all(ReadyForm,"$size_of_subpath",STR_DEFAULT_LEN_OF_SUBPATH);
        }
        else
        {
            boost::replace_all(ReadyForm,"$input_of_subpath","hidden");
            std::string replacer=(request["subpath"]+"\"><I class=\"fasada_val\">"+request["subpath"]+"</I>");
            boost::replace_all(ReadyForm,"$subpath\">",replacer);
            boost::replace_all(ReadyForm,"$size_of_subpath","1");
        }

        ///<input name="field"   type="$input_of_field"    value="$field"   size="$size_of_field">
        if( request.find("field") == request.end() || request["field"]=="" )
        {
            boost::replace_all(ReadyForm,"$input_of_field","text");
            boost::replace_all(ReadyForm,"$field",STR_DEFAULT_FILTER);
            boost::replace_all(ReadyForm,"$size_of_field",STR_DEFAULT_LEN_OF_NAME);
        }
        else
        {
            boost::replace_all(ReadyForm,"$input_of_field","hidden");
            std::string replacer=(request["field"]+"\"><I class=\"fasada_val\">"+request["field"]+"</I>");
            boost::replace_all(ReadyForm,"$field\">",replacer);
            boost::replace_all(ReadyForm,"$size_of_field","1");
        }

        ///<input name="value"   type="$input_of_value"    value="$value"   size="$size_of_value">"
        if( request.find("value") == request.end() || request["value"]=="" )
        {
            boost::replace_all(ReadyForm,"$input_of_value","text");
            boost::replace_all(ReadyForm,"$value",STR_DEFAULT_FILTER);
            boost::replace_all(ReadyForm,"$size_of_value","24");
        }
        else
        {
            boost::replace_all(ReadyForm,"$input_of_value","hidden");
            std::string replacer=(request["value"]+"\"><I class=\"fasada_val\">"+request["value"]+"</I>");
            boost::replace_all(ReadyForm,"$value\">",replacer);
            boost::replace_all(ReadyForm,"$size_of_value","1");
        }

        o+=ReadyForm;
        o+=getHtmlClosure();
    }
}

void processor_find::_implement_action_panel(ShmString& o,URLparser& request)
//Górny i dolny panel akcji dotyczących całej listy
{
    std::string fullpath=request.getFullPath();
    o+="\n";//For cleaner HTML code
    o+=getActionLink(fullpath+"?find&html","FIND","Find subnodes")+"&nbsp;&nbsp; ";
    o+=getActionLink(fullpath+"?dfs&html&long","TREE","View tree in long format")+"&nbsp;&nbsp; ";
    o+=getActionLink(fullpath+"?ls&html&long","LSL","Lista as long")+"&nbsp;&nbsp; ";
    o+=getActionLink(fullpath+"?ls&html","LSS","Lista as short")+"&nbsp;&nbsp; ";
    o+=getActionLink(request.getParentPath()+"?ls&long&html",HTMLBack,"Go back");
    o+="\n";//For cleaner HTML code
}

void processor_find::_implement_substring_find(ShmString& o,const pt::ptree& top,URLparser& request)
{
    unsigned counter=0;
    bool defret=(request["return"]!="false");
    bool   html=request.asHTML();
    std::string fullpath=request.getFullPath();
    if( *(--fullpath.end())!='/' )
        fullpath+="/";

    std::string  subpath=request["subpath"];
    std::string    field=request["field"];
    std::string    value=request["value"];
    bool all_spaths=(subpath=="*");
    bool all_fields=(field=="*");
    bool all_values=(value=="*");

    if(html)//TYPE HEADER AND HTML HEADER
    {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request["&path"])+"\n";
        _implement_action_panel(o,request);
        o+="\n<UL>\n";//Zawsze jest UL?
    }
    else
        o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

    auto subpath_lambda= [subpath](const ptree& t,std::string k)
                                {
                                    auto found=k.find(subpath,0);
                                    return ( found != k.npos );
                                };

    auto field_lambda= [field](const ptree& t,std::string k)
                                {
                                    auto lslas=k.rfind('/');
                                    if(lslas==k.npos)
                                        return false;
                                    auto found=k.find(field,lslas);
                                    //std::cerr<<k<<" "<<lslas<<" "<<found<<std::endl;
                                    return ( found != k.npos );
                                };

    auto value_lambda= [value](const ptree& t,std::string k)
                                {
                                    auto found=t.data().find(value,0);
                                    return ( found != std::string::npos );
                                };

    auto print_lambda=[&o,defret,html,fullpath,&request,&counter,this](const ptree& t,std::string k)
                                {
                                    counter++;
                                    o+=(html?"<LI>":"* ");
                                    std::string pathk=k;
                                    if(html) o+="<B class=fasada_path><A HREF=\""
                                            +fullpath+pathk+"?ls&html&long\">";
                                    o+=pathk;
                                    if(html)
                                        o+="</A></B> : <I class=\"fasada_val\">' <A HREF=\""
                                         +fullpath+pathk+"?get&html&long\">'";
                                        else o+="' : '";
                                    o+=t.data();
                                    o+="'";
                                    if(html)
                                    {
                                        o+="</A></I>&nbsp; "+getNodePanel(t.data(),fullpath+pathk,request);
                                        if(t.data()=="")
                                        {
                                            auto pos=k.rfind('/');
                                            _implement_attributes(o,t,request,k.substr(pos+1));
                                        }
                                        o+="\n";
                                    }
                                    else o+="\n";

                                    return defret;
                                };

    auto and_lambda=[all_fields,field_lambda,all_values,value_lambda,print_lambda](const ptree& t,std::string k)
    {
        return (all_fields || field_lambda(t,k))
                &&
               (all_values || value_lambda(t,k))
                &&
                print_lambda(t,k);
    };

//  It would be easier, but compiler don't cooperate ;-) :-/
//      "error: operands to ?: have different types" ?????????
//  auto real_lambda=( all_spaths ? always_lambda : subpath_lambda );

    if(all_spaths)
        foreach_node( top,"",always, and_lambda, never, "/" );
    else
        foreach_node( top,"",subpath_lambda, and_lambda, never, "/" );

    if(html)
    {
        o+="</UL>";
        o+=boost::lexical_cast<val_string>(counter)+"<BR>\n";
        if(counter>10) _implement_action_panel(o,request);
        o+="\n"+getHtmlClosure();
    }
}

void processor_find::_implement_regex_find(ShmString& o,pt::ptree& top,URLparser& request)
//Called in _implement_write
{

}

void processor_find::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    if(request["ready"]!="true")//<FORM> is NOT filled completelly
    {
        _implement_read(o,top,request);
    }
    else
    {
        _implement_substring_find(o,top,request);
    }
}

}//namespace "fasada"
