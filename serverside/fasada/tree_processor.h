#ifndef TREE_PROCESSOR_H
#define TREE_PROCESSOR_H (1)

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

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

    /// This class ...
    /// ...
    class tree_processor
    {
    protected:
        static std::string HTMLHeader;
        static std::string HTMLFooter;
    public: //SUBTYPES
        enum Category {CONTROL=4,WRITER_READER=3,WRITER=2,READER=1};

    //Category operator + (Category one,Category two) ???
    //{ return Category(One+Two);}
    //error: ‘fasada::tree_processor::Category fasada::tree_processor::operator+(fasada::tree_processor::Category, fasada::tree_processor::Category)’
    //
    // must take either zero or one argument

    protected://Configuration
        std::string  procName;
        Category procCategory;
        std::string  getHtmlHeaderDefaults(std::string& Title);//Default set of html <HEAD> lines finishing by <BODY>
        std::string  getHtmlClosure();//Compatible set of tags for end of html document
    protected://deferred implementation
    virtual
        void _implement_read(ShmString& o,const pt::ptree& top,URLparser& request)=0;
    virtual
        void _implement_write(ShmString& o,pt::ptree& top,URLparser& request)=0;

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
        void read_tree(ShmString& o,const pt::ptree& top,URLparser& request);//may throw
        //Does some work, calls _implement_read, cleans, adds MEM_END & returns
        void write_tree(ShmString& o,pt::ptree& top,URLparser& request);//may throw
};

    void register_processors(); //Function for registering default set of tree_processors
                                //located in fasada.a/lib/so

}//namespace "fasada"
#endif // TREE_PROCESSOR_H
