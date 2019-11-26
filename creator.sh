#!/bin/bash

#KONFIGURACJA FASADY
browser="firefox"
PRIVATEDIR="private/"
PRIVATEPORT=8080 #TYLKO JEDEN UŻYTKOWNIK KOMPUTERA MOŻE MIEĆ TAKI NUMER!
PUBLICDIR="public/"
PUBLICPORT=8000 #TYLKO JEDEN UŻYTKOWNIK KOMPUTERA MOŻE MIEĆ TAKI NUMER!
SOURCEDIR="fasada-core/"

#KONFIGURACJA WYSWIETLANIA
ECHO="echo -e"
COLOR1="\e[36m"
COLOR2="\e[35m"
NORMCO="\e[0m"


$ECHO
$ECHO $COLOR1 "KREATOR INSTALACJI SYSTEMU FASADA DLA Linux-UBUNTU (polskojęzyczny)"
$ECHO $COLOR2 "(For english version use creator_en.sh)"
$ECHO $NORMCO

#NARZĘDZIA

TIMESTAMP=`date --utc +%Y%m%d_%H%M%SZ`

function pause(){
   $ECHO "$*"
   read -p "?"
}

$ECHO $COLOR2 "Szukam niezbędnych składników..." $NORMCO
CMAKE=`whereis cmake | cut --delimiter=' '   -f 2`
GPP=`whereis g++ | cut --delimiter=' '   -f 2`

$ECHO $COLOR2 "Przeszukano." $NORMCO

if [ ! -e "$CMAKE" ]
then
      $ECHO $COLOR1 "cmake jest konieczny. Zainstaluj ten program." $NORMCO
      pause $COLOR2 "KONTYNUOWAĆ? (ENTER lub Ctrl-C żeby przerwać)" $NORMCO
else
      $ECHO $COLOR2 "cmake znaleziony:$COLOR1 $CMAKE" $NORMCO
fi

if [ ! -e "$GPP" ]
then
      $ECHO $COLOR1 "g++  jest konieczny.  Zainstaluj ten program." $NORMCO
      pause $COLOR2 "KONTYNUOWAĆ? (ENTER lub Ctrl-C żeby przerwać)" $NORMCO
else
      $ECHO $COLOR2 "g++ znaleziony:$COLOR1 $GPP" $NORMCO
fi
$ECHO $NORMCO

$ECHO $COLOR2 "AKTUALIZACJA ŹRÓDEŁ..." $NORMCO
git pull
$ECHO $COLOR2 "WYKONANO" 
$ECHO $NORMCO

$ECHO $COLOR2 "BUDOWANIE AKTUALNEJ WERSJI..." $NORMCO
pushd $SOURCEDIR
rm Makefile
$ECHO $COLOR1
$CMAKE .
$ECHO $NORMCO

if [ ! -e Makefile ]
then
      $ECHO $COLOR1 "Makefile nie został wygenerowany" $NORMCO
      exit
else
      $ECHO $COLOR2 "BUDOWANIE PROJEKTU" $NORMCO
      make
fi

if [ ! -e treeserver ]
then
      $ECHO $COLOR1 "Brak koniecznego pliku wykonywalnego treeserver" $NORMCO
      pause $COLOR2 "KONTYNUOWAĆ? (ENTER lub Ctrl-C żeby przerwać)" $NORMCO
fi

if [ ! -e wwwserver ]
then
      $ECHO $COLOR1 "Brak koniecznego pliku wykonywalnego treeserver" $NORMCO
      pause $COLOR2 "KONTYNUOWAĆ? (ENTER lub Ctrl-C żeby przerwać)" $NORMCO
fi

popd
$ECHO $COLOR2 "WYKONANO"
$ECHO $NORMCO

if [ -e "stop.sh" ]
then
     $ECHO $COLOR1 "WYŁĄCZENIE SERWISU FASADA"
     ./stop.sh
fi

$ECHO $COLOR2 "POSZUKIWANIE I AKTUALIZACJA DANYCH Z PORTALI SPOŁECZNOŚCIOWYCH" $NORMCO
pushd $PRIVATEDIR
$ECHO
$ECHO $COLOR2 "LISTA ARCHIWÓW W KATALOGU" $PRIVATEDIR $NORMCO
ls -lt *.zip
ZIPS=`ls -t *.zip`
$ECHO $COLOR2 "DO SPRAWDZENIA:" $COLOR1 $ZIPS
$ECHO $NORMCO

