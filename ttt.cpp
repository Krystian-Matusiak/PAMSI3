#include "tab.h"
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <time.h>
#include <cmath>


#define krzyzyk_png     "./images/cross.png"
#define kolko_png    "./images/circle.png"
#define kwadrat_png "./images/kwadrat.png"


float wsp_x;                // współrzędne x i y zapamiętujące współrzędne "pikselowe" myszki
float wsp_y; 

int indeks[2];            // tablica dwóch zmiennych zapamiętująca współrzędne wynikowe z algorytmu minimax
//int ex=0;                 // zmienna pomocnicza, dzięki której program zlicza ilość wykonań algorytmu minimax


/**********************************************************
 * Funkcja odpowiedzialna za zatrzymanie programu         *
 * na określony czas.Potrzebne do tworzenia interfejsu    *
 * graficznego, aby nie pojawiały się błędy               *
 **********************************************************/
void wait(float seconds){   
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
}




/********************************************
 * Funkcja realizująca algorytm MiniMax     *
 * zwracająca wartości heurystyczne:        *
 * 10 - wygrana                             *
 * 5  - 3 znaki wykonującego ruch w rzędzie *
 * 3  - 2 znaki wykonującego ruch w rzędzie *
 * 0  - remis                               *
 * -3  - 2 znaki wroga w rzędzie            *
 * -5  - 3 znaki wroga w rzędzie            *
 * -10  - przegrana                         *
 ********************************************/
int MiniMax(bool flag , int n, Tablica * T, int alfa, int beta , int gleb){
   
  int MAX=-1000;
  int MIN=1000;
  
  int wartosc = 1;
  
  
  //cout<<ex<<endl;
  //ex++;
  
  if( gleb == 0 ){
    if( T->x_dwa(n) == 1 )
      return 3;
    else if( T->o_dwa(n) == 1 )
      return -3;
    else
      return 0;
  }
  
  if( T->x_wygral(n) == 1)
    return 10;
  else if( T->o_wygral(n) == 1)
    return -10;
  else if( T->pelnyTab(n)== 1)
    return 0;
  
  
  int ** tmp;
  tmp = new int* [n];
  for( int p=0 ; p<n ; p++ )
    tmp[p] = new int [n];
    
  for( int p=0 ; p<n ; p++ )
    for( int q=0 ; q<n ; q++ ){
      if(flag == true)
	tmp[p][q] = -1000;
      else
	tmp[p][q] = 1000;
    }
  
  for( int p=0 ; p<n ; p++ )
    for( int q=0 ; q<n ; q++ ){
      if( T->zwracanie_tab()[p][q] == '*'){ 

	if(flag == true){
	  T->zwracanie_tab()[p][q]= 'X';
	  wartosc = MiniMax( false , n, T , alfa , beta, gleb-1 );
	  
	  if( alfa < wartosc )
	    alfa=wartosc;
	  
	}
	
	else{
	  T->zwracanie_tab()[p][q] = 'O';
	  wartosc = MiniMax( true , n, T , alfa , beta , gleb-1 );
	  
	  if( beta > wartosc )
	    beta = wartosc;
	  
	}
	
	T->zwracanie_tab()[p][q] = '*';
	tmp[p][q] = wartosc;

	if( alfa > beta ){  // Cięcie alfa-beta
	  p=n;
	  q=n;
	}
      }
    }

  /////////////////////////////////////////////////////////////////
  if(flag == true){
  
    MAX  = -1000;
    for( int p=0 ; p<n ; p++ )
      for( int q=0 ; q<n ; q++ ){
	if( tmp[p][q] > MAX && tmp[p][q] != -1000){
	  MAX = tmp[p][q];
	  indeks[0] = p;
	  indeks[1] = q;
	}
      }
    
    for( int p=0 ; p<n ; p++ )
      delete [] tmp[p];
    delete [] tmp;

    return MAX;
  }
  if(flag == false){
    
    MIN = 1000;
    for( int p=0 ; p<n ; p++ )
      for( int q=0 ; q<n ; q++ ){
	if( tmp[p][q] < MIN && tmp[p][q] != 1000){
	  MIN = tmp[p][q];
	  indeks[0] = p;
	  indeks[1] = q;
	}
      }
    
    for( int p=0 ; p<n ; p++ )
      delete [] tmp[p];
    delete [] tmp;

    return MIN;
  }
  /////////////////////////////////////////////////////////////////
  
  
}

