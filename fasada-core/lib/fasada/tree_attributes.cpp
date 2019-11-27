#include "fasada.hpp"
#include "tree_processor.h"

namespace pt = boost::property_tree;

namespace fasada
{

/// Each node may have special subnodes called properties saved under xmlattr subtree
/// Most important properties is "source", "loader", "saver", "alternative_savers", "oth_actions".
/// Standard attributes are presented in HTML by _implement_attributes() method
void insert_property(pt::ptree& Node,const std::string& FasadaPropertyName,const std::string& PropertyValue)
{
    Node.add_child(std::string("xmlattr.")+FasadaPropertyName,pt::ptree{PropertyValue});
}


void tree_processor::_implement_attributes(ShmString& o,pt::ptree& top,URLparser& request)
{

}


} //namespace fasada
