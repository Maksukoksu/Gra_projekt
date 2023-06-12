#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <fstream>

const float SPEED = 5.5; //predkosc auta gracza

//struktura do wyswietlania piwa
typedef struct{
    float x;
    float y;
    float speed;
} Beer;

//struktura do wyswietlania przeszkod
typedef struct{
    float x;
    float y;
    float speed;
    int type; //losowanie koloru
    bool crossed;
} Obstacle;

//sprawdzenie kolizji z piwem
bool BeerCollision(float car_x, float car_y, float beer_x, float beer_y, ALLEGRO_BITMAP* beer){
    //wspolrzedne auta
    float car_left = car_x - 6;
    float car_right = car_x + 50;
    float car_top = car_y;
    float car_bottom = car_y + 93;

    //wspolrzedne piwa
    float beer_left = beer_x + 2;
    float beer_right = beer_x + 30;
    float beer_top = beer_y + 2;
    float beer_bottom = beer_y + 30;

    //warunek wystapienia kolizji
    if(car_left <= beer_right && car_right >= beer_left && car_top <= beer_bottom && car_bottom >= beer_top){
        return true;
    }else{
        return false;
    }
}

//sprawdzenie kolizji z przeskoda
bool CheckCollision(float car_x, float car_y, float obstacle_x, float obstacle_y, ALLEGRO_BITMAP* obstacle){
    //wspolrzedne auta gracza
    float car_left = car_x - 6;
    float car_right = car_x + 50;
    float car_top = car_y;
    float car_bottom = car_y + 93;

    //wspolrzedne przeszkody
    float obstacle_left = obstacle_x - 6;
    float obstacle_right = obstacle_x + 50;
    float obstacle_top = obstacle_y;
    float obstacle_bottom = obstacle_y + 93;

    //warunek wystapienia kolizji
    if(car_left <= obstacle_right && car_right >= obstacle_left && car_top <= obstacle_bottom && car_bottom >= obstacle_top){
        return true;
    }else{
        return false;
    }
}

