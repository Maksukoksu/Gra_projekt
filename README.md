<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <link href='https://fonts.googleapis.com/css?family=Chakra Petch' rel='stylesheet'>
    <link href='https://fonts.googleapis.com/css?family=Poppins' rel='stylesheet'>

    <link rel="stylesheet" href="style_dokumentacja.css">
    
    <title>Dokumentacja - projekt WdP - gra Crazy Driver</title>
</head>
<body>
    <h1>Gra Crazy Driver - Dokumentacja projektu</h1>
    <h2 id="sign">Maksymilian Tatomir, Katarzyna Wawrzycka</h2>

         <h2>Zasady gry</h2>
            <p>W grze Crazy Driver gracz jedzie samochodem pod prąd i ma za zadanie przejchać jak najdalej unikając kolizji.</p>
            <img src="images/overall.png">
            <p>Utrudnieniem rozgrywki są pojawiające się co jakiś czas ikonki kufla z piwem - jeśli gracz w nie wjedzie, spowoduje to odwrócenie sterowania na 4 sekundy, tzn. prawa strzałka będzie kierowała samochodem w lewo, lewa strzałka w lewo itp.</p>
            <p>Celem gry jest uzbieranie jak największej ilości punktów.</p> 

        <h2>Urochomienie gry</h2>
            <h3>Instalacja</h3>
            <p><a href="https://github.com/liballeg/allegro_wiki/wiki/Quickstart" target="_blank">GitHub</a></p>
                <p>Gra została tworzona i testowana na systemie Windows 11.</p>
                <p>Do uruchomienia gry konieczne są:</p>
                <ul>
                    <li>Środowisko programistyczne obsługujące język C++,</li>
                    <li>Biblioteka Allegro 5 - <a href="https://github.com/liballeg/allegro_wiki/wiki/Quickstart" target="_blank">oficjalny tutorial instalacji</a>.</li>
                </ul>
                <p>Po zainstalowaniu biblioteki należy skompilowac plik z grą game.cpp. To wszystko - aby móc zagrać trzeba już tylko uruchomić plik game.exe.</p>
                
            <h3>Sterowanie</h3>
                <ul>
                    <li>1, 2, 3 (zwykła klawiatura lub numeryczna) - wybór samochodu gracza,</li>
                    <li>P - przejście z ekranu startowego do gry,</li>
                    <li>strzałki (w prawo / w lewo / w górę / w dół) - sterowanie samochodem gracza (w prawo / w lewo / do przodu / do tyłu - uwaga: po kolizji z ikonką piwa sterowanie odwraca się),</li>
                    <li>E - wyjscie z gry po wyświetleniu ekranu końcowego</li>
                </ul>

            <h3>Początek rozgrywki</h3>
                <p>Po uruchomieniu gry otworzy się okno z ekranem startowym - w tym miejscu gracz ma możliwość wyboru swojego samochodu.</p>
                <div id="startScreensImg">
                    <img src="images/start1.png">
                    <img src="images/start2.png">
                    <img src="images/start3.png">
                </div>
                <p>Po wyborze samochodu i naciśnięciu "P" rozpoczyna się gra.</p>

            <h3>Rozgrywka</h3>
                <p>W trakcie gry z naprzeciwka nadjeżdżają samochody, które gracz musi omijać. Poruszają się one ze stałą prędkością, każdy na określonym pasie drogi. Między samochodami co jakiś czas pojawia się też ikonka kufla piwa. Piwo, podobnie jak samochody-przeszkody, porusza się ze stałą prędkością po wybranym pasie drogi. Prędkość piwa jest inna niż prędkość przeszkód dla urozmaicenia rozgrywki.</p>
                <img src="images/beer0.png" id="beerExl">
                <p>Gracz musi koniecznie omijać nadjeżdżające samochody, jednak omijanie piwa jest opcjonalne. Dla niektórych kolizja z piwem jest dobrą zabawą, a dla niektórych sporym urudnieniem. Mianowicie, kolizja z piwem powoduje odwrócenie sterowania samochodem gracza na 4 sekundy - prawa strzałka będzie kierować samochodem w lewo, lewa strzałka w prawo, strzałka w górę pokieruje samochodem gracza do tyłu, a strzałka w dół do przodu. Przez cały czas trwania odwróconego sterownia w lewym górnym rogu widoczny się licznik. Jego zadaniem jest sygnalizacja ile sekund zostało do powrótu normalnego sterowania, gdzie jeden kufel oznacza jedną sekundę.</p>   
                <div id="beerImg"> 
                    <img src="images/beer4.png" id="beer4Img">
                    <div id="beerEx">
                        <img src="images/beer3.png">
                        <img src="images/beer2.png">
                        <img src="images/beer1.png">
                    </div>
                </div>
                <p>W prawym górnym rogu natomiast widoczny jest licznik punktów, które gracz zdobywa od początku rozgrywki. Po upływie 1.5 sekundy doliczany jest 1 punkt.</p>
                <img src="images/score.png" id="scoreExl">

            <h3>Koniec gry</h3>
                <p>Gra kończy się w momencie kolizji samochodu gracza z samochodem-przeszkodą. Kolizja może mieć miejsce w dowolnym punkcie samochodu, to znaczy, że zderzenie "tyłu" gracza z "tyłem" przeszkody również oznacza kolizję, tak samo uderzenie w bok przeszkody.</p>
                <p>Po kolizji wyświetlany jest ekran końcowy, na którym widoczna jest ilosć punktów, jaką garcz zebrał w danej rozgrywce. Nowy najlepszy wynik zapisywany jest do pliku txt o nazwie "crazy_driver_highscore".</p>
                <img src="images/end.png" id="endExl">
                <p>Po naciśnięciu "E" cała gra się zamyka.</p>
                <p>*Projekt składa się w całości z autorskich grafik.</p>
        
        <h2>Kod</h2>
        <h3>Funkcje i struktury</h3>
        <p>Stworzyliśmy dwie struktury Obstacle oraz Beer, które służą do określania parametków przy tworzeniu samochodów-przeszkód oraz ikonek piwa. W strukturach określane są współrzędne x i y oraz prędkość każdego obiektu, dodatkowo w strukturze przeszkód został również określony typ, czyli po prostu zmienna potrzrbna so losowania jednej z trzech przeszkód (chodzi o losowanie koloru samochodu-przeszkody). Następnie stworzyliśmy dwie funkcje logiczne, które sprawdzają, czy miała miejsce kolizja z przeszkodą lub piwem.</p>
        <h3>Logika</h3>
        <p>Na podstawie wcześniej opisanych struktur generowane są najpierw ikonki piwa w odpowiednim odstępie między współrzędnymi y, aby nie pojawiały się na ekranie zbyt często. Samochody-przeszkody są generowane w pętli po 4 przeszkody. Współrzędne y każdej przeszkody są dobrane tak, aby między przeszkodami była odpowiednia odległość w pionie, żeby gracz mógł manewrować między samochodami. Współrzędne x zarówno piwa jak i przeszkód zostały z góry określone, tak aby zawsze wyświtlały się na jednym z trzech pasów drogi. Dodatkowo wszystkie elementy, tj. auto gracza, przeszkody i piwo, nigdy nie będą mogły wyjechać poza obszar drogi (na trawę). Całość wyświetla się na drodze, która wyświetlana jest poklatkowo (tak samo auto gracza).</p>
        <p id="last">System klatek jest wykorzystywany do mierzenia czasu, na podstawie którego działa liczenie punktów oraz odwracanie sterowania. Prędkość gry została ustawiona na 60FPS, więc 80 klatek = 1.5 sekundy = 1 punkt, 400 klatek = 4 sekundy = odwrócenie sterowania.</p>
</body>
