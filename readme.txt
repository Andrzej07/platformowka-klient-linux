*****
WYMAGANE BIBLIOTEKI
*****

Środowisko Qt >4.0

sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libglm-dev
sudo apt-get install libglew-dev
sudo apt-get install lib32z1-dev
sudo apt-get install libfreetype6-dev

*****
INSTALACJA SDL2
*****

Kod źródłowy SDL2 do pobrania http://libsdl.org/download-2.0.php

Wypakować do SDL
cd SDL
mkdir build
cd build
../configure
make
sudo make install

*****
BUDOWANIE KLIENTA
*****
(opcjonalnie)qmake platformowka.pro
make
Umieścić foldery Assets, Fonts, Levels(ten sam co w serwerze), Shaders razem z plikiem wykonywalnym
Powinno działać


*****
UŻYWANIE
*****

WAŻNE - można uruchomić kilka instancji klienta, ale obraz będzie odświeżany tylko w jednym,
		testowanie na jednym komputerze jest problematyczne :) - na kliencie bez grafiki 
		działa input więc da się podłączyć do serwera i zaobserować ruch niedziałającego klienta
		w działającym.

OSBŁUGA MENU - *SINGLEPLAYER* uruchamia level0.txt, można sobie pobiegać bez lagów;
			   *MULTIPLAYER* po kliknięciu "Join server" aplikacja będzie czekać aż do
			   wybranego serwera podłączy się dwóch klientów, w międzyczasie nie będzie
			   odpowiadał; jeśli podane IP jest niepoprawne pozostaje tylko kill :)
			   *EDITOR*  <NYI>

*****
STEROWANIE W GRZE
*****

Ruch postacią - *W* skok
				*A* lewo
				*D* prawo

Sterowanie kamerą - *Q* przyblizenie
					*E* oddalenie

Gra			*ESC* powrót do menu


