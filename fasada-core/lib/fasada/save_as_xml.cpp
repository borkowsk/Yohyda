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
#include "save_as_xml.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
//See: https://www.technical-recipes.com/2014/using-boostproperty_tree/
//and: https://stackoverflow.com/questions/18875437/writing-more-complex-than-trivial-xml-with-boost-property-tree

namespace fasada
{

save_as_xml::save_as_xml(const char* name):
    saver_processor(name)
{}

save_as_xml::~save_as_xml()
{}

void save_as_xml::_implement_write(ShmString& o,pt::ptree& top,URLparser& request)
{
     bool        html=request.asHTML();
     if(html)
     {
        o+=ipc::string(EXT_PRE)+"htm\n";
        o+=getHtmlHeaderDefaults(request.getFullPath())+"\n<PRE>\n";
     }

     std::string discPath=_make_path(".xml",request);

     o+="SAVING INTO: \n";
     o+=discPath;

     // Then write the Boost property tree to an output XML file
     pt::xml_writer_settings<std::string> w( ' ', 2 );
     pt::write_xml( discPath, top,std::locale(), w );

     o+="\nDONE";//Jesli nie ma wyjatku...

     if(html)
     {
        o+="\n</PRE>\n";
        o+=getActionLink(request.getFullPath()+"?ls&html&long","LSL","List as long content of "+request["&path"])+"&nbsp;&nbsp; ";
        o+=getHtmlClosure(_compiled);
     }
}

}//namespace "fasada"
