#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.c"
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <pthread.h>

typedef struct _Params {
 PGMData * Datos;
 int Ancho,Alto,xInicial,yInicial,Color,opcion;
}Params, *Pparams;

void *imprimir(void *pVoid)
{
	Pparams params = (Pparams)pVoid;
printf("Thread %lu  \n", pthread_self());
//struct readThreadParams *readParams = n;
//printf("ANCHO DENTRO %d\n",params->Ancho);
switch(params->opcion){
case 1:
cuadro_blanco(params->Datos, params->Ancho, params->Alto, params->xInicial, params->yInicial);
break;
case 2:
cuadro_negro(params->Datos, params->Ancho, params->Alto, params->xInicial, params->yInicial);
break;
case 3:
cuadro_gris(params->Datos, params->Ancho, params->Alto, params->xInicial, params->yInicial, params->Color);
break;
case 4:
cuadro_ruido(params->Datos, params->Ancho, params->Alto, params->xInicial, params->yInicial);
break;
}
pthread_exit(NULL);
}

int main() {

	struct tms InfoInicio, InfoFin;
	clock_t t_inicio, t_fin;
	long tickporseg;
	tickporseg= sysconf(_SC_CLK_TCK);
	t_inicio= times(&InfoInicio);
	pthread_attr_t attr;
	pthread_t thid;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	Params Params;
	PGMData *datos=malloc (sizeof (struct _PGMData));

	// PGMData datos;
	/* Asignando memoria y datos */
	datos->ancho=1000;
	datos->alto=1000;
	datos->max_gray=255;
	datos->fichero="hola.pgm";
	datos->matrix=allocate_dynamic_matrix(datos->ancho, datos->alto);

	/* Escribe la cabecera del fichero */
	pgm_cabecera(datos->fichero, datos);
	Params.Datos=datos;
	int opcion;
	int salir=0;
  int fallo=0;
  int Ancho, Alto, xInicial, yInicial, Color;
  while(salir==0){
    system("clear");
    if(fallo==1){
      printf ("\033[31m OPCION MAL INGRESADA \n");
      fallo=0;
    }
    printf ("\033[34m \n"); //Eleccion de color AZUL para las opciones.
		printf("1: Insertar cuadrado blanco \n");
		printf("2: Insertar cuadrado negro \n");
		printf("3: Insertar cuadrado gris \n");
		printf("4: Insertar cuadrado ruido \n");
		printf("5: Salir \n");
		scanf("%d",&opcion);
		switch(opcion){
		case 1 :
    system("clear");
		printf("Introduzca Ancho: ");
		scanf("%d",&Ancho);
    system("clear");
    printf("Introduzca Alto: ");
		scanf("%d",&Alto);
    system("clear");
    printf("Introduzca coordenada X inicial: ");
		scanf("%d",&xInicial);
    system("clear");
    printf("Introduzca coordenada Y inicial: ");
		scanf("%d",&yInicial);
		Params.Ancho = Ancho;
		Params.Alto = Alto;
		Params.xInicial = xInicial;
		Params.yInicial = yInicial;
		Params.opcion=opcion;
		pthread_create(&thid, &attr, imprimir, &Params);

		break;
		case 2:
    system("clear");
    printf("Introduzca Ancho: ");
		scanf("%d",&Ancho);
    system("clear");
    printf("Introduzca Alto: ");
		scanf("%d",&Alto);
    system("clear");
    printf("Introduzca coordenada X inicial: ");
		scanf("%d",&xInicial);
    system("clear");
    printf("Introduzca coordenada Y inicial: ");
		scanf("%d",&yInicial);
		Params.Ancho = Ancho;
		Params.Alto = Alto;
		Params.xInicial = xInicial;
		Params.yInicial = yInicial;
		Params.opcion=opcion;
		pthread_create(&thid, &attr, imprimir, &Params);
		break;
		case 3:
    system("clear");
    printf("Introduzca Ancho: ");
		scanf("%d",&Ancho);
    system("clear");
    printf("Introduzca Alto: ");
		scanf("%d",&Alto);
    system("clear");
    printf("Introduzca coordenada X inicial: ");
		scanf("%d",&xInicial);
    system("clear");
    printf("Introduzca coordenada Y inicial: ");
		scanf("%d",&yInicial);
    system("clear");
    printf("Introduzca escala de gris: ");
    scanf("%d",&Color);
		Params.Ancho = Ancho;
		Params.Alto = Alto;
		Params.xInicial = xInicial;
		Params.yInicial = yInicial;
		Params.opcion=opcion;
    Params.Color=Color;
		pthread_create(&thid, &attr, imprimir, &Params);
		break;
		case 4:
    system("clear");
    printf("Introduzca Ancho: ");
    scanf("%d",&Ancho);
    system("clear");
    printf("Introduzca Alto: ");
    scanf("%d",&Alto);
    system("clear");
    printf("Introduzca coordenada X inicial: ");
    scanf("%d",&xInicial);
    system("clear");
    printf("Introduzca coordenada Y inicial: ");
    scanf("%d",&yInicial);
		Params.Ancho = Ancho;
		Params.Alto = Alto;
		Params.xInicial = xInicial;
		Params.yInicial = yInicial;
		Params.opcion=opcion;
		pthread_create(&thid, &attr, imprimir, &Params);
		break;
		case 5:
		salir=1;break;
		default:
      fallo=1;


    }
      }

	/* Escribe los datos en el fichero (arriba la cabecera) */
	pgm_escribe_datos(datos->fichero, datos);
	/* TODO: en linux es facil transformar una imagen en otro formato
	convert hola.pgn hola.jpg*/
	if (fork()==0)
	{
		execlp("convert", "convert", "hola.pgm", "hola.jpg", NULL);
		//execlp("convert", "convert", "hola.pgm", "hola.pdf", NULL);
		perror("error ejecutando el programa");
	}
	if (fork()==0)
		{

			execlp("convert", "convert", "hola.pgm", "hola.pdf", NULL);
			perror("error ejecutando el programa");
		}

	/* Libera la memoria */
	deallocate_dynamic_matrix(datos->matrix, datos->ancho);
	wait(NULL);
	pthread_attr_destroy(&attr);
	free(datos);
	t_fin= times(&InfoFin);
	printf ("Tiempo real: %7.2f\n",
	(float)(t_fin - t_inicio)/tickporseg);
	printf ("Tiempo de usuario: %7.2f\n",
	(float)(InfoFin.tms_cutime - InfoInicio.tms_cutime)/tickporseg);
	printf ("Tiempo de sistema: %7.2f\n",
	(float)(InfoFin.tms_cstime - InfoInicio.tms_cstime)/tickporseg);

	return 0;
}
