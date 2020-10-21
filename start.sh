#!/bin/bash
#STARTING FASADA 
if [ -e "output.fac" ]
then
   echo "Kontynuuje ze znalezionego pliku output.fac" 
   cp output.fac private//input.fac
   echo "/data/wb/SCC/__working_copies/yohyda/fasada-core/treeserver private//input.fac" -$1
   /data/wb/SCC/__working_copies/yohyda/fasada-core/treeserver private//input.fac -$1  > treeserver.log 2>&1 &
else
   echo "Rozpoczęcie z pliku indeksu: /data/wb/SCC/__working_copies/yohyda/private/index.json"
   /data/wb/SCC/__working_copies/yohyda/fasada-core/treeserver /data/wb/SCC/__working_copies/yohyda/private/index.json -  > treeserver.log 2>&1 &
fi
sleep 5
/data/wb/SCC/__working_copies/yohyda/fasada-core/wwwserver localhost 8080 private/  > wwwserver.log 2>&1 &
sleep 2
/snap/bin/chromium "http:localhost:8080/?ls&html&long"
