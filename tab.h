#pragma once

#include <iostream>

using namespace std;


/***************************************************************
 * Klasa Tablica posiada wszystkie potrzebne zmienne i funkcje *
 * do stworzenia planszy oraz sprawdzania/zmieniania jej stanu *
 ***************************************************************/

class Tablica{

  char **tab;    // tablica dynamiczna pełniąca funkcję planszy
  int il_znakow; // zmienna mówiąca ile znaków w rzędzie wygrywa gre
  
 public:
 
  ~Tablica();

  int il_wol_miejsc(int n);
  void stworz_tab(int n);
  void rysuj_tab(int n);
  void ruch( int wier , int kol , char ox);  
  char ** zwracanie_tab(){ return tab;};     // funkcja zwracająca tablicę dynamiczną, aby można było sprawdzać jej elementy
  void il_zn(int x){ il_znakow = x; };       // funkcja potrzebna do pobierania informacji o ilości znaków potrzebnych do wygrania
  
  bool pelnyTab(int n);
  bool x_wygral(int n);
  bool o_wygral(int n);
  bool x_dwa   (int n);
  bool x_trzy  (int n);
  bool o_dwa   (int n);
  bool o_trzy  (int n);
  
};


// Destruktor dealokujący pamięc tablicy dynamicznej **tab.
Tablica::~Tablica(){
  int dlugosc=0;
  do{
    if( tab[0][dlugosc] != '\0' )
      dlugosc++;
  }while( tab[0][dlugosc] != '\0' );
	   
  for( int i=0 ; i<dlugosc ; i++)
    delete [] tab[i];
  delete [] tab;
}

// Funkcja sprawdzająca ilość wolnych miejsc na planszy ( tzn bez X oraz O ).
int Tablica::il_wol_miejsc(int n){
  int ilosc=0;
  for( int i=0 ; i<n ; i++)
      for( int j=0 ; j<n ; j++)
	if ( tab[i][j] == '*' )
	  ilosc++;
     
  return ilosc;
}

// Funkcja tworząca tablice dynamiczną o rozmiarze nxn. Wymiar pobierany poprzez argument.
void Tablica::stworz_tab(int n){
  tab = new char* [n];
  for( int i=0 ; i<n ; i++)
    tab[i] = new char [n];
  
  for( int i=0 ; i<n ; i++)
    for( int j=0 ; j<n ; j++)
      tab[i][j] = '*';
}

// Funkcja pokazująca dany stan planszy na terminalu. Potrzebna
// w poczatkowych wersjach programu oraz służy do porównania
// z obrazem interfejsu graficznego.
void Tablica::rysuj_tab(int n){
  
    cout << "\n\n" ;
    for( int i=0 ; i<n ; i++){
      for( int j=0 ; j<n ; j++){
	cout << tab[i][j] << "\t|\t" ;
      }
      cout<<endl;
      for( int k=0 ; k<(2*(n-1)+1) ; k++)
	cout << "--------" ;
      cout<<endl;
    }
    cout<<"\n\n\n";
}

// Funkcja przyjmująca w argumentach współrzędne
// planszy, w których ma być dany znak oraz owy znak (X lub O).
void Tablica::ruch(int wier , int kol , char ox){
  tab[wier][kol] = ox ;
}

// Funkcja zwracająca 1 jeśli plansza jest cała wypełniona
// znakami (O oraz X), zaś w przeciwnym wypadku zwróci zero.
bool Tablica::pelnyTab(int n){
  for( int i=0 ;i<n ; i++)
      for( int j=0 ; j<n ; j++){
        if( tab[i][j] != 'X'){
	  if( tab[i][j] != 'O'){
	    return 0;
	  }
        }
      }
  
  return 1;
}

