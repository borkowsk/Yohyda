#fasada

Project FASADA version 0.01 (top level README)
==============================================================================

FASADA to system bibliotek i aplikacji umożliwiający komunikację na wzór WWW pomiędzy przystosowanymi programami, a za pośrednictwem własnego mini-serwera WWW także z dowolnymi programami rozumiejącymi protokoł HTTP, przynajmniej w sieci lokalnej.

Także głównym interfejsem użytkownika jest przegladarka WWW.

Użycie standardowych formatów plików, takich jak TXT,JSON,HTML,CSV i przeglądarki WWW uniezależnia aplikacje używające FASADY od specyfiki lokalnego systemu.

Daje to różnorodne możliwości twórcom prostych aplikacji użytkowych i rozrywkowych. Pierwszym zastosowaniem Fasady będzie system umożliwiający przeglądanie zasobów jakie użytkownik wygenerował w takich sieciach społecznościowych jak Facebok, Twitter, LinkedIn i Google+ , i które sieci te zgodnie z wymaganiami prawa udostępniają, ale w formie praktycznie bezużytecznej dla "zwykłego zjadacza bitów".

Innym, zaplanowanym, przykładem jest prosta gra WWW, działająca w sieci lokalnej. 

Jeszcze inna możliwość to właczenie interfejsu FASADY (poprzez bibliotekę) programom symulacyjnym i obliczeniowym, dzięki czemu kontrola ich działania i podgląd wyników będzie możliwy bezpościednio przez przeglądarkę WWW, bez konieczności konfigurowania.

Po za tym aplikacje oparte o FASADĘ mogą umożliwiać, a przynajmniej ułatwiać, użytkownikowi nie znającemu się tworzeniu kontentu WWW aktywne korzystanie z takiej możlowości w sieci lokalnej, a także, pośrednio, globalnej.

UWAGA:
Projekt został oparty na bibliotece BOOST. Wymaga jej do kompilacji i działania. Instalacja boost na UBUNTU 16.04: 

sudo apt-get install libboost-all-dev
