# facies
Facies P2P social network (browser part README)
================================================================
#opensocialmedia #peer2peer #opensource #pgp #free #freedom #openaccess #privacy #ownership

WYMAGANIA
---------
Po stronie przeglądarki konieczne jest uruchomienie skryptu śledzącego inne zakładki/okna 
w celu synchronizacji sieci Facies z innymi sieciami społecznościowymi.
Taki skrypt, który wylistowałby wszystkie zakładki otwarte w przegladarce, a potem dla wybranej przez uzytkownika 
prze-iterował całe aktualne drzewo dokumentu i zrobił z niego ponownie html (zwrócony w postaci stringu).
Lokalny mini-serwer Facies mógłby z w takim stringu (zawierającym widok z Fb, Google+ czy Twittera) 
znaleźć posty swojego użytkownika i przekazać ich treść do systemu p2p 
(oczywiście sprawdzając czy już tego wcześniej nie zrobił, chyba że treść postu się zmieniła na skutek edycji)
Czyli chodzi o implementacje pkt 7 z założeń projektu.  
Być może to niemożliwe lub trudne, wtedy alternatywą byłby server proxy przechwytujący posty użytkownika wysyłane do innych sieci społecznościwych.
