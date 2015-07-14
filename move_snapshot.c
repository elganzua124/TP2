#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"

char cadena[66];

char clase_p(int num)
{
	switch (num) {
	case 0:
		return 'r';
		break;
	case 1:
		return 'n';
		break;
	case 2:
		return 'b';
		break;
	case 3:
		return 'q';
		break;
	case 4:
		return 'k';
		break;
	case 5:
		return 'p';
		break;
	default:
		return 'z';
	}
}

void jugada_a_string(char cadena[66])
{
	int i, j,k=63;
	for( i=0; i<8; i++ )
	{
		for( j=7; j>=0; j-- )
		{
			if(tablero[j][i].pieza) // si hay pieza en casillero
			{
				if( tablero[j][i].pieza->color_pieza ) // si pieza es negra
					cadena[k--] = clase_p(tablero[j][i].pieza->clase_pieza)-32;
				else // si pieza es blanca
					cadena[k--] = clase_p(tablero[j][i].pieza->clase_pieza);
			}
			else if (tablero[j][i].color_casillero) // si no hay pieza en casillero
				cadena[k--] = '#';
			else
				cadena[k--] = ' ';
		}
	}
	cadena[64]=0;
}
