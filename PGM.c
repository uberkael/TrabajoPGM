/*Librerias usadas en el programa: */
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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>

/*Macros que se utilizan:  */
#define TAM 100
#define TAM_EXP 30

//estructura de un cuadrado que luego es escrito en la imagen.
typedef struct _Params {
  PGMData * Datos;
  int Ancho,Alto,xInicial,yInicial,Color,opcion;
}Params, *Pparams;

/*Funcion que se encarga de llamar a las funciones correspondientes a los
  que se desean imprimir.
  Esta funcion es la que ejecutan los hilos*/
void *imprimir(void *pVoid)
{
  Pparams params = (Pparams)pVoid;
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
  /*Estructura usada para contar los tiempos del programa */
  struct tms InfoInicio, InfoFin;
  clock_t t_inicio, t_fin;
  long tickporseg;
  tickporseg= sysconf(_SC_CLK_TCK);
  t_inicio= times(&InfoInicio);

  //Clave de la memoria compartida.
  key_t clave;
  int shmid;
  //El segmento de la memoria compartida.
  Params *seg;
  clave=ftok(".",'S'); 
  pthread_attr_t attr;
  pthread_t thid;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  Params Params;
  PGMData *datos=malloc (sizeof (struct _PGMData));
  
  // PGMData datos;
  /* Asignando memoria y datos */
  system("clear");
  printf("Introduzca tamaño deseado de la imagen: ");
  scanf("%d", &datos->ancho);
  datos->alto=datos->ancho;
  datos->max_gray=255;
  datos->fichero="imagen.pgm";
  //Reserva de memoria de toda la estructura.
  datos->matrix=allocate_dynamic_matrix(datos->ancho, datos->alto);
  /* Escribe la cabecera del fichero */
  pgm_cabecera(datos->fichero, datos);
  Params.Datos=datos;
  int opcion;//Opcion escogida en el menu.
  int salir=0;//Variable de control para no volver a imprimir el menu.
  int fallo=0;//Variable de control para imprimir que se ha escrito una opcion una valida.
  int cont=0;//Variable de control para enumerar el numero de cuadrados pintados.
  int Ancho, Alto, xInicial, yInicial, Color;
  if((shmid=shmget(clave,TAM,IPC_CREAT|IPC_EXCL|0660))==-1)
    {
      printf("El segmento de memoria compartida ya existe\n");
      printf("Abriendo como cliente\n");
      if((shmid=shmget(clave,TAM,0))==-1)//Comprobacion de que se puede abrir el segmento.
	printf("Error al abrir el segmento\n");
    }
  else
    printf("Nuevo segmento creado\n");
  
  if (shmid != -1) {//Comprobar al mapear la memoria.
    if((seg=shmat(shmid,NULL,0))== (Pparams)-1)
      printf("Error al mapear el segmento\n");
  }
  while(salir==0){
    // system("clear");
    if(fallo==1){
      system("clear");
      printf ("\033[31m OPCION MAL INGRESADA \n");//Imprimir en rojo que se ha seleccionado una opcion no valida.
      fallo=0;//Actualizar la variable de control.
    }
    printf ("\033[34m \n"); //Eleccion de color AZUL para las opciones.
    /*Imprimir menu: */
    printf("1: Insertar cuadrado blanco \n");
    printf("2: Insertar cuadrado negro \n");
    printf("3: Insertar cuadrado gris \n");
    printf("4: Insertar cuadrado ruido \n");
    printf("5: Cuadrados Insertados \n");
    printf("6: Generar imagen y salir \n");
    printf("\n\nOpción: ");
    scanf("%d",&opcion);//Se guarda la opcion deseada.
    switch(opcion){
    case 1 ://Opcion de pintar cuadro blanco.
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
      seg[cont]=Params;
      printf("Hecho...\n");
      cont++;
      pthread_create(&thid, &attr, imprimir, &Params);
      break;
    case 2://Opcion de pintar cuadro negro.
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
      seg[cont]=Params;
      printf("Hecho...\n");
      cont++;
      pthread_create(&thid, &attr, imprimir, &Params);
      break;
    case 3://Opcion de pintar cuadro gris
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
      seg[cont]=Params;
      printf("Hecho...\n");
      cont++;
      pthread_create(&thid, &attr, imprimir, &Params);
      break;
    case 4://Opcion de pintar cuadro ruido.
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
      seg[cont]=Params;
      printf("Hecho...\n");
      cont++;
      pthread_create(&thid, &attr, imprimir, &Params);
      break;
    case 5://Opcion imprimir los datos de los cuadros seleccionados.
      system("clear");
      printf("Esta imagen tiene %d cuadrados creados\n", cont);
      for (int i=0; i<cont;i++){
	printf("Cuadrado  Ancho   Alto   xInicial   yInicial   Color \n" );
	int num_cuadrado=i;
	num_cuadrado++;
	printf("%d)        %d     %d       %d        %d        %d\n", num_cuadrado,seg[i].Ancho,seg[i].Alto,seg[i].xInicial,seg[i].yInicial,seg[i].Color);
      }
      break;
    case 6://Opcion generar imagen.
      shmctl(shmid,IPC_RMID,0);
      salir=1;
    default://Opcion por defecto.
      fallo=1;
    }
  }
  
  /* Escribe los datos en el fichero (arriba la cabecera) */
  pgm_escribe_datos(datos->fichero, datos);
  /*Variables usadas para seleccion de exportacion a diferentes formatos: */
  char opcion_exportar[TAM_EXP];//Entrada STDIN.
  char opcion_leidaJPG;//JPG.
  char opcion_leidaPDF;//PDF.
  do{
    system("clear");
    printf("¿Desea exportar la imagen a formato JPG? Y/N: ");
    scanf("%s",opcion_exportar);
    opcion_leidaJPG=opcion_exportar[0];
    if (opcion_leidaJPG=='Y')
      salir=1;
    else if (opcion_leidaJPG=='y')
      salir=1;
    else if (opcion_leidaJPG=='N')
      salir=1;
    else if (opcion_leidaJPG=='n')
      salir=1;
    else
      salir=0;
  }
  while(salir==0);
  do{
    system("clear");
    printf("¿Desea exportar la imagen a formato PDF? Y/N: ");
    scanf("%s",opcion_exportar);
    opcion_leidaPDF=opcion_exportar[0];
    if (opcion_leidaPDF=='Y')
      salir=1;
    else if (opcion_leidaPDF=='y')
      salir=1;
    else if (opcion_leidaPDF=='N')
      salir=1;
    else if (opcion_leidaPDF=='n')
      salir=1;
    else
      salir=0;
  }
  /*Creacion de los hijos que ejecutan la conversion de .pgm a .pdf y .jpg*/
  while(salir==0);
  if(opcion_leidaJPG=='Y' || opcion_leidaJPG=='y'){
    if (fork()==0)
      {
	execlp("convert", "convert", "imagen.pgm", "imagen.jpg", NULL);
	perror("ERROR AL EXPORTAR A JPG");
      }
  }
  if(opcion_leidaPDF=='Y' || opcion_leidaPDF=='y'){
    if (fork()==0)
      {
	execlp("convert", "convert", "imagen.pgm", "imagen.pdf", NULL);
	perror("ERROR AL EXPORTAR A PDF");	
      }
  }
  /* Liberacion de la memoria */
  deallocate_dynamic_matrix(datos->matrix, datos->ancho);
  //El padre espera a que los hijos finalicen:
  wait(NULL);
  //Destruccion de los atributos de los hilos
  pthread_attr_destroy(&attr);
  //Libera el nombre del segmento.
  shmdt(seg);

  /*Finalizacion de conteo del programa: */
  t_fin= times(&InfoFin);
  printf ("Tiempo real: %7.2f\n",
	  (float)(t_fin - t_inicio)/tickporseg);
  printf ("Tiempo de usuario: %7.2f\n",
	  (float)(InfoFin.tms_cutime - InfoInicio.tms_cutime)/tickporseg);
  printf ("Tiempo de sistema: %7.2f\n",
	  (float)(InfoFin.tms_cstime - InfoInicio.tms_cstime)/tickporseg);
  
  return 0;
}
