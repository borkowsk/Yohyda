#./bin/bash
# You may need to install libmagic before
#
#    sudo apt-get install -y libmagic-dev #https://zoomadmin.com/HowToInstall/UbuntuPackage/libmagic-dev
#
# Compilation:
g++ -std=c++11 -Wall -pedantic indexer_main.cpp -lmagic -lboost_system -lboost_filesystem \
&& echo -e "\nCompiled\n" \
&& ./a.out $1 $2 \
&& echo -e "\nReady\n"
if [ $? -eq 0 ]; 
then
    mv a.out indexer
fi
