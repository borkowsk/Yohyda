/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @footnote Technical mail: <fasada.wb@gmail.com>
/// @copyright 2019
/// @version 0.01
///
///  See CURRENT licence file!
///

#ifndef FORMAT_TOOLBOX_H
#define FORMAT_TOOLBOX_H

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "fasada.hpp"
#include "http/URLparser.hpp"
#include <string>
#include <stdexcept>

namespace fasada
{


class format_error : public std::runtime_error
{
public:
    /// Instantiate a format_error instance with the given message.
    /// @param what The message to associate with this error.
    format_error(const std::string &what):std::runtime_error(what)
    {}

    ~format_error() throw() {}
};


class format_toolbox
{
protected://Theese below should be changed into configurable dictionary TODO
    static std::string HTMLHeader;//Full HEAD of HTML page
    static std::string HTMLFooter;//Compatible footer of HTML page
    static std::string HTMLAction;//HTML contruction for action link
    static std::string HTMLBack;  //"UP","RETURN","WRÓĆ" of "<---"

public:
    format_toolbox();
    virtual//Replacing ${variable_name} with variables from request
        std::string replace_all_variables(std::string template_version,URLparser& request);
    static
        std::string  getHtmlHeaderDefaults(const std::string& Title);//Default set of html <HEAD> lines finishing by <BODY>
    static
        std::string  getHtmlClosure(const std::string& _unit_comp="");//Compatible set of tags for end of html document
    static
        std::string  getActionLink(const std::string& Href,const std::string& Content,const std::string& Title="");
    static
        std::string  getSeeLink(const std::string& data,URLparser& request,const std::string& Content);
    static
        std::string  getNodePanel(const pt::ptree& node,const std::string& fullpath,URLparser& request);
    static
        std::string  preprocessIntoHtml(const std::string& tmp);//Preprocess links and other markers into HTML tags & unicode chars
    static
        bool isLink(const std::string& str);//Checks for whole str looks like URL
    static
        bool isLocalFile(const std::string& str);//Checks for whole str looks like filename
    static
        unsigned int countCharacters(std::string str,char c);//Count \n / . or other importatnt characters in the string str

};

}//namespace "fasada"
#endif // FORMAT_TOOLBOX_H
