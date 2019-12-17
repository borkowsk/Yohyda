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
#include "view_csv.h"
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp> //https://bravenewmethod.com/2016/09/17/quick-and-robust-c-csv-reader-with-boost/
#include <boost/algorithm/string/replace.hpp> ///https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
#include <boost/property_tree/ptree.hpp>
// See: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
// or : https://stackoverflow.com/questions/18365463/how-to-parse-csv-using-boostspirit

namespace fasada
{
using boost::property_tree::ptree;
typedef std::vector<std::string> CsvRow;

// used to split the file in lines
const boost::regex linesregx("\\r\\n|\\n\\r|\\n|\\r");

// used to split each line to tokens, assuming ',' as column separator
const boost::regex fieldsregx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");

std::vector<CsvRow> parse(const char* data, unsigned int length)
//This function failed when there is newline inside the quotation marks :-( TODO
{
    std::vector<CsvRow> result;

    // iterator splits data to lines
    boost::cregex_token_iterator li(data, data + length, linesregx, -1);
    boost::cregex_token_iterator end;

    while (li != end) {
        std::string line = li->str();
        ++li;

        // Split line to tokens
        boost::sregex_token_iterator ti(line.begin(), line.end(), fieldsregx, -1);
        boost::sregex_token_iterator end2;

        std::vector<std::string> row;
        while (ti != end2) {
            std::string token = ti->str();
            ++ti;
            row.push_back(token);
        }
        if (line.back() == ',') {
            // last character was a separator
            row.push_back("");
        }
        result.push_back(row);
    }
    return result;
}

view_csv::view_csv(const char* name):
    view_processor(name)
{}

view_csv::~view_csv()
{}

void view_csv::_implement_read(ShmString& o,const pt::ptree& top,URLparser& request)
{
    auto icontent=top.find("_raw_content");

    if(icontent!=top.not_found())
    {
        const ptree& content=icontent->second;
        const std::string& scontent=content.data();
        bool longer=request.asLONG();

        //REPLACEMENT & SPLISING
        std::string copied=scontent.c_str();
        boost::replace_all(copied, ","   , "\u200A," );
        boost::replace_all(copied, ",\""   , ",\" " );
        if(request.asHTML())
        {
            boost::replace_all(copied, " \"\"" , " <q>" );
            boost::replace_all(copied, "\"\""  , "</q>" );
        }
        else
        {
            boost::replace_all(copied, " \"\"" , " ''" );
            boost::replace_all(copied, "\"\""  , "''" );
        }
        auto decoded_content=parse(copied.c_str(),copied.length());

        //"PRINTING"
        if(request.asHTML())
        {
            o+=ipc::string(EXT_PRE)+"htm\n";//TYPE HEADER
            o+=getHtmlHeaderDefaults(request["&path"]);
            bool tabhead=true;
            o+=getActionLink(request.getFullPath()+"?"+procName+"&html"+(!longer?"&long":""),(longer?"Less":"More"),
                             (longer?"View in short format":"View in long format"))
                             +"&nbsp;&nbsp; "
              +getActionLink(request.getParentPath()+"?ls&html"+(longer?"&long":""),HTMLBack,"Go back")
              +"\n";

            o+="<TABLE class=fasada_table>\n";
            for(CsvRow& row:decoded_content)
            {
                if(tabhead)
                {
                    o+="<TR> ";
                    for(auto colhader:row)
                    {
                        o+="<TH>"+colhader+"</TH> ";
                    }
                    o+="</TR>\n";
                    tabhead=false;
                }
                else
                {
                    o+="<TR> ";
                    for(auto field:row)
                    {
                        if(longer) field=preprocessIntoHtml(field);
                        o+="<TD>"+field+"</TD> ";
                    }
                    o+="</TR>\n";
                }

            }
            o+="</TABLE>\n<BR>";
            o+="\n"+getHtmlClosure(_compiled);
        }
        else
        {
            if(!request.asLONG())
                o+=ipc::string(EXT_PRE)+"txt\n";//TYPE HEADER

            for(CsvRow& row:decoded_content)
            {
                for(auto field:row)
                {
                    o+=field+"\t;\t";
                }
                o+="\n";
            }
        }
    }
    else
    {
        throw(tree_processor_exception("PROCESSOR '"+procName+"' CAN WORK ONLY ON '_raw_content' FIELD!"));
    }
}


}//namespace "fasada"