// Funkcja realizująca ruch gracza. Pobierane są współrzędne "pikselowe"
// i odpowiednio przetwarzane zaraz po wykonaniu funkcji.
void ruch_gracza( sf::RenderWindow &okno , float rozmiar){
  bool czy_ruch = false ;
  sf::Event wydarz;
  while( czy_ruch == false ){
    okno.waitEvent( wydarz );
    switch( wydarz.type ){
      
    case sf::Event::Closed:
      czy_ruch= true;
      okno.close();
      break;
      
    case sf::Event::MouseButtonPressed:
      if(wydarz.mouseButton.button == sf::Mouse::Left){
	wsp_x = wydarz.mouseButton.x / rozmiar;
	wsp_y = wydarz.mouseButton.y / rozmiar;

	
	czy_ruch = true;
      }
      break;
      
    }
  }
}




/******************************************************************************
 *                                    MAIN                                    *
 *                                                                            *
 * Gracz początkowo wprowadza rozmiar planszy kwadratowej (3x3,4x4,5x5,...).  *
 * Następnie ilość znaków potrzebnych do wygranej oraz kto ma                 *
 * ma zacząć - gracz (znak O) czy komputer (znak X). Następnie ukazuje się    *
 * plansza. Gdy grasz ma swoją kolej na wprowadzenie ruchu, należy nacisnąć   *
 * lewym przyciskiem myszki w kwadrat, w którym chce wprowadzić swój znak O.  *
 * gdy naciśniemy planszę podczas ruchu komputera, ruch zostanie zapamiętany. *
 * Należy więc czekać na swoją kolej, aby nie pojawiały się problemy.         *
 ******************************************************************************/
