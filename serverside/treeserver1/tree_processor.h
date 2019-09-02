#ifndef TREE_PROCESSOR_H
#define TREE_PROCESSOR_H (1)

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include "tree_types.h"
#include "URLparser.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <string>

namespace pt = boost::property_tree;

namespace facjata
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
    public: //SUBTYPES
        enum Category {CONTROL=0,WRITER=1,READER=2};

    protected://Configuration
        std::string  procName;
        Category procCategory;

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
        //Do some work, call _implement_read, clean & return
        void read_tree(ShmString& o,const pt::ptree& top,URLparser& request);//may throw
        //Do some work, call _implement_write, clean & return
        void write_tree(ShmString& o,pt::ptree& top,URLparser& request);//may throw
};


}//namespace facjata
#endif // TREE_PROCESSOR_H
