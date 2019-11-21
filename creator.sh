#!/bin/bash

#KONFIGURACJA FASADY
browser="firefox"
PRIVATEDIR="private/"
PRIVATEPORT=8080 #ONLY ONE USER PER HOST CAN HAVE THIS NUMBER!
PUBLICDIR="public/"
PUBLICPORT=8000 #ONLY ONE USER PER HOST CAN HAVE THIS NUMBER!

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


$ECHO
$ECHO $COLOR2 "AKTUALIZACJA ŹRÓDEŁ..." $NORMCO
git pull
$ECHO "WYKONANO" 
$ECHO $NORMCO

$ECHO $COLOR2 "BUDOWANIE AKTUALNEJ WERSJI..." $NORMCO
pushd fasada-core/
rm Makefile
$ECHO $COLOR1
$CMAKE .
if [ ! -e Makefile ]
then
      $ECHO $COLOR1 "Makefile nie został wygenerowany"
else
      $ECHO $COLOR2 "BUDOWANIE PROJEKTU" $NORMCO
      make
fi
popd
$ECHO $COLOR2 "WYKONANO"
$ECHO $NORMCO

$ECHO $COLOR2 "POSZUKIWANIE I AKTUALIZACJA DANYCH Z PORTALI SPOŁECZNOŚCIOWYCH" $NORMCO
pushd $PRIVATEDIR
$ECHO
$ECHO $COLOR2 "LISTA ARCHIWÓW W KATALOGU" $PRIVATEDIR $NORMCO
ls -lt *.zip
ZIPS=`ls -t *.zip`
$ECHO $COLOR1 $ZIPS $NORMCO

for file in $ZIPS
do 
      DIRNAME=`echo "$file" | cut -d'.' -f1`
      $ECHO $COLOR2 "	ARCHIWUM" $COLOR1 $file 
      $ECHO $COLOR2 "   ROZPAKOWUJE DO KATALOGU" $COLOR1 $DIRNAME $COLOR2 "\n"

      if [ ! -e "$DIRNAME" ]
      then
	$ECHO "	TWORZĘ KATALOG" $DIRNAME 
	mkdir $DIRNAME
      fi

      $ECHO "\n\t" "ROZPAKOWYWANIE: unzip -u $file -d $DIRNAME $NORMCO"
      unzip -u $file -d $DIRNAME
      $ECHO $COLOR2 "	ROZPAKOWYWANIE ZAKOŃCZONE"
done
$ECHO $NORMCO

BACKUP=".BACKUP"
if [ ! -e "$BACKUP" ]
then
  $ECHO $COLOR1 "	TWORZĘ KATALOG" "$BACKUP" $NORMCO
  mkdir "$BACKUP"
fi

if [ -e "index.json" ]
then
  $ECHO $COLOR2 "ZAPAMIĘTUJE POPRZEDNI PLIK index.json" $COLOR1
  mv index.json "${BACKUP}/"
  $ECHO $NORMCO
fi
$ECHO $NORMCO

$ECHO $COLOR2 "INDEKSUJE ZAWARTOŚĆ KATALOGU" $PRIVATEDIR $NORMCO
pwd
$ECHO $COLOR1
../fasada-core/indexer "."
$ECHO $COLOR2 "WYKONANO"
$ECHO $NORMCO

popd
$ECHO $COLOR2 "POSZUKIWANIE I INDEKSOWANIE WYKONANE" 
$ECHO $NORMCO

SKIN="$PRIVATEDIR/_skin/"
if [ ! -e "$SKIN" ]
then
  $ECHO $COLOR1 "	TWORZĘ KATALOG $SKIN i JEGO OBOWIĄZKOWĄ ZAWARTOŚĆ" $NORMCO
  mkdir "$SKIN"
  cp fasada-core/lib/fasada/_skin_template/* "$SKIN"
fi

$ECHO $COLOR2 "PIERWSZE ODPALENIE SERWISU" $COLOR1
indexpath=`realpath "$PRIVATEDIR/index.json"`
$ECHO "treeserver $indexpath - " $NORMCO
./fasada-core/treeserver $indexpath --force &
pause $COLOR2 "Naciśnij ENTER" $COLOR1
$ECHO
$ECHO "wwwserver localhost $PRIVATEPORT $PRIVATEDIR > wwwserver.log" $NORMCO
./fasada-core/wwwserver localhost $PRIVATEPORT $PRIVATEDIR > wwwserver.log &
pause $COLOR2 "Naciśnij ENTER" $NORMCO
$ECHO 
$browser "http:localhost:$PRIVATEPORT/?ls&html&long"
$ECHO
pause $COLOR2 "Nacisniej ENTER gdy skończysz testowanie" $NORMCO
$browser "http:localhost:$PRIVATEPORT/!!!!"
$ECHO $COLOR2 "ZAKOŃCZONE"
$ECHO $NORMCO

cat > fasada_start.sh <<EOF 
./fasada-core/treeserver $indexpath - 2>&1 > treeserver.log &
./fasada-core/wwwserver localhost $PRIVATEPORT $PRIVATEDIR 2>&1 > wwwserver.log &
$browser "http:localhost:$PRIVATEPORT/?ls&html&long"
EOF

chmod +x fasada_start.sh

wait
$ECHO $COLOR1 "NIE MAM JUŻ NIC WIĘCEJ DO ZROBIENIA" $NORMCO