// Funkcja zwracająca 1 jeśli X wygrał oraz 0 gdy nie wygrał.
bool Tablica::x_wygral(int n){

  int ilosc=1;
  for( int i=0 ;i<n ; i++) // Sprawdzanie czy są w rzędzie
    for( int j=0 ; j<(n-il_znakow+1) ; j++){
      
      for( int k=1 ; k<il_znakow ; k++)
	if( tab[i][j] == 'X' && tab[i][j] == tab[i][j+k] )
	  ilosc++;
	
      
      if( ilosc == il_znakow )
	return 1;
      
      ilosc=1;
      
    }
  ilosc=1;
  for( int i=0 ; i<(n-il_znakow+1) ; i++) // Sprawdzanie czy są w kolumnie
    for( int j=0 ; j<n ; j++){
      
      for( int k=1 ; k<il_znakow ; k++)
	if( tab[i][j] == 'X' && tab[i][j] == tab[i+k][j] )
	  ilosc++;
      
      
      if( ilosc == il_znakow )
	return 1;
      
      ilosc=1;
      
    }
  ilosc=1;
  for( int i=0 ; i<(n-il_znakow+1) ; i++) // Sprawdzanie czy są na ukos
    for( int j=0 ; j<(n-il_znakow+1) ; j++){
      
      for( int k=1 ; k<il_znakow ; k++ )
	if(tab[i][j]=='X' && tab[i][j]==tab[i+k][j+k] )
	  ilosc++;
      
      
      if( ilosc == il_znakow )
	return 1;
      
      ilosc=1;
    }
  ilosc=1;
  for( int i=(il_znakow-1) ; i<n ; i++) // Sprawdzanie czy są na ukos
    for( int j=0 ; j<(n-il_znakow+1) ; j++){
      
      for( int k=1 ; k<il_znakow ; k++ )
	if(tab[i][j]=='X' && tab[i][j]==tab[i-k][j+k] )
	  ilosc++;
      
      if( ilosc == il_znakow )
	return 1;
      
      ilosc=1;
    }
  
  return 0;
}

// Funkcja zwracająca 1 jeśli O wygrał oraz 0 gdy nie wygrał.
bool Tablica::o_wygral(int n){
  
  int ilosc=1;
  for( int i=0 ;i<n ; i++) // Sprawdzanie czy są w rzędzie
    for( int j=0 ; j<(n-il_znakow+1) ; j++){

      for( int k=1 ; k<il_znakow ; k++)
	if( tab[i][j] == 'O' && tab[i][j] == tab[i][j+k] )
	  ilosc++;

      if( ilosc == il_znakow )
	return 1;
      ilosc=1;
    }
  ilosc=1;
  for( int i=0 ; i<(n-il_znakow+1) ; i++) // Sprawdzanie czy są w kolumnie
    for( int j=0 ; j<n ; j++){
      
      for( int k=1 ; k<il_znakow ; k++)
	if( tab[i][j] == 'O' && tab[i][j] == tab[i+k][j] )
	  ilosc++;
      
      if( ilosc == il_znakow )
	return 1;
      ilosc=1;
    }
  ilosc=1;
  for( int i=0 ; i<(n-il_znakow+1) ; i++) // Sprawdzanie czy są na ukos
    for( int j=0 ; j<(n-il_znakow+1) ; j++){

      for( int k=1 ; k<il_znakow ; k++ )
	if(tab[i][j]=='O' && tab[i][j]==tab[i+k][j+k] )
	  ilosc++;

      if( ilosc == il_znakow )
	return 1;
      ilosc=1;
    }
  ilosc=1;
  for( int i=(il_znakow-1) ; i<n ; i++) // Sprawdzanie czy są na ukos
    for( int j=0 ; j<(n-il_znakow+1) ; j++){
      
      for( int k=1 ; k<il_znakow ; k++ )
	if(tab[i][j]=='O' && tab[i][j]==tab[i-k][j+k] )
	  ilosc++;
      
      if( ilosc == il_znakow )
	return 1;
      ilosc=1;
    }
  
  return 0;
}

// Funkcja zwracająca 1 jeśli w rzędzie/kolumnie/ukosie
// znajdują się 2 znaki X.
bool Tablica::x_dwa(int n){
  
  for( int i=0 ;i<n ; i++) // Sprawdzanie czy są dwa w rzędzie
    for( int j=0 ; j<(n-2+1) ; j++)
	if( tab[i][j] == 'X' && tab[i][j] == tab[i][j+1] )
	  return true;
  
  for( int i=0 ; i<(n-2+1) ; i++) // Sprawdzanie czy są dwa w kolumnie
    for( int j=0 ; j<n ; j++)
      if( tab[i][j] == 'X' && tab[i][j] == tab[i+1][j] )
	return true;
  
  for( int i=0 ; i<(n-2+1) ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-2+1) ; j++)
      if( tab[i][j] == 'X' && tab[i][j] == tab[i+1][j+1] )
	return true;
  
  for( int i=1 ; i<n ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-2+1) ; j++)
      if( tab[i][j] == 'X' && tab[i][j] == tab[i-1][j+1] )
	return true;
  
  return false;
}

