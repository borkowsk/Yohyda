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
{}

processor_find::~processor_find()
{}

//default HTML form for this processor
std::string processor_find::Form=
        "<form action=\"$fullpath!$proc\" class=\"fasada_form\">\n"
        "<input name=\"ready\"   type=\"hidden\"             value=\"$is_ready\" >\n"
        "\n<BR>SUBPATH:     "
        "<input name=\"subpath\" type=\"$input_of_subpath\" size=\"$size_of_subpath\" value=\"$subpath\">"
        "\n<BR>FIELD NAME:  "
        "<input name=\"field\"   type=\"$input_of_field\"   size=\"$size_of_field\"   value=\"$field\">"
        "\n<BR>FIELD VALUE: "
        "<input name=\"value\"   type=\"$input_of_value\"   size=\"$size_of_value\"   value=\"$value\">"
        "\n<BR>WILL BE FIND IN <B class=fasada_path>'$path'</B><BR>\n"
        "<input type=\"submit\" value=\"OK\">\n"
        "</form>";

void processor_find::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    bool html=request["html"]!="false";
    unsigned    noc=top.size();//czy ma jakieś elementy składowe?
    std::string fullpath=request.getFullPath();

    if(noc<2)
    {
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" HAS NOTHING TO DO WITH NODE "+request["&path"]));
    }

    if(request["ready"]=="true")//<FORM> is filled completelly
    {
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+": COMPLETE FORM SHOULD BE PROCESSED WITH WRITING ENABLED."));
    }
    else
    {
        if(!html)
            throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS CANNOT PREPARE FORM OTHER THAN HTML!"));

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
            std::string replacer=(request["subpath"]+"\"><I>"+request["subpath"]+"</I>");
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
            std::string replacer=(request["field"]+"\"><I>"+request["field"]+"</I>");
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
            std::string replacer=(request["value"]+"\"><I>"+request["value"]+"</I>");
            boost::replace_all(ReadyForm,"$value\">",replacer);
            boost::replace_all(ReadyForm,"$size_of_value","1");
        }

        o+=ReadyForm;
        o+=getHtmlClosure();
    }
}

void processor_find::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    if(request["ready"]!="true")//<FORM> is NOT filled completelly
    {
        _implement_read(o,top,request);
    }
    else
    {
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS REALLY NOT IMPLEMENTED!"));
    }
}

}//namespace "fasada"
