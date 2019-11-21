#!/bin/bash

#NARZĘDZIA
function pause(){
   read -p "$*"
}

#KONFIGURACJA
browser="firefox"
PRIVATEDIR="private/"
PRIVATEPORT=8080 #ONLY ONE USER PER HOST CAN HAVE THIS NUMBER!
PUBLICDIR="public/"
PUBLICPORT=8000 #ONLY ONE USER PER HOST CAN HAVE THIS NUMBER!

echo
echo "KREATOR INSTALACJI SYSTEMU FASADA DLA Linux-UBUNTU (polskojęzyczny)"
echo "(For english version use creator_en.sh)"
echo

echo "Szukam niezbędnych składników..."
CMAKE=`whereis cmake | cut --delimiter=' '   -f 2`
GPP=`whereis g++ | cut --delimiter=' '   -f 2`

echo "Przeszukano."

if [ ! -e "$CMAKE" ]
then
      echo "cmake jest konieczny. Zainstaluj ten program."
      pause "KONTYNUOWAĆ? (ENTER lub Ctrl-C żeby przerwać)"
else
      echo "cmake znaleziony: $CMAKE"
fi

if [ ! -e "$GPP" ]
then
      echo "g++ jest konieczny. Zainstaluj ten program."
      pause "KONTYNUOWAĆ? (ENTER lub Ctrl-C żeby przerwać)"
else
      echo "g++ znaleziony: $GPP"
fi


echo
echo "AKTUALIZACJA ŹRÓDEŁ..."
git pull
echo "WYKONANO"
echo

echo "BUDOWANIE AKTUALNEJ WERSJI..."
pushd fasada-core/
rm Makefile
$CMAKE .
if [ ! -e Makefile ]
then
      echo "Makefile nie został wygenerowany"
else
      echo "BUDOWANIE PROJEKTU"
      make
fi
popd
echo "WYKONANO"
echo

echo "POSZUKIWANIE I AKTUALIZACJA DANYCH Z PORTALI SPOŁECZNOŚCIOWYCH"
pushd $PRIVATEDIR
echo
echo "LISTA ARCHIWÓW W KATALOGU $PRIVATEDIR"
ls -lt *.zip
ZIPS=`ls -t *.zip`
echo
for file in "$ZIPS"
do 
      echo "	ARCHIWUM" `file $file`
      DIRNAME=`echo "$file" | cut -d'.' -f1`
      if [ ! -e $DIRNAME ]
      then
	echo "	TWORZĘ KATALOG $DIRNAME	" 
	mkdir $DIRNAME
      fi
      echo "	ROZPAKOWYWANIE: unzip -u $file -d $DIRNAME "
      unzip -u $file -d $DIRNAME
      echo "	ROZPAKOWYWANIE ZAKOŃCZONE"
done
echo

BACKUP=".BACKUP"
if [ ! -e "$BACKUP" ]
then
  echo "	TWORZĘ KATALOG" "$BACKUP"
  mkdir "$BACKUP"
fi

if [ -e "index.json" ]
then
  echo "	ZAPAMIĘTUJE POPRZEDNI PLIK index.json"
  mv index.json "${BACKUP}/"
fi

echo
echo "INDEKSUJE ZAWARTOŚĆ KATALOGU $PRIVATEDIR"
../fasada-core/indexer "."
echo "WYKONANO"
echo

popd
echo "POSZUKIWANIE I INDEKSOWANIE WYKONANE"
echo

echo "PIERWSZE ODPALENIE SERWISU"
indexpath=`realpath "$PRIVATEDIR/index.json"`
echo "treeserver $indexpath - "
./fasada-core/treeserver $indexpath --force > treeserver.log &
pause "Naciśnij ENTER"
echo
echo "wwwserver localhost $PRIVATEPORT $PRIVATEDIR > wwwserver.log"
./fasada-core/wwwserver localhost $PRIVATEPORT $PRIVATEDIR > wwwserver.log &
pause "Naciśnij ENTER"
echo 
$browser "http:localhost:$PRIVATEPORT/?ls&html"
echo
pause "Nacisniej ENTER gdy skończysz testowanie"
$browser "http:localhost:$PRIVATEPORT/!!!!"
echo "ZAKOŃCZONE"


echo "..."

echo "NIE MAM JUŻ NIC WIĘCEJ DO ZROBIENIA"