// Funkcja zwracająca 1 jeśli w rzędzie/kolumnie/ukosie
// znajdują się 3 znaki X.
bool Tablica::x_trzy(int n){
  
  for( int i=0 ;i<n ; i++) // Sprawdzanie czy są dwa w rzędzie
    for( int j=0 ; j<(n-3+1) ; j++)
	if( tab[i][j] == 'X' && tab[i][j] == tab[i][j+1]  && tab[i][j] == tab[i][j+2] )
	  return true;
  
  for( int i=0 ; i<(n-3+1) ; i++) // Sprawdzanie czy są dwa w kolumnie
    for( int j=0 ; j<n ; j++)
      if( tab[i][j] == 'X' && tab[i][j] == tab[i+1][j]  && tab[i][j] == tab[i+2][j]  )
	return true;
  
  for( int i=0 ; i<(n-3+1) ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-3+1) ; j++)
      if( tab[i][j] == 'X' && tab[i][j] == tab[i+1][j+1] && tab[i][j] == tab[i+2][j+2]  )
	return true;
  
  for( int i=2 ; i<n ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-3+1) ; j++)
      if( tab[i][j] == 'X' && tab[i][j] == tab[i-1][j+1] && tab[i][j] == tab[i-2][j+2] )
	return true;
  
  return false;
}

// Funkcja zwracająca 1 jeśli w rzędzie/kolumnie/ukosie
// znajdują się 2 znaki O.
bool Tablica::o_dwa(int n){
    
  for( int i=0 ;i<n ; i++) // Sprawdzanie czy są dwa w rzędzie
    for( int j=0 ; j<(n-2+1) ; j++)
	if( tab[i][j] == 'O' && tab[i][j] == tab[i][j+1] )
	  return true;
  
  for( int i=0 ; i<(n-2+1) ; i++) // Sprawdzanie czy są dwa w kolumnie
    for( int j=0 ; j<n ; j++)
      if( tab[i][j] == 'O' && tab[i][j] == tab[i+1][j] )
	return true;
    
  for( int i=0 ; i<(n-2+1) ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-2+1) ; j++)
      if( tab[i][j] == 'O' && tab[i][j] == tab[i+1][j+1] )
	return true;
  
  for( int i=1 ; i<n ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-2+1) ; j++)
      if( tab[i][j] == 'O' && tab[i][j] == tab[i-1][j+1] )
	return true;

  return false;
}

// Funkcja zwracająca 1 jeśli w rzędzie/kolumnie/ukosie
// znajdują się O znaki O.
bool Tablica::o_trzy(int n){
  
  for( int i=0 ;i<n ; i++) // Sprawdzanie czy są dwa w rzędzie
    for( int j=0 ; j<(n-3+1) ; j++)
	if( tab[i][j] == 'O' && tab[i][j] == tab[i][j+1]  && tab[i][j] == tab[i][j+2] )
	  return true;
  
  for( int i=0 ; i<(n-3+1) ; i++) // Sprawdzanie czy są dwa w kolumnie
    for( int j=0 ; j<n ; j++)
      if( tab[i][j] == 'O' && tab[i][j] == tab[i+1][j]  && tab[i][j] == tab[i+2][j]  )
	return true;
  
  for( int i=0 ; i<(n-3+1) ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-3+1) ; j++)
      if( tab[i][j] == 'O' && tab[i][j] == tab[i+1][j+1] && tab[i][j] == tab[i+2][j+2]  )
	return true;
  
  for( int i=2 ; i<n ; i++) // Sprawdzanie czy są dwa w ukosie
    for( int j=0 ; j<(n-3+1) ; j++)
      if( tab[i][j] == 'O' && tab[i][j] == tab[i-1][j+1] && tab[i][j] == tab[i-2][j+2] )
	return true;
  
  return false;
}
