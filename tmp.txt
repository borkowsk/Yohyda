/data/wb/SCC/working_copies/facies/serverside/fasada/processor_find.cpp:136: error: operands to ?: have different types ‘fasada::processor_find::_implement_substring_find(fasada::ShmString&, boost::property_tree::ptree&, fasada::URLparser&)::<lambda(const ptree&, std::__cxx11::string)>’ and ‘fasada::processor_find::_implement_substring_find(fasada::ShmString&, boost::property_tree::ptree&, fasada::URLparser&)::<lambda(const ptree&, std::__cxx11::string)>’
         ( subpath == "*" ?



/data/wb/SCC/working_copies/facies/serverside/fasada/processor_find.cpp:142: error: operands to ?: have different types ‘fasada::processor_find::_implement_substring_find(fasada::ShmString&, boost::property_tree::ptree&, fasada::URLparser&)::<lambda(const ptree&, std::__cxx11::string)>’ and ‘fasada::processor_find::_implement_substring_find(fasada::ShmString&, boost::property_tree::ptree&, fasada::URLparser&)::<lambda(const ptree&, std::__cxx11::string)>’
     auto real_lambda=( subpath == "*" ? first_lambda : secon_lambda );
                        ~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/data/wb/SCC/working_copies/facies/serverside/fasada/processor_find.cpp:142: error: operands to ?: have different types ‘fasada::processor_find::_implement_substring_find(fasada::ShmString&, boost::property_tree::ptree&, fasada::URLparser&)::<lambda(const ptree&, std::__cxx11::string)>’ and ‘fasada::processor_find::_implement_substring_find(fasada::ShmString&, boost::property_tree::ptree&, fasada::URLparser&)::<lambda(const ptree&, std::__cxx11::string)>’
     auto real_lambda=( true ? first_lambda : secon_lambda );//subpath == "*"
                        ~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
