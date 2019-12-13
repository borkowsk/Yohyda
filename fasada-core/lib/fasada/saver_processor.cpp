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


std::string saver_processor::_make_path(const std::string& ext,URLparser& request)
{
    std::string prepPath=request["targetpath"];

    if(prepPath=="" || prepPath=="...")
            prepPath=request["&private_directory"]+request["&path"];

    if(ext!="")
    {
      if(prepPath.find(ext,0)==prepPath.npos)
          prepPath+=ext;
    }

    return prepPath;
}

//default HTML form for this processor
std::string saver_processor::Form=
        "<form action=\"${fullpath}!${proc}\" class=\"fasada_form\">"
        "\n<input name=\"html\"   type=\"hidden\" >"
        "\n<input name=\"long\"   type=\"hidden\" >"
        "\n<input name=\"ready\"  type=\"hidden\"  value=\"true\" >" ///value=\"$is_ready\" >"
        "\n&#x26C1; &#x25c1; &#x25c1; &#x25c1; &#x25c1; &#x25c1; &#x25c1;  &#x26C2;"    ///&#x25c1;
        "\n<BR><input name=\"targetpath\" type=\"input\" size=\"${size_of_targetpath}\" "
        " value=\"${targetpath}\">"
        "\n&nbsp;<input name=\"force\"  type=\"checkbox\" >FORCE?"
        "\n<BR>THIS FILE WILL BE REPLACED BY DATA EXISTING BELOW"
        "<BR><q>${fullpath}</q>"
        "\n<BR><input type=\"submit\" value=\"YES\" >"
        "\n&nbsp;<input type=\"button\" value=\"CANCEL\" onclick=\"window.history.back();\" >"
        "\n&nbsp;<a class=\"fasada_action\" href=\"${fullpath}?ls&html&long\" >LSL</A>&nbsp;&nbsp; "
        "\n&nbsp;<a class=\"fasada_action\" href=\"${fullpath}?dfs&html&long\" >TREE</A>&nbsp;&nbsp; \n"
        "</form>";
        ;

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

        request["targetpath"]=get_property(top,"_source",_make_path("",request));//... default target
        request["size_of_targetpath"]=STR_WIDTH_MAX_OF_FIELD;
        std::string ReadyForm=replace_all_variables(_get_form_template(),request);
        o+=ReadyForm;

        o+=getHtmlClosure(_compiled);
    }
}

void saver_processor::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS CURRENTLY NOT IMPLEMENTED AS A WRITER!"));
    insert_property(top,"_savetimestamp",std::to_string(time(NULL)));//MUST BE IN IMPLEMENTATIONS !TODO
}

}//namespace "fasada"
