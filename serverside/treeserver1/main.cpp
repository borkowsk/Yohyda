//Local server of "facies/facjata" resources stored in json files
//
//Based on examples from: 
//      http://techgate.fr/boost-property-tree/
//      https://stackoverflow.com/questions/12346787/send-complex-data-structure-via-boost-message-queue
//And docs:
//      https://www.boost.org/doc/libs/1_71_0/doc/html/boost/interprocess/message_queue_t.html
//
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
const char debug_path[]="/data/wb/SCC/working_copies/facies/private/TimelineOfTheEarth/posts/posts.json";

// Short alias for this namespace
namespace pt = boost::property_tree;

// Create a root
pt::ptree root;

// Load the json file in this ptree
int main()
{
        pt::read_json(debug_path, root);//Czyta podstawowe dane - jakiś całkiem spory plik json
        
        return 0;
}