int main() {
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    srand(time(0));

    //czcionki do punktow
    ALLEGRO_FONT* font = al_load_font("Arcade.ttf", 22, 0); //w oknie gry
    ALLEGRO_FONT* font2 = al_load_font("Arcade.ttf", 40, 0); //na ekranie koncowym

    ALLEGRO_DISPLAY* disp = al_create_display(400, 600); //okno

    //ekrany startowe
    ALLEGRO_BITMAP* start_screen1 = al_load_bitmap("start_screen_1.png");
    ALLEGRO_BITMAP* start_screen2 = al_load_bitmap("start_screen_2.png");
    ALLEGRO_BITMAP* start_screen3 = al_load_bitmap("start_screen_3.png");

    ALLEGRO_BITMAP* end_screen = al_load_bitmap("end_screen.png"); //ekran koncowy

    ALLEGRO_BITMAP* road = al_load_bitmap("road.png"); //droga

    //auto gracza
    ALLEGRO_BITMAP* car1 = al_load_bitmap("main_car_1.png");
    ALLEGRO_BITMAP* car2 = al_load_bitmap("main_car_2.png");
    ALLEGRO_BITMAP* car3 = al_load_bitmap("main_car_3.png");

    //licznik piwa
    ALLEGRO_BITMAP* beer = al_load_bitmap("beer.png");
    ALLEGRO_BITMAP* b1 = al_load_bitmap("beer_count_1.png");
    ALLEGRO_BITMAP* b2 = al_load_bitmap("beer_count_2.png");
    ALLEGRO_BITMAP* b3 = al_load_bitmap("beer_count_3.png");
    ALLEGRO_BITMAP* b4 = al_load_bitmap("beer_count_4.png");

    //auto - przeszkoda
    ALLEGRO_BITMAP* obstacle1 = al_load_bitmap("car_obstacle_1.png");
    ALLEGRO_BITMAP* obstacle2 = al_load_bitmap("car_obstacle_2.png");
    ALLEGRO_BITMAP* obstacle3 = al_load_bitmap("car_obstacle_3.png");

    ALLEGRO_BITMAP* crash = al_load_bitmap("crash_test.png"); //wybuch

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(disp));
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    //licznik punktow naliczanych wedlug klatek
    int frame_count = 0;
    int score = 0;

    //licznik klatek drogi / auta gracza
    int road_frame = 0;
    int car_frame = 0;

    //pozycja startowa gracza
    float pos_x = 170;
    float pos_y = 480;

    //zmienne do sterowania strzalkami
    bool right_pressed = false;
    bool left_pressed = false;
    bool up_pressed = false;
    bool down_pressed = false;

    //zmienna do zamkniecia okna
    bool exit_pressed = false;

    //flaga ekranu startowego
    bool start_screen_visible = true;

    //flaga wyboru auta
    int car_choice = 1;

    //stworzenie piwa
    Beer beers[1];
    for(int i=0; i<1; i++){
        beers[i].x = (rand() % 3) * 110 + 60;
        beers[i].y = -((rand() % 300) + 2000);
        beers[i].speed = 4.5;
    }

    //flaga odwrocenia sterowania
    bool reverse_controls = false;
    float reverse_time = 0.0;

    //stworzenie przeszkod
    Obstacle obstacles[4];
    for(int i=0; i<4; i++){
        obstacles[i].x = (rand() % 3) * 110 + 55;
        if(i == 0){
            obstacles[i].y = -((rand() % 200) + 250);
        }else if(i>0){
            obstacles[i].y = obstacles[i-1].y - 250;
        }
        obstacles[i].speed = 5.0;
        obstacles[i].type = rand() % 3 + 1;
    }

    //zmienna do petli glownej
    bool game_over = false;

    while(!game_over) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break; //zamkniecie okna
        }

        if (event.type == ALLEGRO_EVENT_TIMER){
            //rysowanie
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //ekrany startowe -> wybor auta gracza
            if(start_screen_visible){
                if(car_choice == 1){
                    al_draw_bitmap(start_screen1, 0, 0, 0);
                }else if(car_choice == 2){
                    al_draw_bitmap(start_screen2, 0, 0, 0);
                }else if(car_choice == 3){
                    al_draw_bitmap(start_screen3, 0, 0, 0);
                }

                //wybor auta -> klawisze zwykle lub klawiatura numeryczna
                if(event.keyboard.keycode == ALLEGRO_KEY_1 || event.keyboard.keycode == ALLEGRO_KEY_PAD_1){
                    car_choice = 1;
                }else if(event.keyboard.keycode == ALLEGRO_KEY_2 || event.keyboard.keycode == ALLEGRO_KEY_PAD_2){
                    car_choice = 2;
                }else if(event.keyboard.keycode == ALLEGRO_KEY_3 || event.keyboard.keycode == ALLEGRO_KEY_PAD_3){
                    car_choice = 3;
                }

                //rozpoczecie gry po nacisnieciu P
                if (event.keyboard.keycode == ALLEGRO_KEY_P) {
                    start_screen_visible = false;
                }
            }else{
                //rysowanie drogi
                al_draw_bitmap_region(road, road_frame * 400, 0, 400, 600, 0, 0, 0);

                //rysowanie wybranego samochodu
                if(car_choice == 1){
                    al_draw_bitmap_region(car1, car_frame * 60, 0, 60, 100, pos_x, pos_y, 0);
                }else if(car_choice == 2){
                    al_draw_bitmap_region(car2, car_frame * 60, 0, 60, 100, pos_x, pos_y, 0);
                }else if(car_choice == 3){
                    al_draw_bitmap_region(car3, car_frame * 60, 0, 60, 100, pos_x, pos_y, 0);
                }

                //rysowanie piwa
                for(int i=0; i<1; i++){
                    beers[i].y = beers[i].y + beers[i].speed; //przesuwanie ikonki w dol
                    if(beers[i].y >= 600){ //generowanie na nowo po wyjsciu ikonki poza okno gry
                        beers[i].y = -((rand() % 300) + 2000);  //losowy y - generowanie poza oknem (ikonka wjezdza na ekran z gory)
                        beers[i].x = (rand() % 3) * 110 + 60; //ustalone x: 60, 170, 280
                    }

                    al_draw_bitmap(beer, beers[i].x, beers[i].y,0);

                    //sprawdzenie kolizji z piwem -> kolizja powoduje odwrocenie sterowania
                    if(BeerCollision(pos_x, pos_y, beers[i].x, beers[i].y, beer)){
                        beers[i].y = beers[i].y - 1800; //po kolizji ikonka jest wyrzucana poza ekran
                        reverse_controls = true;
                    }

                    //odwrocenie sterowania na 4 sekundy
                    if(reverse_controls && reverse_time >= 4.0){
                        reverse_controls = false;
                        reverse_time = 0.0; //reset licznika czasu
                    }

                    //wyswietlanie licznika sekund -> ile zostalo do konca odwrconego sterowania
                    if(reverse_controls){
                        ALLEGRO_BITMAP* beer_counter = NULL;
                        if(reverse_time >= 0.0 && reverse_time < 1.0){ // 0 - 1 sek -> zostaly 4 -> 4 ikonki piwa
                            beer_counter = b4;
                        }else if(reverse_time >= 1.0 && reverse_time < 2.0){ //1 - 2 sek -> zostaly 3
                            beer_counter = b3;
                        }else if(reverse_time >= 2.0 && reverse_time < 3.0){ //2 - 3 sek -> zostaly  2
                            beer_counter = b2;
                        }else if(reverse_time >= 3.0 && reverse_time < 4.0){ //3 - 4 sek ->zostala 1
                            beer_counter = b1;
                        }

                        //wyswietlanie odpowiedniej grafiki
                        al_draw_bitmap(beer_counter, 50, 15, 0);
                    }
                }

                //rysowanie przeszkod
                //losowanie 1 z 3 aut (rozne kolory)
                for(int i=0; i<4; i++){
                    ALLEGRO_BITMAP* obstacle_bitmap = NULL;
                    switch(obstacles[i].type){
                        case 1:
                            obstacle_bitmap = obstacle1;
                            break;
                        case 2:
                            obstacle_bitmap = obstacle2;
                            break;
                        case 3:
                            obstacle_bitmap = obstacle3;
                            break;
                    }
                }

                //rysowanie
                for(int i=0; i<4; i++){
                    obstacles[i].y = obstacles[i].y + obstacles[i].speed; //przesuwanie przeszkody w dol
                    if(obstacles[i].y >= 600){
                        if(i == 0){
                            obstacles[i].y = -((rand() % 200) + 250); //pierwsza przeszkoda generuje sie w losowym y (poza oknem gry na gorze)
                        }else if(i>0){
                            obstacles[i].y = obstacles[i-1].y - 250; //kazda nastepna przeszkoda generuje sie 250px za poprzednia -> odpowiednia przerwa zeby mozna bylo przejechac miedzy nimi
                        }

                        obstacles[i].x = (rand() % 3) * 110 + 55; //ustalone x: 55, 165, 275
                        obstacles[i].type = rand() % 3 + 1; //losowanie koloru
                    }

                    //rysowanie wylosowanej przeszkody
                    ALLEGRO_BITMAP* obstacle_bitmap = NULL;
                    switch(obstacles[i].type){
                        case 1:
                            obstacle_bitmap = obstacle1;
                            break;
                        case 2:
                            obstacle_bitmap = obstacle2;
                            break;
                        case 3:
                            obstacle_bitmap = obstacle3;
                            break;
                    }

                    al_draw_bitmap(obstacle_bitmap, obstacles[i].x, obstacles[i].y, 0);

                    //sprawdzanie kolizji
                    if(CheckCollision(pos_x, pos_y, obstacles[i].x, obstacles[i].y, obstacle_bitmap)){
                        al_draw_bitmap(crash, pos_x, pos_y, 0); //rysowanie wybuchu
                        game_over = true;

                        //zapisywanie najlepszego wyniku do pliku .txt
                        FILE* file = fopen("crazy_driver_highscore.txt", "r");


                        int bufor;
                        fscanf(file, "%d", &bufor);
                        fclose(file);

                        if(score > bufor){
                            file = fopen("crazy_driver_highscore.txt", "w");
                            fprintf(file, "%d", score);
                            fclose(file);
                        }

                        break;
                    }
                }

                //liczenie punktow wedlug klatek
                //80 klatek = 1.5 sek
                if(frame_count % 80 == 0){
                    score += 1;
                }

                //wyswietlanie wyniku na biezaco w rogu okna gry
                al_draw_textf(font, al_map_rgb(500,500,500), 270,15,0,"SCORE: %d", score);

                //nastepna klatka drogi i auta gracza
                road_frame = (road_frame + 1) % 10;
                car_frame = (car_frame + 1) % 23;

                //sterowanie strzalkami + ograniczenie auta do obszaru drogi
                if(reverse_controls){ //odwrocone sterowanie (po kolizji z piwem)
                    if(right_pressed){
                        if(pos_x - SPEED > 48){
                            pos_x -= SPEED;
                        }
                    }
                    if(left_pressed){
                        if(pos_x + SPEED < 292){
                            pos_x += SPEED;
                        }
                    }
                    if(up_pressed){
                        if(pos_y + SPEED < 495){
                            pos_y += SPEED;
                        }
                    }
                    if(down_pressed){
                        if(pos_y - SPEED > 5){
                            pos_y -= SPEED;
                        }
                    }

                    reverse_time += 1.0/60.0; //liczenie czasu odwrocenia sterowania (do 4 sekund)

                }else{ //normalne sterowanie
                    if(right_pressed){
                        if(pos_x + SPEED < 292){
                            pos_x += SPEED;
                        }
                    }
                    if(left_pressed){
                        if(pos_x - SPEED > 48){
                            pos_x -= SPEED;
                        }
                    }
                    if(up_pressed){
                        if(pos_y - SPEED > 5){
                            pos_y -= SPEED;
                        }
                    }
                    if(down_pressed){
                        if(pos_y + SPEED < 495){
                            pos_y += SPEED;
                        }
                    }
                }
            }

            frame_count++; //naliczanie klatek ogolnie -> na tej podstawie licza sie punkty

            al_flip_display();

        }

        //akcje klawiszy
        if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            if(reverse_controls){
                switch(event.keyboard.keycode) {
                    case ALLEGRO_KEY_RIGHT:
                        right_pressed = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        left_pressed = true;
                        break;
                    case ALLEGRO_KEY_UP:
                        up_pressed = true;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        down_pressed = true;
                        break;
                }
            }else{
                switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_RIGHT:
                        right_pressed = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        left_pressed = true;
                        break;
                    case ALLEGRO_KEY_UP:
                        up_pressed = true;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        down_pressed = true;
                        break;
                    case ALLEGRO_KEY_1:
                        car_choice = 1;
                        break;
                    case ALLEGRO_KEY_PAD_1:
                        car_choice = 1;
                        break;
                    case ALLEGRO_KEY_2:
                        car_choice = 2;
                        break;
                    case ALLEGRO_KEY_PAD_2:
                        car_choice = 2;
                        break;
                    case ALLEGRO_KEY_3:
                        car_choice = 3;
                        break;
                    case ALLEGRO_KEY_PAD_3:
                        car_choice = 3;
                        break;
                }
            }
        }

        if(event.type == ALLEGRO_EVENT_KEY_UP){
            if(reverse_controls){
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_RIGHT:
                        right_pressed = false;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        left_pressed = false;
                        break;
                    case ALLEGRO_KEY_UP:
                        up_pressed = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        down_pressed = false;
                        break;
                }
            }else{
                switch (event.keyboard.keycode){
                    case ALLEGRO_KEY_RIGHT:
                        right_pressed = false;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        left_pressed = false;
                        break;
                    case ALLEGRO_KEY_UP:
                        up_pressed = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        down_pressed = false;
                        break;
                    case ALLEGRO_KEY_P:
                        if (start_screen_visible) {
                            start_screen_visible = false;
                        }
                        break;
                    case ALLEGRO_KEY_E:
                        game_over = false;
                        break;
                }
            }
        }
    }

    while (game_over) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        //zamkniecie okna gry po nacisnieciu E
        if (event.type == ALLEGRO_EVENT_KEY_UP && event.keyboard.keycode == ALLEGRO_KEY_E) {
            game_over = false;
        }

        //wyewietlanie ekranu koncowego i wyniku
        al_draw_bitmap(end_screen, 0, 0, 0);
        al_draw_textf(font2, al_map_rgb(500,500,500), 220, 255, 0, "%d", score);
        al_flip_display();
    }

    al_destroy_bitmap(start_screen1);
    al_destroy_bitmap(start_screen2);
    al_destroy_bitmap(start_screen3);
    al_destroy_bitmap(road);
    al_destroy_bitmap(car1);
    al_destroy_bitmap(car2);
    al_destroy_bitmap(car3);
    al_destroy_bitmap(beer);
    al_destroy_bitmap(b1);
    al_destroy_bitmap(b2);
    al_destroy_bitmap(b3);
    al_destroy_bitmap(b4);
    al_destroy_bitmap(obstacle1);
    al_destroy_bitmap(obstacle2);
    al_destroy_bitmap(obstacle3);
    al_destroy_bitmap(crash);
    al_destroy_bitmap(end_screen);
    al_destroy_event_queue(event_queue);
    al_destroy_display(disp);
    al_destroy_timer(timer);

    return 0;
}
