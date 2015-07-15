#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "tablero.h"

nodo_t *head; // puntero al primer nodo

void tablero_blanco(char cadena[66]);
void tablero_negro(char cadena[66]);
int chekar_coordenada(char coord[7]);
int mover_pieza(casillero_t tablero[8][8], color_t color_jugador, const char coord[7]);
void jugada_a_string(char cadena[66]);
 
nodo_t * crear_nodo(char *cadena, char movimiento[6], char *nombre_jugador)
{
	nodo_t *nodoPtr = (nodo_t *)malloc(sizeof(nodo_t));
	strcpy(nodoPtr->jugada, movimiento);
	nodoPtr->player = malloc(20);
	strcpy(nodoPtr->player, nombre_jugador);
	strcpy(nodoPtr->stg, cadena);
	nodoPtr->ant = NULL;
	nodoPtr->sig = NULL;
	return nodoPtr;
}

void insertar_nodo(char *cadena, char movimiento[6], char *nombre_jugador) {
	nodo_t *temp = head;
	nodo_t *newNode = crear_nodo(cadena,movimiento,nombre_jugador);
	if(head == NULL)
	{
		head = newNode;
		return;
	}
	while(temp->sig != NULL)
		temp = temp->sig;
	temp->sig = newNode;
	newNode->ant = temp;
}

int crear_archivo()
{
	nodo_t *tmp = head;
	FILE *archivo;
	if((archivo = fopen(nombre_archivo,"w")))
	{
		fprintf(archivo, "#CHS w:%s b:%s\n",nombre_blancas,nombre_negras);
		while(tmp)
		{
			fprintf(archivo, "%s", tmp->jugada);
			tmp = tmp->sig;
		}
		fclose(archivo);
	}
	else
		fputs("ERROR no se pudo crear el archivo\n",stderr);
	return 0;
}

int leer_opcion()
{
	struct termios old_tio, new_tio;
	int opc;
	/* get the terminal settings for stdin */
	tcgetattr(STDIN_FILENO,&old_tio);
	/* we want to keep the old setting to restore them a the end */
	new_tio=old_tio;
	/* disable canonical mode (buffered i/o) and local echo */
	new_tio.c_lflag &=(~ICANON & ~ECHO);
	/* set the new settings immediately */
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
	opc = getchar();
	/* restore the former settings */
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
	return opc;
}

void imprimir_lista()
{
	char cadena_inicial[65] = "RNBQKBNRPPPPPPPP # # # ## # # #  # # # ## # # # pppppppprnbqkbnr";

	nodo_t *tmp = head, *inic = crear_nodo(cadena_inicial,"-\n","-");

	tmp->ant = inic;
	inic->sig = tmp;
	tmp = tmp->ant;

	printf("\n\n\n\nWHITE PLAYER: %s        BLACK PLAYER: %s\n",nombre_blancas, nombre_negras);
	tablero_blanco(cadena_inicial);
	printf("Jugador: %s. Movimiento: %s\n", tmp->player, tmp->jugada);
	puts("<n> proxima jugada <p> jugada anterior <q> salir:");
	int opc = leer_opcion();

	color_t color_jugador = BLANCO; // blanco = 0, negro = 1

	while (opc != 'q')
	{
		if(opc == 'p')
		{
			if (tmp->ant)
			{
				tmp = tmp->ant;
				printf("\n\n\n\nWHITE PLAYER: %s        BLACK PLAYER: %s\n",nombre_blancas, nombre_negras);
				if(tmp->ant == NULL) // su anterior es el tablero inicial
					tablero_blanco(tmp->stg);
				else
					color_jugador == 0 ? tablero_blanco(tmp->stg) : tablero_negro(tmp->stg); 
				printf("Jugador: %s. Movimiento: %s\n", tmp->player, tmp->jugada);
				puts("<n> proxima jugada <p> jugada anterior <q> salir:");
				color_jugador = !color_jugador;
			}
			else
				puts("TABLERO INICIAL. <n> proxima jugada <q> salir:");
		}
		else if(opc == 'n')
		{
			if(tmp->sig)
			{
				tmp = tmp->sig;
				printf("\n\n\n\nWHITE PLAYER: %s        BLACK PLAYER: %s\n",nombre_blancas, nombre_negras);
				color_jugador == 0 ? tablero_blanco(tmp->stg) : tablero_negro(tmp->stg);
				printf("Jugador: %s. Movimiento: %s\n", tmp->player, tmp->jugada);
				puts("<n> proxima jugada <p> jugada anterior <q> salir:");
				color_jugador = !color_jugador;
			}
			else
				puts("ULTIMA JUGADA. <p> jugada anterior <q> salir:");
		}
		else
		{
			puts("Opcion no valida. Intente nuevamente:");
			puts("Hint: fijate si esta en mayus (me paso).");
		}
		opc = leer_opcion();
	}
}

int comprobar_archivo(char *nombre_archivo)
{
	int cant;
	FILE *arch;
	color_t color_jugador;
	
	char *linea = NULL, coord[7];
	linea = malloc(50);

	if( (arch = fopen(nombre_archivo,"r")) )
	{
		fgets(linea,51,arch); // (19c x 2) + 10c + '\n' + '\0' = 50c

		nombre_blancas = malloc(65);
		nombre_negras = malloc(65);

		cant = sscanf(linea, "#CHS w:%s b:%s%*[\n]",nombre_blancas,nombre_negras);
		if (cant == 2)
		{
			color_jugador = BLANCO;
			while (fgets(coord, 7, arch) != NULL)
			{
				if( chekar_coordenada(coord) ||  mover_pieza(tablero,color_jugador,coord) )
				{
					fputs("El archivo contiene movidas invalidas\n", stderr);
					free(nombre_blancas);
					free(nombre_negras);
					return 1;
				}
				jugada_a_string(cadena);

				if(color_jugador == 0)
					insertar_nodo(cadena,coord,nombre_blancas);
				else
					insertar_nodo(cadena,coord,nombre_negras);
				color_jugador = !color_jugador;
			}
			return 0;
		}
		else
			puts("Ha ocurrido un ERROR!");
		fclose(arch);
		free(nombre_blancas);
		free(nombre_negras);
		return 1;
	}
	else
		fputs("ERROR no se pudo abrir el archivo\n",stderr);
	free(linea);
	return 1;
}
