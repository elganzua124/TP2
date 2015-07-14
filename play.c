#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"

void jugada_a_string(char cadena[66]);
void insertar_nodo(char *cadena, char movimiento[6], char *nombre_jugador);

void tablero_blanco(char cadena[66])
{
	int i, j,k=0;

	for( i=8; i>0; i-- )
	{
		puts("  +---+---+---+---+---+---+---+---+");
		printf("%d ", i);
		for( j=1; j<9; j++ )
			printf("| %c ", cadena[k++]);
		puts("|");
	}
	puts("  +---+---+---+---+---+---+---+---+");
	puts("    a   b   c   d   e   f   g   h");
}

void tablero_negro(char cadena[66])
{
	int i, j,k=63;
	for( i=1; i<9; i++ )
	{
		puts("  +---+---+---+---+---+---+---+---+");
		printf("%d ", i);
		for( j=1; j<9; j++ )
			printf("| %c ", cadena[k--]);
		puts("|");
	}
	puts("  +---+---+---+---+---+---+---+---+");
	puts("    h   g   f   e   d   c   b   a");
}

int chekar_coordenada(char coord[7])
{
	if( coord[5] != '\n' ) // cant de caracteres no validos, chequear si hay un '\n'
	{//funciona cuando el stream es stdin, pero no cuando es un archivo, en tal caso hay q presionar enter
		short i;
		for( i=0; i<5 && coord[i] != '\n'; i++ ); // si corta por i==5, no hay '\n', limpiar buffer

		if( i == 5 ) // no se encontro '\n', input too long
			while (getchar()!='\n'); // solo dsps de fgets, y solo si el buffer llenado por fgets no termina en '\n'
		return 1;
	}
	if( (coord[0] >= 'a' && coord[0] <= 'h') &&
		(coord[1] >= '1' && coord[1] <= '8') &&
		(coord[2] == '-') &&
		(coord[3] >= 'a' && coord[3] <= 'h') &&
		(coord[4] >= '1' && coord[4] <= '8') )
	{
		return 0;
	}
	return 1;
}

int mover_pieza(casillero_t tablero[8][8], color_t color_jugador, const char coord[7])
{
	int orig_col = coord[0] - 'a',
		orig_fil = coord[1] - '1',
		dest_col = coord[3] - 'a',
		dest_fil = coord[4] - '1';

	if( tablero[orig_col][orig_fil].pieza == NULL )
		return 2; // err: no hay pieza en origen
	if( tablero[orig_col][orig_fil].pieza->color_pieza != color_jugador )
		return 3; // err: en origen no hay pieza de jugador actual
	if( tablero[dest_col][dest_fil].pieza == NULL ) // ok, dest vacio
		tablero[dest_col][dest_fil].pieza = calloc(1,sizeof(pieza_t));
	else if( tablero[dest_col][dest_fil].pieza->color_pieza == color_jugador )
		return 4; // err: en destino ya hay pieza de jugador actual
	// si dest vacio, ya se aloco mem mas arriba, sino dest ocupado x oponente
	tablero[dest_col][dest_fil].pieza->clase_pieza = tablero[orig_col][orig_fil].pieza->clase_pieza;
	tablero[dest_col][dest_fil].pieza->color_pieza = tablero[orig_col][orig_fil].pieza->color_pieza;
	free( tablero[orig_col][orig_fil].pieza );
	tablero[orig_col][orig_fil].pieza = NULL;
	return 0;
}

void imprimir_error(short error)
{
	switch(error) {
	case 1 :
		puts("Error: formato o rango incorrectos.");
		break;
	case 2 :
		puts("Error: no hay pieza en origen");
		break;
	case 3 :
		puts("Error: en origen no hay pieza de jugador actual");
		break;
	case 4 :
		puts("Error: en destino ya hay pieza de jugador actual");
		break;
	}
}

void play_game(casillero_t tablero[8][8], char cadena[66], short cod)
{
	short codigo;

	color_t color_jugador = NEGRO;

	jugada_a_string(cadena); // en move_snapshot.c
	char coord[7]; 

	for( ;; )
	{
		if( (color_jugador = !color_jugador) ) // va de 0 a 1 a 0 a 1...
		{
			tablero_negro(cadena);
			printf("\nTurno de %s.\n", nombre_negras);
		} else {
			tablero_blanco(cadena);
			printf("\nTurno de %s.\n", nombre_blancas);
		}
		puts("Por favor, ingrese su movimiento, presione \"q\" para salir: ");

		fgets(coord, 7, stdin); // lee coordenadas
		codigo = chekar_coordenada(coord); // chekea formato de coords

		if( coord[0] == 'q' && coord[1] == '\n') // para salir
			return;

		if(codigo == 0) // si es valido el formato
			codigo = mover_pieza(tablero,color_jugador,coord); // chekar en tablero

		while(codigo!= 0 && codigo != 5)
		{
			imprimir_error( codigo );
			fgets(coord, 7, stdin);
			codigo = chekar_coordenada(coord);

			if( coord[0] == 'q' && coord[1] == '\n') // para salir
				return;

			if(codigo == 0)
				codigo = mover_pieza(tablero,color_jugador,coord);
		}
		jugada_a_string(cadena);

		if(cod == 4) // cargar en lista
		{
			if(color_jugador == 0)
				insertar_nodo(cadena,coord,nombre_blancas);
			else
				insertar_nodo(cadena,coord,nombre_negras);
		}
	}
}
