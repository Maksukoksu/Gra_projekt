   

Gra Crazy Driver - Dokumentacja projektu
========================================

Maksymilian Tatomir, Katarzyna Wawrzycka
----------------------------------------

Zasady gry
----------

W grze Crazy Driver gracz jedzie samochodem pod prąd i ma za zadanie przejechać jak najdalej unikając kolizji.

Utrudnieniem rozgrywki są pojawiające się co jakiś czas ikonki kufla z piwem - jeśli gracz w nie wjedzie, spowoduje to odwrócenie sterowania na 4 sekundy, tzn. prawa strzałka będzie kierowała samochodem w lewo, lewa strzałka w lewo itp.

Celem gry jest uzbieranie jak największej ilości punktów.

Uruchomienie gry
----------------

### Instalacja

[GitHub](https://github.com/liballeg/allegro_wiki/wiki/Quickstart)

Gra została tworzona i testowana na systemie Windows 11.

Do uruchomienia gry konieczne są:

*   Środowisko programistyczne obsługujące język C++, [CLion](https://www.jetbrains.com/clion/)
*   Pobrana paczka MinGW 4.7.0 w wersji ZIP ze strony [allegro.cc]( https://www.allegro.cc/files/), którą należy rozpakować,
*   Pobrane i zainstalowane [MinGW](https://sourceforge.net/projects/mingw/)

Dokładna instrukcja znajduje się w pliku Instrukcja.pdf

### Sterowanie

*   1, 2, 3 (zwykła klawiatura lub numeryczna) - wybór samochodu gracza,
*   P - przejście z ekranu startowego do gry,
*   strzałki (w prawo / w lewo / w górę / w dół) - sterowanie samochodem gracza (w prawo / w lewo / do przodu / do tyłu - uwaga: po kolizji z ikonką piwa sterowanie odwraca się),
*   E - wyjście z gry po wyświetleniu ekranu końcowego

### Początek rozgrywki

Po uruchomieniu gry otworzy się okno z ekranem startowym - w tym miejscu gracz ma możliwość wyboru swojego samochodu.

Po wyborze samochodu i naciśnięciu "P" rozpoczyna się gra.

### Rozgrywka

W trakcie gry z naprzeciwka nadjeżdżają samochody, które gracz musi omijać. Poruszają się one ze stałą prędkością, każdy na określonym pasie drogi. Między samochodami co jakiś czas pojawia się też ikonka kufla piwa. Piwo, podobnie jak samochody-przeszkody, porusza się ze stałą prędkością po wybranym pasie drogi. Prędkość piwa jest inna niż prędkość przeszkód dla urozmaicenia rozgrywki.

Gracz musi koniecznie omijać nadjeżdżające samochody, jednak omijanie piwa jest opcjonalne. Dla niektórych kolizja z piwem jest dobrą zabawą, a dla niektórych sporym utrudnieniem. Mianowicie, kolizja z piwem powoduje odwrócenie sterowania samochodem gracza na 4 sekundy - prawa strzałka będzie kierować samochodem w lewo, lewa strzałka w prawo, strzałka w górę pokieruje samochodem gracza do tyłu, a strzałka w dół do przodu. Przez cały czas trwania odwróconego sterownia w lewym górnym rogu widoczny się licznik. Jego zadaniem jest sygnalizacja ile sekund zostało do powrótu normalnego sterowania, gdzie jeden kufel oznacza jedną sekundę.

W prawym górnym rogu natomiast widoczny jest licznik punktów, które gracz zdobywa od początku rozgrywki. Po upływie 1.5 sekundy doliczany jest 1 punkt.

### Koniec gry

Gra kończy się w momencie kolizji samochodu gracza z samochodem-przeszkodą. Kolizja może mieć miejsce w dowolnym punkcie samochodu, to znaczy, że zderzenie "tyłu" gracza z "tyłem" przeszkody również oznacza kolizję, tak samo uderzenie w bok przeszkody.

Po kolizji wyświetlany jest ekran końcowy, na którym widoczna jest ilość punktów, jaką gracz zebrał w danej rozgrywce. Nowy najlepszy wynik zapisywany jest do pliku txt o nazwie "crazy\_driver\_highscore".

![](images/end.png)

Po naciśnięciu "E" cała gra się zamyka.

\*Projekt składa się w całości z autorskich grafik.

Kod
---

### Funkcje i struktury

Stworzyliśmy dwie struktury Obstacle oraz Beer, które służą do określania parametrów przy tworzeniu samochodów-przeszkód oraz ikonek piwa. W strukturach określane są współrzędne x i y oraz prędkość każdego obiektu, dodatkowo w strukturze przeszkód został również określony typ, czyli po prostu zmienna potrzebna do losowania jednej z trzech przeszkód (chodzi o losowanie koloru samochodu-przeszkody). Następnie stworzyliśmy dwie funkcje logiczne, które sprawdzają, czy miała miejsce kolizja z przeszkodą lub piwem.

### Logika

Na podstawie wcześniej opisanych struktur generowane są najpierw ikonki piwa w odpowiednim odstępie między współrzędnymi y, aby nie pojawiały się na ekranie zbyt często. Samochody-przeszkody są generowane w pętli po 4 przeszkody. Współrzędne y każdej przeszkody są dobrane tak, aby między przeszkodami była odpowiednia odległość w pionie, żeby gracz mógł manewrować między samochodami. Współrzędne x zarówno piwa jak i przeszkód zostały z góry określone, tak aby zawsze wyświetlały się na jednym z trzech pasów drogi. Dodatkowo wszystkie elementy, tj. auto gracza, przeszkody i piwo, nigdy nie będą mogły wyjechać poza obszar drogi (na trawę). Całość wyświetla się na drodze, która wyświetlana jest po klatkowo (tak samo auto gracza).

System klatek jest wykorzystywany do mierzenia czasu, na podstawie którego działa liczenie punktów oraz odwracanie sterowania. Prędkość gry została ustawiona na 60FPS, więc 80 klatek = 1.5 sekundy = 1 punkt, 400 klatek = 4 sekundy = odwrócenie sterowania.
