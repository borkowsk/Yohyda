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
#include "saver_processor.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <iostream>

namespace fasada
{

saver_processor::saver_processor(const char* name):
    form_processor(name)
{}

saver_processor::~saver_processor()
{}

//default HTML form for this processor
std::string saver_processor::Form=
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

void saver_processor::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)    //TODO
//Wspólny formularz dla SAVERÓW!
{
    bool        html=request.asHTML();
    unsigned    noc=top.size();//czy ma jakieś elementy składowe?
    std::string fullpath=request.getFullPath();

    if(noc<1)
    {
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" HAS NOTHING TO DO WITH LEAF NODE "+request["&path"]));
    }

    if(request["ready"]=="true")//<FORM> is filled completelly
    {
        //_implement_/*something*/(o,top,request);
    }
    else
    {
        if(!html)
            throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT PREPARE FORM OTHER THAN HTML!"));

        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(fullpath)+"\n";
        std::string ReadyForm=Form;
/*
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
*/
        o+=ReadyForm;
        o+=getHtmlClosure();
    }
}

void saver_processor::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"