for file in $ZIPS
do 
      DIRNAME=`echo "$file" | cut -d'.' -f1`
      $ECHO $COLOR2 "\t" "ARCHIWUM" $COLOR1 $file 
      $ECHO $COLOR2 "\t" "ROZPAKOWUJE DO KATALOGU" $COLOR1 $DIRNAME $COLOR2

      if [ ! -e "$DIRNAME" ]
      then
	$ECHO $COLOR2 "\t" "TWORZĘ KATALOG" $COLOR1 $DIRNAME $NORMCO
	mkdir $DIRNAME
      fi

      $ECHO "\t" "ROZPAKOWYWANIE: $COLOR1 unzip -u $file -d $DIRNAME $NORMCO"
      unzip -u $file -d $DIRNAME
      $ECHO $COLOR2 "\t" "ROZPAKOWYWANIE ZAKOŃCZONE"
      $ECHO $NORMCO
done


BACKUP=".BACKUP"
if [ ! -e "$BACKUP" ]
then
  $ECHO $COLOR1 "TWORZĘ KATALOG" "$BACKUP" $NORMCO
  mkdir "$BACKUP"
fi 

if [ -e "index.json" ]
then
  $ECHO $COLOR2 "ZAPAMIĘTUJE POPRZEDNI PLIK $COLOR1 index.json" $NORMCO
  mv index.json "${BACKUP}/index$TIMESTAMP.json"
  $ECHO $NORMCO
fi
$ECHO $NORMCO

$ECHO $COLOR2 "INDEKSUJE ZAWARTOŚĆ KATALOGU" $PRIVATEDIR $NORMCO
pwd
$ECHO $COLOR1
../fasada-core/indexer "." | tee indexer.log | grep -v "#"
$ECHO $COLOR2 "WYKONANO"
$ECHO $NORMCO

popd
$ECHO $COLOR2 "POSZUKIWANIE I INDEKSOWANIE WYKONANE" 
$ECHO $NORMCO

SKIN="$PRIVATEDIR/_skin/"
if [ ! -e "$SKIN" ]
then
  $ECHO $COLOR2 "TWORZĘ KATALOG $COLOR1 $SKIN $COLOR2 i JEGO OBOWIĄZKOWĄ ZAWARTOŚĆ" $NORMCO
  mkdir "$SKIN"
  cp fasada-core/lib/fasada/_skin_template/* "$SKIN"
fi
$ECHO $NORMCO

$ECHO $COLOR2 "TESTOWE ODPALENIE SERWISU" $COLOR1

if [ -e "output.fac" ]
then
   $ECHO $COLOR2 "ZABEZPIECZENIE OSTATNICH WYNIKÓW:$COLOR1  cp output.fac output$TIMESTAMP.fac $NORMCO"
   mv output.fac output$TIMESTAMP.fac
fi

indexpath=`realpath "$PRIVATEDIR/index.json"`
$ECHO "treeserver $indexpath - " $NORMCO
./fasada-core/treeserver $indexpath --force &
sleep 5
pause $COLOR2 "Naciśnij ENTER" $COLOR1
$ECHO
$ECHO "wwwserver localhost $PRIVATEPORT $PRIVATEDIR > wwwserver.log" $NORMCO
./fasada-core/wwwserver localhost $PRIVATEPORT $PRIVATEDIR > wwwserver.log &
sleep 5
pause $COLOR2 "Naciśnij ENTER" $NORMCO
$ECHO 
$browser "http:localhost:$PRIVATEPORT/?ls&html&long"
$ECHO
pause $COLOR2 "Nacisniej ENTER gdy skończysz testowanie" $NORMCO
$browser "http:localhost:$PRIVATEPORT/!!!!"
$ECHO $COLOR2 "ZAKOŃCZONE"
$ECHO $NORMCO

cat > start.sh <<EOF 
#!/bin/bash
#STARTING FASADA 
if [ -e "output.fac" ]
then
   cp output.fac $PRIVATEDIR/input.fac
   ./fasada-core/treeserver $PRIVATEDIR/input.fac -  > treeserver.log 2>&1 &
else
   ./fasada-core/treeserver $indexpath -  > treeserver.log 2>&1 &
fi
./fasada-core/wwwserver localhost $PRIVATEPORT $PRIVATEDIR  > wwwserver.log 2>&1 &
$browser "http:localhost:$PRIVATEPORT/?ls&html&long"
EOF

cat > stop.sh <<EOF 
#!/bin/bash
#FINISHING FASADA - WHEN WWWSERVER STOPS, ALL ATHERS PARTS ALSO EXIT
$browser "http:localhost:$PRIVATEPORT/!!!!"
EOF

chmod +x *.sh

wait
$ECHO $COLOR1 "NIE MAM JUŻ NIC WIĘCEJ DO ZROBIENIA" $NORMCO

