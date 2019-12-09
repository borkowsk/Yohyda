#!/bin/bash
echo "$0 is a creator of new tree processors for fasada"
echo "USAGE:"
echo "$0 'class_name' 'base_class' 'fasada_name'"
echo 
echo "    only letters and _ are allowed in parameters!"

UPCASE3=${3^^} #https://linuxhint.com/bash_lowercase_uppercase_strings/

if [ $# -ne 3 ]; 
then
   echo "Illegal number of parameters! Expected 3."
else
   echo "Creating:"
   
   echo "../$1.h"
   if [ -f "../$1.h" ]
   then
        echo "../$1.h already exist."
   else
        sed "s/processor_xxx/$1/g" processor_xxx.h   | sed "s/yyy_class/$2/g" |\
        sed "s/XXX/$UPCASE3/g" | sed "s/xxx/$3/g"  > "../$1.h"
   fi

   echo "../$1.cpp"
   if [ -f "../$1.cpp" ]
   then
        echo "../$1.cpp already exist."
   else
        sed "s/processor_xxx/$1/g" processor_xxx.cpp | sed "s/yyy_class/$2/g" |\
        sed "s/XXX/$UPCASE3/g" | sed "s/xxx/$3/g"  > "../$1.cpp"
   fi
fi
