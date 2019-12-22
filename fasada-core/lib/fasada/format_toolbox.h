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
    static std::string HTMLHeaderBegin;//Beginng of HEAD of HTML page
    static std::string HTMLTitleBegin;//<TITLE> tag with attributes
    static std::string HTMLHeaderEnd;//And of header & BODY start
    static std::string HTMLBack;  // Symbol for returning to higher level;
    static std::string HTMLBodyEnd;//Compatible finishing of HTML page
    static std::string HTMLAction;//HTML contruction for action link

public:
    format_toolbox();

    /// Coding page elements
    static //Default set of html <HEAD> lines, then optionally definitions from parameter, finishing by <BODY>
        std::string  getPageHeader(const std::string& Title,
                                   const std::string AddHeaderDefs="");

    static
        std::string  getPageClosure(const std::string& unitCompilation="",
                                    const std::string AddToFooter=""
                                    );//Compatible set of tags for end of html document

    static
        std::string  getActionLink(const std::string& Href,const std::string& Content,const std::string& Title="");

    static
        std::string  getSeeLink(const std::string& data,URLparser& request,const std::string& Content);

    static
        std::string  getActionsPanel(URLparser& request);

    static
        std::string  getNodePanel(const pt::ptree& node,const std::string& fullpath,URLparser& request);

    static
        std::string  preprocessRawTxt(const std::string& tmp);//Preprocess links and other markers into HTML tags & unicode chars

    /// Informative functions
    static
        bool isLink(const std::string& str);//Checks for whole str looks like URL
    static
        bool isLocalFile(const std::string& str);//Checks for whole str looks like filename
    static
        unsigned int countCharacters(std::string str,char c);//Count \n / . or other importatnt characters in the string str

    /// For backward compatibility ;-)
    virtual//Replacing ${variable_name} with variables from request
        std::string replace_all_variables(std::string template_version,URLparser& request);

};

}//namespace "fasada"
#endif // FORMAT_TOOLBOX_H
