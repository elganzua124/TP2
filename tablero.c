#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"

casillero_t tablero[8][8];

void pintar_casilleros(casillero_t tablero[8][8])
{
	short i, j;

	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
			tablero[i][j].color_casillero = ( (i%2) == (j%2) );
	}
}

//{TORRE, CABALLO, ALFIL, REINA, REY, PEON}
//{ROCK,  KNIGHT,  BISHOP, QUEEN, KING, PAWN}

void llenar_tablero(casillero_t tablero[8][8])
{
	short i;
	for( i = 0; i < 3; i++ )
	{
		tablero[i][0].pieza = calloc(1,sizeof(pieza_t));
		tablero[i][0].pieza->clase_pieza = i;
		tablero[i][0].pieza->color_pieza = 0;

		tablero[7-i][0].pieza = calloc(1,sizeof(pieza_t));
		tablero[7-i][0].pieza->clase_pieza = i;
		tablero[7-i][0].pieza->color_pieza = 0;

		tablero[i][7].pieza = calloc(1,sizeof(pieza_t));
		tablero[i][7].pieza->clase_pieza = i;
		tablero[i][7].pieza->color_pieza = 1;

		tablero[7-i][7].pieza = calloc(1,sizeof(pieza_t));
		tablero[7-i][7].pieza->clase_pieza = i;
		tablero[7-i][7].pieza->color_pieza = 1;
	}
	for( i = 0; i < 8; i++ ) // peones
	{
		tablero[i][1].pieza = calloc(1,sizeof(pieza_t));
		tablero[i][1].pieza->clase_pieza = 5;
		tablero[i][1].pieza->color_pieza = 0;

		tablero[i][6].pieza = calloc(1,sizeof(pieza_t));
		tablero[i][6].pieza->clase_pieza = 5;
		tablero[i][6].pieza->color_pieza = 1;
	}
	for( i = 3; i < 5; i++ ) // reyes
	{
		tablero[i][0].pieza = calloc(1,sizeof(pieza_t));
		tablero[i][0].pieza->clase_pieza = i;
		tablero[i][0].pieza->color_pieza = 0;

		tablero[i][7].pieza = calloc(1,sizeof(pieza_t));
		tablero[i][7].pieza->clase_pieza = i;
		tablero[i][7].pieza->color_pieza = 1;
	}
	short j;
	for( i = 2; i < 6; i++) // casilleros vacios
	{
		for( j = 0; j < 8; j++)
			tablero[j][i].pieza = NULL;
	}
}

void inic_tablero(casillero_t tablero[8][8])
{
	pintar_casilleros(tablero);
	llenar_tablero(tablero);
}
