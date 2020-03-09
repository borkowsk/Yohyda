# fasada

Project FASADA version 0.01 (top level README)
==============================================================================
FASADA to napisany w C++ zestaw bibliotek i aplikacji (mini-framework) umożliwiający komunikację na wzór WWW pomiędzy przystosowanymi programami, a za pośrednictwem własnego mini-serwera WWW także z dowolnymi programami rozumiejącymi protokół HTTP, przynajmniej w sieci lokalnej.

Także głównym interfejsem użytkownika jest przeglądarka WWW.

Użycie standardowych formatów plików, takich jak  JSON, XML, CSV, TXT i przeglądarki jako interfejsu uniezależnia aplikacje używające tego frameworku od specyfiki lokalnego systemu operacyjnego, a jednocześnie pozwala zachować wydajność języka C++.
Daje to różnorodne możliwości twórcom prostych aplikacji użytkowych i rozrywkowych.

Pierwszym zastosowaniem Fasady jest system "yo-hy-da"* umożliwiający przeglądanie zasobów jakie użytkownik wygenerował w takich sieciach społecznościowych jak Facebook, Twitter, LinkedIn i Google+, które sieci te zgodnie z wymaganiami prawa udostępniają, ale w formie praktycznie bezużytecznej dla "zwykłego zjadacza bitów".

Innym, zaplanowanym, przykładem jest prosta gra WWW, działająca w sieci lokalnej. 

Jeszcze inna możliwość to właczenie interfejsu FASADY (poprzez bibliotekę) programom symulacyjnym i obliczeniowym, dzięki czemu kontrola ich działania i podgląd wyników będzie możliwy bezpośrednio przez przeglądarkę WWW, bez konieczności konfigurowania.

Po za tym aplikacje oparte o FASADĘ mogą umożliwiać, a przynajmniej ułatwiać, użytkownikowi nie znającemu się tworzeniu kontentu WWW aktywne korzystanie z takiej możlowości w sieci lokalnej, a także, pośrednio, globalnej.


*__UWAGA - BOOST!__*

Projekt został oparty na bibliotece BOOST. Wymaga jej do kompilacji i działania. Instalacja boost na UBUNTU 16.04: 

__sudo apt-get install libboost-all-dev__

*UWAGA - LIBMAGIC*

Do dedukowania typów plików używana jest biblioteka libMagic. Trzeba ją zainstalować:

__sudo apt-get install libmagic-dev__

*CO ZROBIĆ GDY ZMIENI SIĘ NAZWA REPOZYTORIUM?*

https://www.linuxquestions.org/questions/programming-9/git-my-remote-repository-moved-how-do-i-tell-my-local-repo-to-use-the-new-one-781785/

*TWITTER PROJEKTU*

https://twitter.com/fasada02964935

*TYMCZASOWA WITRYNA PROJEKTU:*

https://sites.google.com/view/fasada-cpp/

* yo(ur)-hy(per)-da(ta)

Podobny, ale dużo bardziej zaawansowany projekthierrchiczno-obiektowej bazy danych to:
https://www.mongodb.com/

 
