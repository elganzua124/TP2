typedef enum {BLANCO, NEGRO} color_t;

typedef enum {TORRE, CABALLO, ALFIL, REINA, REY, PEON} clase_t;

typedef struct {
	color_t color_pieza;
	clase_t clase_pieza;
} pieza_t;

typedef struct {
	pieza_t *pieza;
	color_t color_casillero;
} casillero_t;

typedef struct nodo
{
	char jugada[6];
	char *player;
	char stg[65];
	struct nodo *ant;
	struct nodo *sig;
} nodo_t;

//########### globales ###########

extern char *nombre_negras;

extern char *nombre_blancas;

extern char *nombre_archivo;

extern char cadena[66];

extern casillero_t tablero[8][8];

extern nodo_t *head;

//################################
