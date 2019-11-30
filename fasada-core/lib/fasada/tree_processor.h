/// This file is a part of Fasada toolbox
/// ---------------------------------------------------
/// @author Wojciech Borkowski <wborkowsk@gmail.com>
/// @copyright 2019
/// 
/// See licence file!
///

#ifndef TREE_PROCESSOR_H
#define TREE_PROCESSOR_H (1)

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "fasada.hpp"
#include "tree_types.h"
#include "http/URLparser.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <string>

namespace pt = boost::property_tree;

namespace fasada
{
    /// The boost::ptree have its own exception class ptree_error
    /// and derived class of exceptions.
    /// but sometimes tree_processor have to throw on error
    /// which is not connected with ptree
    /// In such cases it use this exception class
    // TODO - Consider this: https://www.boost.org/doc/libs/1_59_0/libs/exception/doc/frequently_asked_questions.html
    class tree_processor_exception:public pt::ptree_error
    {
        public:
            tree_processor_exception(std::string msg):pt::ptree_error(msg)
            {}
    };

    /// Each node may have special subnodes called properties saved under xmlattr subtree
    /// Most important properties is "_source", "loader", "viever", "saver", "alternative_savers", "oth_actions".
    /// Standard attributes are presented in HTML by _implement_attributes() method
    void insert_property(pt::ptree& Node,const std::string& FasadaPropertyName,const std::string& PropertyValue);

    /// This class ...
    /// ...
    class tree_processor
    {
    protected://Theese below should be changed into configurable dictionary TODO
        static std::string HTMLHeader;//Full HEAD of HTML page
        static std::string HTMLFooter;//Compatible footer of HTML page
        static std::string HTMLAction;//HTML contruction for action link
        static std::string HTMLBack;  //"UP","RETURN","WRÓĆ" of "<---"

    public: //SUBTYPES
        enum Category {CONTROL=4,WRITER_READER=3,WRITER=2,READER=1};

    //Category operator + (Category one,Category two) ???
    //{ return Category(One+Two);}
    //
    //error: ‘fasada::tree_processor::Category 
    //        fasada::tree_processor::operator+(fasada::tree_processor::Category, fasada::tree_processor::Category)’
    //
    // must take either zero or one argument

    protected://Configuration
        std::string  procName;
        Category procCategory;

    protected://deferred implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request)=0;
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request)=0;
    virtual
        void _implement_attributes(ShmString& o,const pt::ptree& top,URLparser& request,std::string nameOfTop="");

    public://Construction, destruction etc.
        tree_processor(Category cat,const char* name);
    virtual
        ~tree_processor();
    static
        tree_processor& getReadProcessor (const char* name);
    static
        tree_processor& getWriteProcessor(const char* name);
    static
        tree_processor& getReadProcessor (const std::string& name)
        {return getReadProcessor(name.c_str());}
    static
        tree_processor& getWriteProcessor(const std::string& name)
        {return getWriteProcessor(name.c_str());}

    public://Interface
        //Does some work, calls _implement_read, cleans, adds MEM_END & returns
        void read_tree(ShmString& o,const pt::ptree& top,URLparser& request);//may throw TODO readFromTree?
        //Does some work, calls _implement_read, cleans, adds MEM_END & returns
        void write_tree(ShmString& o,pt::ptree& top,URLparser& request);//may throw TODO writeToTree?

    protected: //Tools for childrens - theese below schould be changed into "mixin" TODO
    static
        std::string  getHtmlHeaderDefaults(const std::string& Title);//Default set of html <HEAD> lines finishing by <BODY>
    static
        std::string  getHtmlClosure(const char* _unit_comp="");//Compatible set of tags for end of html document
    static
        std::string  getActionLink(const std::string& Href,const std::string& Content,const std::string& Title="");
    static
        std::string  getSeeLink(const std::string& data,URLparser& request,const std::string& Content);
    static
        std::string  getNodePanel(const std::string& nodeval,const std::string& fullpath,URLparser& request);
    static
        std::string  preprocessIntoHtml(const std::string& tmp);//Preprocess links and other markers into HTML tags & unicode chars
    static
        bool isLink(std::string str);//Checks for whole str looks like URL
    static
        bool isLocalFile(std::string str);//Checks for whole str looks like filename
    static
        unsigned int countCharacters(std::string str,char c);//Count \n / . or other importatnt characters in the string str
};

}//namespace "fasada"
#endif // TREE_PROCESSOR_H
