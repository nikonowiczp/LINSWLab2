# LINSWLab2
Poradnik do laboratorium 2 z przedmiotu Linux w Systemach wbudowanych

$BRPATH - katalog g³ówny buildroota

#Co trzeba zrobiæ? 

1. Skompilowaæ kod w c++ na Raspberry Pi. 
2. Skomunikowaæ siê z diod¹ i przyciskami z kodu w c++.

Tutaj poka¿ê jak skompilowaæ kod który bêdzie œwieci³ diod¹ (diod¹ obok tej oznaczaj¹c¹ czy system jest ratunkowy czy zwyk³y).

---

#Kompilacja hello world w c++
##Podstawy pakietu.
¯eby skompilowaæ kod prze buildroota nale¿y dodaæ swój w³asny pakiet do buildroota.


Pakiety znajduj¹ siê w folderze $BRPATH/package 
$PACKAGENAME - nazwa jakiegoœ wybranego pakietu
Ka¿dy pakiet znajduje siê w folderze $BRPATH/package/$PACKAGENAME
Ka¿dy pakiet jest opisany przez co najmniej dwa pliki(s¹ w repo):

1.Config.in 
Plik opisuje jak wygl¹da pakiet gdy widzimy go w menuconfig - jego nazwê, tekst jego pomocy, pakiety które powinny byæ zainstalowane w buildroocie ¿eby nasz pakiet siê skompilowa³.
Wa¿na uwaga: nie jest konieczne, ¿eby w tym pliku zale¿noœci by³y wypisane. Jest to jednak przydatne gdy zaczyna siê robiæ wiêcej pakietów.

2. $PACKAGENAME.md
Plik opisuje sposób kompilacji naszego pakietu. Podajemy tam takie informacje, jak miejsce gdzie znajduj¹ siê kody Ÿród³owe naszego pakietu, jak ma je pobraæ i w jaki sposób skompilowaæ.
Na przyk³ad linijki:
${PACKAGENAME}_SITE = ${PATH_TO_CODE}
${PACKAGENAME}_SITE_METHOD = local

Oznacza, ¿e kod ma zostaæ skopiowany z lokalnego systemu plików.
Mo¿na na przyk³ad pobraæ Ÿród³a z githuba lub za pomoc¹ scp, wget itd.

Tutaj nale¿y umieœciæ zale¿noœci naszego pakietu.

Polecam wykonywaæ kompilacjê przy u¿yciu CMake. Sprowadza siê to do napisania jednego(dwóch je¿eli chce siê to zrobiæ porz¹dnie) plików z ~5 linijkami a nie trzeba siê bawiæ bezpoœrednio z Makefile'ami


##Dodanie pakietu do systemu

Gdy mamy ju¿ gotowy folder z dwoma plikami opisuj¹cymi nasz pakiet musimy jeszcze sprawiæ, ¿eby by³ on widoczny w menuconfig.
Sprowadza siê to do edytowania pliku $BRPATH/package/Config.in
W tym pliku wypisana jest lista wielu pakietów. 
Dodanie naszego pakietu sprowadza siê do dodanie linijki 

source "package/$PACKAGENAME/Config.in"

Tam gdzie chcemy.
Gdy dodamy t¹ linijkê wewn¹trz bloku menu nasz pakiet bêdzie w opowiedniej zak³adce w menuconfig.
Polecam dodaæ linijki 

menu "Private apps"
source "package/$PACKAGENAME/Config.in"
endmenu

Stworzy siê wtedy zak³adka z naszymi paczkami.

Po zmodyfikowaniu $BRPATH/package/Config.in wpisujemy make menuconfig, znajdujemy nasz pakiet w   (Target Packages) --> (Nasze menu) --> $PACKAGENAME. W³¹czamy go do projektu. Przy nastepnym make skompiluje siê do systemu i wykona make install.

Je¿eli po wpisaniu make nie ma ¿adnych b³êdów zwi¹zzanych z naszym pakietem oznacza to ¿e skompilowa³ siê i jest git!.

##Czysta rekompilacja naszego pakietu.

Czasami gdy chcemy zmieniæ np CMakeLists lub Makefile chcemy mieæ pewnoœæ ¿e rekompilacja wykona³a siê poprawnie.

Najlatwiej to osi¹gn¹æ usuwaj¹c folder $BRPATH/output/build/${PACKAGENAME}*  <--- jakoœ tak siê nazywa, mniejsza o szczegó³y.

#Dodanie pakietu z kodem obs³uguj¹cym LEDY

Obs³ugê LEDów osi¹gamy korzystaj¹c z libgpiod.

Musimy dodaæ ten pakiet w (Target packages) -> (Libraries) -> (Hardware handling) -> []libgpiod

Doda³em te¿ tools bo siê pokaza³y pod spodem.

Nale¿y dodaæ linkowanie biblioteki libgpiod w odpowiednich miejscach w pliku ${PACKAGENAME}.md i w samym Makefile/CMake . Kody s¹ w repozytorium.





