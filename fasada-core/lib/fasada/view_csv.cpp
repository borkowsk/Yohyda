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

// used to split the file in lines
static const boost::regex linesregx("\\r\\n|\\n\\r|\\n|\\r");

// used to split each line to tokens, assuming ',' as column separator
static const boost::regex fieldsregx(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");

namespace fasada
{

typedef std::vector<std::string> CsvRow;

std::vector<CsvRow> parse(const char* data, unsigned int length)
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
    //IMPLEMENTATION
    //...
}


}//namespace "fasada"
