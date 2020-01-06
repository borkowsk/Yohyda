#!/bin/bash
#STARTING FASADA 
if [ -e "output.fac" ]
then
   echo "Kontynuuje ze znalezionego pliku output.fac" 
   cp output.fac private//input.fac
   echo "/data/wb/SCC/__working_copies/fasada/fasada-core/treeserver private//input.fac" -$1
   /data/wb/SCC/__working_copies/fasada/fasada-core/treeserver private//input.fac -$1 &
else
   echo "Rozpoczęcie z pliku indeksu: /data/wb/SCC/__working_copies/fasada/private/index.json"
   /data/wb/SCC/__working_copies/fasada/fasada-core/treeserver /data/wb/SCC/__working_copies/fasada/private/index.json - &
fi
sleep 5
/data/wb/SCC/__working_copies/fasada/fasada-core/wwwserver localhost 8080 private/  > wwwserver.log 2>&1 &
sleep 2
/opt/google/chrome/chrome "http:localhost:8080/?ls&html&long"
