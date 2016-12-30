#ifndef FORMATO
	#define FORMATO "P2"
#endif
#define HI(num) (((num) & 0x0000FF00)>> 8)
#define LO(num) ((num) & 0x000000FF)
/* Estructura general con todos los datos importantes */
typedef struct _PGMData {
	int ancho;
	int alto;
	int max_gray;
	int **matrix; // Esto son los datos de la imagen y la RC (Region Critica)
	char *fichero;
} PGMData;
/*
#################
# Declaraciones #
#################*/
// TODO: declarar las funciones (por ahora no es necesario pero está feo)

/*
################
# Definiciones #
################*/
/* Reserva memoria para todos los datos de la imagen (ver bibliografia 2) */
int **allocate_dynamic_matrix(int ancho, int alto)
{
	int **ret_val;
	int i;
	ret_val=(int **)malloc(sizeof(int *) * ancho);
	if (ret_val==NULL) {
		perror("memory allocation failure");
		exit(EXIT_FAILURE);
	}
	for (i=0; i<ancho; ++i) {
		ret_val[i]=(int *)malloc(sizeof(int) * alto);
		if (ret_val[i]==NULL) {
			perror("memory allocation failure");
			exit(EXIT_FAILURE);
		}
	}
	return ret_val;
}
/* Borra la memoria con los datos de la imagen (ver bibliografia 2) */
void deallocate_dynamic_matrix(int **matrix, int ancho)
{
	int i;
	for (i=0; i<ancho; ++i) {
		free(matrix[i]);
	}
	free(matrix);
}
/* Escribe la cabecera de la imagen */
void pgm_cabecera(const char *filename, const PGMData *datos)
{
	FILE *archivo;
	archivo=fopen(filename, "wb");
	if (archivo==NULL) {
		perror("No se puede abrir el fichero");
		exit(EXIT_FAILURE);
	}
	fprintf(archivo, "P5 ");
	fprintf(archivo, "%d %d ", datos->alto, datos->ancho);
	fprintf(archivo, "%d ", datos->max_gray);
	fclose(archivo);
}
// TODO No se utiliza, solo como ejemplo (ver bibliografia 2)
/* Escritura de archivo */
void pgm(const char *filename, const PGMData *datos)
{
	FILE *archivo;
	int i, j, hi, lo;
	archivo=fopen(filename, "wb");
	if (archivo==NULL) {
		perror("No se puede abrir el fichero");
		exit(EXIT_FAILURE);
	}
	fprintf(archivo, "P5 ");
	fprintf(archivo, "%d %d ", datos->alto, datos->ancho);
	fprintf(archivo, "%d ", datos->max_gray);
	if (datos->max_gray > 255) {
		for (i=0; i<datos->ancho; ++i) {
			for (j=0; j<datos->alto; ++j) {
				hi=HI(datos->matrix[i][j]);
				lo=LO(datos->matrix[i][j]);
				fputc(hi, archivo);
				fputc(lo, archivo);
			}
		}
	}
	else {
		for (i=0; i<datos->ancho; ++i) {
			for (j=0; j<datos->alto; ++j) {
				lo=LO(datos->matrix[i][j]);
				fputc(lo, archivo);
			}
		}
	}
	fclose(archivo);
	deallocate_dynamic_matrix(datos->matrix, datos->ancho);
}
// TODO No se utiliza, solo como ejemplo (ver bibliografia 2)
/*for reading:*/
PGMData* readPGM(const char *file_name, PGMData *datos)
{
	FILE *pgmFile;
	char version[3];
	int i, j;
	int lo, hi;
	pgmFile=fopen(file_name, "rb");
	if (pgmFile==NULL) {
		perror("cannot open file to read");
		exit(EXIT_FAILURE);
	}
	fgets(version, sizeof(version), pgmFile);
	if (strcmp(version, "P5")) {
		fprintf(stderr, "Wrong file type!\n");
		exit(EXIT_FAILURE);
	}
	//SkipComments(pgmFile);
	fscanf(pgmFile, "%d", &datos->ancho);
	//SkipComments(pgmFile);
	fscanf(pgmFile, "%d", &datos->alto);
	//SkipComments(pgmFile);
	fscanf(pgmFile, "%d", &datos->max_gray);
	fgetc(pgmFile);
	datos->matrix=allocate_dynamic_matrix(datos->alto, datos->ancho);
	if (datos->max_gray > 255) {
		for (i=0; i<datos->alto; ++i) {
			for (j=0; j<datos->ancho; ++j) {
				hi=fgetc(pgmFile);
				lo=fgetc(pgmFile);
				datos->matrix[i][j]=(hi<< 8)+lo;
			}
		}
	}
	else
	{
		for (i=0; i<datos->alto; ++i){
			for (j=0; j<datos->ancho; ++j) {
				lo=fgetc(pgmFile);
				datos->matrix[i][j]=lo;
			}
		}
	}
	fclose(pgmFile);
	return datos;
}
/* Escritura de archivo */
void pgm_escribe_datos(const char *filename, const PGMData *datos)
{
	FILE *archivo;
	int i, j, hi, lo;
	archivo=fopen(filename, "ab");
	if (archivo==NULL) {
		perror("No se puede abrir el fichero");
		exit(EXIT_FAILURE);
	}
	if (datos->max_gray > 255) {
		for (i=0; i<datos->ancho; ++i) {
			for (j=0; j<datos->alto; ++j) {
				hi=HI(datos->matrix[i][j]);
				lo=LO(datos->matrix[i][j]);
				fputc(hi, archivo);
				fputc(lo, archivo);
			}
		}
	}
	else {
		for (i=0; i<datos->ancho; ++i) {
			for (j=0; j<datos->alto; ++j) {
				lo=LO(datos->matrix[i][j]);
				fputc(lo, archivo);
			}
		}
	}
	fclose(archivo);
}
/* Prueba */ // TODO No se utiliza
char * genera_cadena()
{
	// char *frase="Veremos si la transferencia es buena";
	return "#Ejemplo de imagen";
}
/* Define la cabecera de la imagen creada */ // TODO No se utiliza
char * limites_de_imagen(int ancho, int alto) {
	//char final[40]; //=malloc(40);
	// printf("%s\n%d %d\n", FORMATO, ancho, alto);
	// char final[NUM_CHARS+1];
	// sprintf(final, "%s\n%d %d\n", FORMATO, ancho, alto);
	char *final=malloc(sizeof(char)*15);
	sprintf(final, "%s\n%d %d\n", FORMATO, ancho, alto);
	return final;
}
/* Generera cuadrado de color gris (usado en las demás funciones) */
/* cuadro_negro(datos, Ancho, Alto, xInicial, yInicial, color) */
void cuadro_gris(PGMData *datos, int ancho, int alto, int xini, int yini, int gris) {
	// char lo, hi;
	// for (i=0; i<datos->ancho; ++i) {
	// 	for (j=0; j<datos->alto; ++j) {
	// 		hi='F';
	// 		lo='F';
	// 		datos->matrix[i][j]=hi;
	// 		datos->matrix[i][++j]=lo;
	// 		// hi=fgetc(pgmFile);
	// 		// lo=fgetc(pgmFile);
	// 		// datos->matrix[i][j]=(hi<< 8)+lo;
	// 		// printf("%d\n", datos->matrix[i][j]);
	// 	}
	// }
	int i, j;
	// Comprobacion de seguridad TODO sumar el ancho y xini etc
	if(yini>datos->alto || alto>datos->alto || xini>datos->ancho || ancho>datos->ancho) {
		perror("Datos fuera de rango");
		exit(EXIT_FAILURE);
	}
	for (i=yini; i<alto+yini; ++i) {
		for (j=xini; j<ancho+xini; ++j) {
			datos->matrix[i][j]=gris;
		}
	}
}
/* Genera un cuadrado negro */
/* cuadro_negro(datos, Ancho, Alto, xInicial, yInicial) */
void cuadro_negro(PGMData *datos, int ancho, int alto, int xini, int yini) {
	cuadro_gris(datos, ancho, alto, xini, yini, 0);
}
/* Genera un cuadrado blanco */
/* cuadro_blanco(datos, Ancho, Alto, xInicial, yInicial) */
void cuadro_blanco(PGMData *datos, int ancho, int alto, int xini, int yini) {
	cuadro_gris(datos, ancho, alto, xini, yini, datos->max_gray);
}
/* Numero aleatorio entre un rango usado en cuadro_ruido http://stackoverflow.com/a/17554531 */
unsigned int rand_interval(unsigned int min, unsigned int max)
{
	unsigned int r;
	const unsigned int range=1+max-min;
	const unsigned int buckets=RAND_MAX / range;
	const unsigned int limit=buckets * range;
	do {
		r=rand();
	} while (r>=limit);
	return min+(r / buckets);
}
/* Genera un cuadrado de ruido blanco */
/* cuadro_ruido(datos, Ancho, Alto, xInicial, yInicial) */
void cuadro_ruido(PGMData *datos, int ancho, int alto, int xini, int yini) {
	int i, j;
	// Comprobacion de seguridad TODO sumar el ancho y xini etc
	if(yini>datos->alto || alto>datos->alto || xini>datos->ancho || ancho>datos->ancho) {
		perror("Datos fuera de rango");
		exit(EXIT_FAILURE);
	}
	for (i=yini; i<alto+yini; ++i) {
		for (j=xini; j<ancho+xini; ++j) {
			datos->matrix[i][j]=rand_interval(0, datos->max_gray);
		}
	}
}
