Wymagane biblioteki


sudo apt-get libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libglm-dev
sudo apt-get install libglew-dev
#***sudo apt-get install lib32z1-dev
#***sudo apt-get install libfreetype6-dev


*****
INSTALACJA SDL2
*****

SDL2 do pobrania http://libsdl.org/download-2.0.php

cd SDL
mkdir build
cd build
../configure
make
sudo make install

*****
BUDOWANIE KLIENTA
*****
qmake platformowka.pro
make
Umieścić foldery Assets, Fonts, Levels, Shaders razem z plikiem wykonywalnym
Powinno działać
