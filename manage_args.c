#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablero.h"

char *nombre_blancas;
char *nombre_negras;
char *nombre_archivo;

void inic_tablero(casillero_t tablero[8][8]);
int comprobar_archivo(char *nombre_archivo);
void imprimir_lista();
void play_game(casillero_t tablero[8][8], char cadena[66], short cod);
int crear_archivo();


short chequeo_nombre_archivo(char *nombre)
{
	short l = strlen(nombre); // .chs tiene long=4, el min sera de 5
	if(l > 4)
	{
		if( !strcmp(&nombre[l-4],".chs") )
			return 0; // ok con archivo de salida
	}
	return 1;
}

/*	si devuelve 1: error
	si devuelve 2: ./ajedrez --play archivo.chs
	si devuelve 3: ./ajedrez jugadores
	si devuelve 4: ./ajedrez jugadores --output archivo.chs
	si devuelve 5:  se escribio --help
*/
short chequeo_de_args(int argc, char *argv[])
{
	short i, num = 0;

	if(argc == 7) // con archivo de salida
	{
		for(i=1; i < 6; i+=2) // chequea argv[1], argv[3] y argv[5]
		{
			if( !strcmp(argv[i],"--white") )
				num++;
			else if( !strcmp(argv[i],"--black") )
				num += 2;
			else if( !strcmp(argv[i],"--output") )
				num += 5;
		}
		if( num == 8 ) // 1+2+5 = 8.
		{
			i = 1;
			while( strcmp(argv[i],"--output") ) // busco la pos de --output
				i+=2;
			if( !chequeo_nombre_archivo(argv[i+1]) )
				return 4;
		}
	} else if(argc == 5) // sin archivo de salida
	{
		for(i=1; i < 4; i+=2) // chequea argv[1] y argv[3]
		{
			if( !strcmp(argv[i],"--white") )
				num++;
			else if( !strcmp(argv[i],"--black") )
				num += 3;
		}
		if( num == 4 ) // 1+3 = 4.
			return 3;
	} else if(argc == 3) // reproducir jugada
	{
		if( !strcmp(argv[1],"--play") && !chequeo_nombre_archivo(argv[2]) )
			return 2;
	} else if( argc == 2 && !strcmp(argv[1],"--help") ) // tipio --help
		return 5;
	return 1;
}

void cargar_nombres(char *argv[])
{
	nombre_blancas = malloc(20);
	nombre_negras = malloc(20);

	short i = 1;
	while( strcmp(argv[i],"--white") ) // busco la pos de --white
		i+=2;
	strcpy(nombre_blancas, argv[i+1]);

	i = 1;
	while( strcmp(argv[i],"--black") ) // busco la pos de --black
		i+=2;
	strcpy(nombre_negras, argv[i+1]);
}

void cargar_nombre_archivo(char *argv[], short cod)
{
	nombre_archivo = malloc(20);
	short i = 1;

	if(cod == 2)
	{
		while( strcmp(argv[i],"--play") )
			i+=2;
	} else if(cod == 4)
	{
		while( strcmp(argv[i],"--output") )
			i+=2;
	}
	strcpy(nombre_archivo, argv[i+1]);
}

void manejo_de_args(int argc, char *argv[])
{
	short cod;
	cod = chequeo_de_args(argc,argv);
	if(cod == 1)
		puts("err: en params. Ejecute \"./ajedrez --help\".");
	else if(cod == 2) // reproducir archivo
	{
		inic_tablero(tablero);
		cargar_nombre_archivo(argv,cod);
		nombre_blancas = malloc(20);
		nombre_negras = malloc(20);
		if( !comprobar_archivo(nombre_archivo) )
			imprimir_lista();
		free(nombre_blancas);
		free(nombre_negras);
		free(nombre_archivo);
	}
	else if(cod == 3) // jugar sin archivo de salida
	{
		inic_tablero(tablero);
		cargar_nombres(argv);
		play_game(tablero,cadena,cod); // en play.c
		free(nombre_blancas);
		free(nombre_negras);
	}
	else if(cod == 4) // jugar con archivo de salida
	{
		inic_tablero(tablero);
		cargar_nombre_archivo(argv,cod);
		cargar_nombres(argv);
		play_game(tablero,cadena,cod);// check xq no funka con head y act como args
		crear_archivo();
		free(nombre_blancas);
		free(nombre_negras);
		free(nombre_archivo);
	}
	else if(cod == 5) // imprime help
	{
		puts("Opciones disponibles:");
		puts("--------------------");
		puts("./ajedrez --white nombre --black nombre");
		puts("./ajedrez --white nombre --black nombre --output archivo.chs");
		puts("./ajedrez --play archivo.chs");
		puts("./ajedrez --help");
	}
}
