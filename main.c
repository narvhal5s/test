// Główny moduł sterujacy

//Jego podstawowym zadaniem jest wczytanie danych konfiguracyjnych
//Nastepnie bedzie wywoływał kolejne moduly

#include "field/field.h"
#include "game/game.h" 


int main( int argc, char **argv){

	//deklaracja parametrow ktore zostana wczytane
		
	int width ;
	int height ;
	int gen_counter ;
	char *load_type ;
	char *load_detail ;
	char *save_to ;
	char *loaded_rules ;
	
	//Jezeli nie podano argumentow to wczytane sa domyslne

	if(argc == 1){
			printf("Wczytane zostaną parametry domyslne\n");
			width = 100;
			height = 100;
			gen_counter = 200;
			load_type = "random" ;
			load_detail = "25" ;	
			save_to = "gif";
			loaded_rules = "23/3";
		}

	//Jeżeli podano 7 argumetnow to program interpretuje je jako parametry konfiguracyjne podane w odpowiedniej kolejnosci

	else if( argc == 8){
		printf("Podane wsadowo parametry domyślne zostana wczytane\n");
		width = atoi(argv[1]) ;
		height = atoi(argv[2]) ;
		gen_counter = atoi(argv[3]) ;
		load_type = argv[4] ;
		load_detail = argv[5] ;
		save_to = argv[6] ;
		loaded_rules = argv[7] ; 
	}

	//Jezeli powyzsze warunki sa niespelnione program wyswietla odpowiedni blad
	
	else{
		printf ("Podano nieprawidlowa liczbe argumentow wywolanie\n") ;
		printf ("Prosze podac dokladnie 87 parametrow\n") ;
		return 1 ;
	}


	//Sprawdzenie poprawnosci argumentow

	if(width > 300 || width < 1){
		printf("Argument width wykracza poza prawidlowy przedzial\nWczytano wartosc domyslna\n");
		width = 100 ; 
	}	
	if(height > 300 || height < 1){
		printf("Argument height wykracza poza prawidlowy przedzial\nWczytano wartosc domyslna\n");
		height = 100 ; 
	}	
	if(gen_counter > 1000 || gen_counter < 1){
		printf("Argument gen_counter wykracza poza prawidlowy przedzial\nWczytano wartosc domyslna\n");
		gen_counter = 200 ; 
	}	
	if(strcmp(load_type , "png") == 0 || strcmp(load_type , "txt") == 0 ){
		FILE *input = fopen( load_detail , "r");
		if( input == NULL){
			printf("Nie udalo sie otworzyc pliku %s, uzyty zostanie losowe wypelnienie planszy\n" , load_detail ) ;
			 load_type = "random" ;
			 load_detail = "25" ; 
		}
		else{
			fclose(input);
		}
	}
	else if( strcmp(load_type , "random") == 0 && atoi(load_detail) <=100 && atoi(load_detail) >= 0 ){
		;
	}
	else{
		printf("Nieprawidlowy argument load_type i/lub load_detail\n");
	}
	if( strcmp(save_to , "png") != 0 && strcmp(save_to , "gif") != 0 && strcmp(save_to , "txt") != 0){
		printf("Argument save_to jest nieprawidlowy( prawidłowe: png,txt,gif)\nWczytano wartosc domyslna\n");
		save_to = "txt" ; 
	}
	
	// Ponizej weryfikacja poprawnosci parametru rules
	
	int rules_check = 0 ;

	int i ;
	for( i = 0 ; i < strlen(loaded_rules) ; i++ ){
		if( (loaded_rules[i] < '0' ||  loaded_rules[i] > '8')  && loaded_rules[i] != '/' ){
			printf("Argument rules jest nieprawidlowy ");
			rules_check = 1 ;
			break;
		}
	}

	//Jezli parametr rules zawiera bledne dane zostanie zresetowny do domyslnych danych

	if(rules_check == 1){
		printf("Wczytano wartosc domyslna (23/3)\n");
		loaded_rules = "23/3";
	}
	
	//Stworzenie struktury rules oraz wczytanie jej wartosci 	
	Rules rules ;
	rules = read_rules( loaded_rules , rules ) ;
	
	int error_chec = 0 ; 
	
	//Wywolanie modulu field
	Cell ***field = field_control( width , height , load_type , load_detail );

	//Wywolanie modulu game 	
	error_chec = play_game( field , width , height  , rules , save_to , gen_counter);	
	if( error_chec != 0 )
		printf("Podczas symulacji gry wystapil blad\n" )  ;
	
	//Finalizacja programu , sprzatanie
	error_chec = clear_memory( field , width , height , rules ) ;
	if( error_chec != 0 )
		printf("Wystapil blad przy zwalnianiu pamiec\n") ;


	return 0;

}