int main(){


  
  Tablica *T;               // klasa odpowiedzialna za przetrzymywanie rozmiaru, ilości znaków w rzędzie (aby wygrać) i funkcje sprawdzające stan gry (wygrana itp)
  T = new Tablica;

  
  int blad=0;               // zmienna potrzeba do sprawdzania czy nie popełniono błędu podczas wprowadzania ruchu
  int roz;                  // zmienna, do której przypisywany jest rozmiar planszy
  int ilosc;                // zmienna mówiąca ile znaków w rzędzie wygrywa gre
  int move;                 // zmienna odpowiadająca za wspolrzedne umieszczenia O/X
  int ruch_wier , ruch_kol; // zmienna zawierająca dane o indeksie wiersza i kolumny - potrzebne do wykonania ruchu
  int wybor;                // zmienna pomocnicza; będzie przyjmować wartości 1/2
  int il_ruchow=0;          // określa ile ruchów wykonał komputer
  int kwad_20proc = 414;    // zawiera ilość pikseli w rzędzie/kolumnie znajdujących się w pierwszych 20% obrazka "kwadrat"
  int roz_kwad = 414;       // rozmiar kwadratu - daleszej części programu jest on zeskalowany
  float skala;              // skala - potrzebne do skalowania kwadratu aby (niezależnie od rozmiaru) plansza zawsze się mieściła na ekranie
  int glebokosc;            // wartość, która zmienia się podczas działania programu ze względu na wielkość planszy
 
  
  sf::Texture krzyzyk_;     
  sf::Texture kolko_;       
  sf::Texture kwadrat_;

  if( !kwadrat_.loadFromFile ( kwadrat_png ) )  // Pobieranie zdjęć z pliku images.
    return 0;
  if( !krzyzyk_.loadFromFile ( krzyzyk_png ) )
    return 0;
  if( !kolko_.loadFromFile   ( kolko_png   ) )
    return 0;



  
  // Poberanie romiaru planszy
  do{
    cout << "Podaj rozmiar (minimum 3):\n";
    cin >> roz;
    if( roz<3 )
      cout << "Zbyt maly rozmiar\n";
  }while( roz<3 );
  T->stworz_tab(roz);
  T->rysuj_tab(roz);


  sf::Sprite kwadrat;
  sf::Sprite kolko;
  sf::Sprite krzyzyk;

  skala = (float)1/roz;            // skalowanie planszy
  roz_kwad = roz_kwad/roz;         // ustawianie rozmiaru planszy aby mieściła się na ekranie
  kwad_20proc = roz_kwad * 0.2;
  
  kwadrat.setTexture(kwadrat_);
  kwadrat.setScale( skala , skala );


  /////////////////////////////////////
  // Ustawienia kółka i krzyżyka (tekstura i skala)
  
  kolko.setTexture (kolko_);
  kolko.setScale   ( (float)(roz_kwad - kwad_20proc)/718 , (float)(roz_kwad - kwad_20proc)/718 );
    
  krzyzyk.setTexture (krzyzyk_);
  krzyzyk.setScale   (  (float)(roz_kwad - kwad_20proc)/981 , (float)(roz_kwad - kwad_20proc)/981 );
  
  ////////////////////////////////////
  
  
 

  // Pobieranie informacji dotyczącej ilości znaków potrzebnych do wygranej
  do{
    cout << "Podaj ile znakow w rzedzie wygrywa gre\n";
    cout << "(minimum 3 maksimum " << roz << ")\n";
    cin >> ilosc;
    if( ilosc<3 || ilosc>roz )
      cout << "Zly zakres\n";
  }while( ilosc<3 || ilosc>roz );
  T->il_zn( ilosc );
  
  cout << endl << "GRACZ--->(O)";
  cout << endl << "AI----->(X)";
  do{
    cout << endl << "\nWybierz:";
    cout << endl << "1-gracz zaczyna";
    cout << endl << "2-komputer zaczyna\n";
    cin >> wybor;
    
    if( wybor != 1 && wybor != 2 )
      cout << "Bledny wybor\n";
  }while( wybor != 1 && wybor != 2 );



  // Tworzenie interfejsu graficznego gry
  
  sf::RenderWindow okno(
			sf::VideoMode( roz*roz_kwad , roz*roz_kwad ),
			"Tic Tac Toe"
			);
  okno.draw( kwadrat );
  okno.display();

  
  for (int yp=0 ; yp<roz ; yp++ )
    for ( int xp=0 ; xp<roz ; xp++){
      kwadrat.setPosition( xp*roz_kwad , yp*roz_kwad );
      okno.draw( kwadrat );
      okno.display();
      wait(0.03);
    }
  
  sf::Event wydarzenie;
  


  
  //////////////////////////////////////////////////
  /*     Główna część kodu odpowiadająca za grę   */
  //////////////////////////////////////////////////







  
  if(true){
    if(wybor == 1){ // Jeśli zaczyna gracz

      cout << "\nWprowadz ruch. Nacisnij myszka w odpowiednie miejsce";

      for( bool bez_bledu=false ; !bez_bledu ; ){
	ruch_gracza(okno , roz_kwad);
	
	kolko.setPosition (
			   kwad_20proc/2 + roz_kwad*( (int)wsp_x) ,
			   kwad_20proc/2 + roz_kwad*( (int)wsp_y)
			   );
	if(
	   T->zwracanie_tab()[ (int)wsp_y ][ (int)wsp_x ] == 'X' ||
	   T->zwracanie_tab()[ (int)wsp_y ][ (int)wsp_x ] == 'O'
	   ){
	  cout << "\nPole zajete. Wykonaj ruch jeszcze raz.\n";
	}
	else
	  bez_bledu = true;
      }
      
      T->ruch( (int)wsp_y , (int)wsp_x  , 'O');
      T->rysuj_tab(roz);
      okno.draw( kolko );
      okno.display();
    }

    
    while(true){  // Ruch komputera

      wait(0.3);
      cout<<endl<<"Ruch komputera:";

      // dzięki temu warunkowi pierwszy ruch komputera
      // jest z góry ustawiony na najlepszą możliwość w
      // celu skrócenia czasu działania programu
      if( il_ruchow == 0 ){  
	if( T->zwracanie_tab()[1][1] == '*' ){
	  T->ruch( 1 , 1 ,'X' );
	  T->rysuj_tab(roz);
	  krzyzyk.setPosition (
			       kwad_20proc/2 + roz_kwad ,
			       kwad_20proc/2 + roz_kwad
			       );
	}
	else if( T->zwracanie_tab()[0][0] == '*' ){
	  T->ruch( 0 , 0 ,'X' );
	  T->rysuj_tab(roz);

	  krzyzyk.setPosition (
			       kwad_20proc/2 + 0 ,
			       kwad_20proc/2 + 0
			       );
	}
	
	il_ruchow = 1;
	okno.draw( krzyzyk );
	okno.display();
      }

      
      else{

	if( roz >3 ){ 
	  glebokosc = 100/sqrt(10*T->il_wol_miejsc(roz));
	  if( glebokosc > 10 )
	    glebokosc = 10;
	}
	
	auto q1 = chrono::high_resolution_clock::now() ;

	if( roz >3 )  
	  MiniMax(true , roz , T , -1000 , 1000 , glebokosc );
	else
	  MiniMax(true , roz , T , -1000 , 1000 , roz*roz);
	
	auto q2 = chrono::high_resolution_clock::now() ;
	auto A = chrono::duration_cast<std::chrono::microseconds>( q2-q1 ).count();
	
	cout << "\nCzas: " << A/1000 <<endl;
	
	T->ruch( indeks[0], indeks[1] ,'X' );
	T->rysuj_tab(roz);

	krzyzyk.setPosition (
			     kwad_20proc/2 + roz_kwad*(indeks[1]) ,
			     kwad_20proc/2 + roz_kwad*(indeks[0])
			     );
      
	okno.draw( krzyzyk );
	okno.display();
      }
      
      if( T->x_wygral(roz)==1){
	cout<<endl<<"Komputer wygral\n";
	return 0;
      }
      if( T->pelnyTab(roz)==1){
	cout<<endl<<"Zremisowales\n";
	return 0;
      }



      // Kolejny ruch gracza
     
      cout << "\nWprowadz ruch. Nacisnij myszka w odpowiednie miejsce";

      for( bool bez_bledu=false ; !bez_bledu ; ){
	ruch_gracza(okno , roz_kwad);
	
	kolko.setPosition (
			   kwad_20proc/2 + roz_kwad*( (int)wsp_x) ,
			   kwad_20proc/2 + roz_kwad*( (int)wsp_y)
			   );
	if(
	   T->zwracanie_tab()[ (int)wsp_y ][ (int)wsp_x ] == 'X' ||
	   T->zwracanie_tab()[ (int)wsp_y ][ (int)wsp_x ] == 'O'
	   ){
	  cout << "\nPole zajete. Wykonaj ruch jeszcze raz.\n";
	}
	else
	  bez_bledu = true;
      }
      
      
      T->ruch( (int)wsp_y , (int)wsp_x  , 'O');
      T->rysuj_tab(roz);
      okno.draw( kolko );
      okno.display();
      
      if( T->o_wygral(roz)==1){
	cout<<endl<<"Wygrales\n";
        return 0;
      }
      if( T->pelnyTab(roz) ){
	cout<<endl<<"Zremisowales\n";
	return 0;
      }
    }
  }
  
  return 0;
  
}
  
