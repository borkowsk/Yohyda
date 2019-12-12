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
#include "loader_processor.h"
#include <boost/lexical_cast.hpp>
#include <string>

namespace fasada
{

loader_processor::loader_processor(const char* name):
        form_processor(name)
{}

loader_processor::~loader_processor()
{}


//default HTML form for this processor
std::string loader_processor::Form=
        "<form action=\"${fullpath}!${proc}\" class=\"fasada_form\">"
        "\n<input name=\"html\"   type=\"hidden\" >"
        "\n<input name=\"long\"   type=\"hidden\" >"
        "\n<input name=\"force\"  type=\"hidden\"  value=\"true\" >"
        "\n<input name=\"ready\"  type=\"hidden\"  value=\"true\" >" ///value=\"$is_ready\" >"
        "\n<BR>REPLACE ALL CURRENT DATA BELOW"
        "<BR><q>${fullpath}</q>"
        "<BR>WITH DATA FROM FILE?"
        "\n<BR><input name=\"targetpath\" type=\"input\" size=\"${size_of_targetpath}\" "
        " value=\"${targetpath}\">"
        "\n<BR><input type=\"submit\" value=\"YES\" >"
        "\n&nbsp;<input type=\"button\" value=\"CANCEL\" onclick=\"window.history.back();\" >"
        "\n&nbsp;<a class=\"fasada_action\" href=\"${fullpath}?ls&html&long\" >LSL</A>&nbsp;&nbsp; "
        "\n&nbsp;<a class=\"fasada_action\" href=\"${fullpath}?dfs&html&long\" >TREE</A>&nbsp;&nbsp; \n"
        "</form>";
        ;

void loader_processor::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)   //TODO
//Wspólna funkcja formularza dla wszystkich loaderów
{
    bool        html=request.asHTML();
    if(!html)
        throw(tree_processor_exception("PTREE PROCESSOR "+procName+" CANNOT PREPARE FORM OTHER THAN HTML!"));

    o+=ipc::string(EXT_PRE)+"htm\n";
    o+=getHtmlHeaderDefaults(request.getFullPath())+"\n";

    request["targetpath"]=get_property(top,"_source",request["&private_directory"]+request["&path"]);//... default target
    request["size_of_targetpath"]=STR_WIDTH_MAX_OF_FIELD;
    std::string ReadyForm=replace_all_variables(_get_form_template(),request);
    o+=ReadyForm;

    o+=getHtmlClosure(); 
}

void loader_processor::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
    throw(tree_processor_exception("PTREE PROCESSOR "+procName+" IS NOT IMPLEMENTED AS A WRITER!"));
}

}//namespace "fasada"
