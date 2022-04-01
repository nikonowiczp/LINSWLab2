# LINSWLab2
Poradnik do laboratorium 2 z przedmiotu Linux w Systemach wbudowanych

$BRPATH - katalog g��wny buildroota

#Co trzeba zrobi�? 

1. Skompilowa� kod w c++ na Raspberry Pi. 
2. Skomunikowa� si� z diod� i przyciskami z kodu w c++.

Tutaj poka�� jak skompilowa� kod kt�ry b�dzie �wieci� diod� (diod� obok tej oznaczaj�c� czy system jest ratunkowy czy zwyk�y).

---

#Kompilacja hello world w c++
##Podstawy pakietu.
�eby skompilowa� kod prze buildroota nale�y doda� sw�j w�asny pakiet do buildroota.


Pakiety znajduj� si� w folderze $BRPATH/package 
$PACKAGENAME - nazwa jakiego� wybranego pakietu
Ka�dy pakiet znajduje si� w folderze $BRPATH/package/$PACKAGENAME
Ka�dy pakiet jest opisany przez co najmniej dwa pliki(s� w repo):

1.Config.in 
Plik opisuje jak wygl�da pakiet gdy widzimy go w menuconfig - jego nazw�, tekst jego pomocy, pakiety kt�re powinny by� zainstalowane w buildroocie �eby nasz pakiet si� skompilowa�.
Wa�na uwaga: nie jest konieczne, �eby w tym pliku zale�no�ci by�y wypisane. Jest to jednak przydatne gdy zaczyna si� robi� wi�cej pakiet�w.

2. $PACKAGENAME.md
Plik opisuje spos�b kompilacji naszego pakietu. Podajemy tam takie informacje, jak miejsce gdzie znajduj� si� kody �r�d�owe naszego pakietu, jak ma je pobra� i w jaki spos�b skompilowa�.
Na przyk�ad linijki:
${PACKAGENAME}_SITE = ${PATH_TO_CODE}
${PACKAGENAME}_SITE_METHOD = local

Oznacza, �e kod ma zosta� skopiowany z lokalnego systemu plik�w.
Mo�na na przyk�ad pobra� �r�d�a z githuba lub za pomoc� scp, wget itd.

Tutaj nale�y umie�ci� zale�no�ci naszego pakietu.

Polecam wykonywa� kompilacj� przy u�yciu CMake. Sprowadza si� to do napisania jednego(dw�ch je�eli chce si� to zrobi� porz�dnie) plik�w z ~5 linijkami a nie trzeba si� bawi� bezpo�rednio z Makefile'ami


##Dodanie pakietu do systemu

Gdy mamy ju� gotowy folder z dwoma plikami opisuj�cymi nasz pakiet musimy jeszcze sprawi�, �eby by� on widoczny w menuconfig.
Sprowadza si� to do edytowania pliku $BRPATH/package/Config.in
W tym pliku wypisana jest lista wielu pakiet�w. 
Dodanie naszego pakietu sprowadza si� do dodanie linijki 

source "package/$PACKAGENAME/Config.in"

Tam gdzie chcemy.
Gdy dodamy t� linijk� wewn�trz bloku menu nasz pakiet b�dzie w opowiedniej zak�adce w menuconfig.
Polecam doda� linijki 

menu "Private apps"
source "package/$PACKAGENAME/Config.in"
endmenu

Stworzy si� wtedy zak�adka z naszymi paczkami.

Po zmodyfikowaniu $BRPATH/package/Config.in wpisujemy make menuconfig, znajdujemy nasz pakiet w   (Target Packages) --> (Nasze menu) --> $PACKAGENAME. W��czamy go do projektu. Przy nastepnym make skompiluje si� do systemu i wykona make install.

Je�eli po wpisaniu make nie ma �adnych b��d�w zwi�zzanych z naszym pakietem oznacza to �e skompilowa� si� i jest git!.

##Czysta rekompilacja naszego pakietu.

Czasami gdy chcemy zmieni� np CMakeLists lub Makefile chcemy mie� pewno�� �e rekompilacja wykona�a si� poprawnie.

Najlatwiej to osi�gn�� usuwaj�c folder $BRPATH/output/build/${PACKAGENAME}*  <--- jako� tak si� nazywa, mniejsza o szczeg�y.

#Dodanie pakietu z kodem obs�uguj�cym LEDY

Obs�ug� LED�w osi�gamy korzystaj�c z libgpiod.

Musimy doda� ten pakiet w (Target packages) -> (Libraries) -> (Hardware handling) -> []libgpiod

Doda�em te� tools bo si� pokaza�y pod spodem.

Nale�y doda� linkowanie biblioteki libgpiod w odpowiednich miejscach w pliku ${PACKAGENAME}.md i w samym Makefile/CMake . Kody s� w repozytorium.





