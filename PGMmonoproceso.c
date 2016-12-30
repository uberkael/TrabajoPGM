#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.c"
// int main(int argc, char *argv[]) {
int main() {
	PGMData *datos=malloc (sizeof (struct _PGMData));
	// int dato[ancho][alto]={
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
	// 	{0, 0, 1, 1, 0, 0, 1, 1, 0, 0}
	// };
	// PGMData datos;
	/* Asignando memoria y datos */
	datos->ancho=1000;
	datos->alto=1000;
	datos->max_gray=255;
	datos->fichero="hola.pgm";
	datos->matrix=allocate_dynamic_matrix(datos->ancho, datos->alto);
	// printf("%s\n", genera_cadena());
	// printf("%s\n", limites_de_imagen(ancho, alto));
	// printf("%s\n", genera_negro(10, 5));
	// printf("%s\n", genera_blanco(10, 5));
	/* Escribe la cabecera del fichero */
	pgm_cabecera(datos->fichero, datos);
	/* Ruido blanco toda la imagen */
	//cuadro_ruido(datos, 1000, 1000, 0, 0); // Linea de ruido
	/* Creando objetos (Ancho, Alto, xInicial, yInicial, Color)*/
	cuadro_negro(datos, 1000, 1000, 0, 0); // Todo de negro
	cuadro_blanco(datos, 500, 1000, 0, 0); // Mitad de blanco
	cuadro_gris(datos, 500, 1000, 20, 0, 50); // Mitad gris un poco desplazada
	cuadro_blanco(datos, 1000, 20, 0, 90); // Linea arriba
	cuadro_blanco(datos, 1000, 20, 0, 20); // Identica un poco mas arriba
	cuadro_blanco(datos, 250, 250, 200, 200); // Cuadro esquina izquierda
	cuadro_gris(datos, 250, 100, 600, 500, 100); // Rectangulo derecha
	cuadro_gris(datos, 200, 50, 625, 525, 50); // dentro del anterior
	cuadro_gris(datos, 150, 20, 650, 540, 20); // dentro del anterior
	cuadro_ruido(datos, 200, 200, 800, 800); // Ruido abajo derecha
	cuadro_ruido(datos, 800, 10, 100, 700); // Linea de ruido
	cuadro_negro(datos, 2, 1000, 5, 0); // Linea vertical izquierda
	/* Escribe los datos en el fichero (arriba la cabecera) */
	pgm_escribe_datos(datos->fichero, datos);
	/* TODO: en linux es facil transformar una imagen en otro formato
	convert hola.pgn hola.jpg
	if (fork()==0)
	{ execvp("convert", "convert hola.pgn hola.jpg"); perror("error ejecutando el programa"); }
	*/
	/* Libera la memoria */
	deallocate_dynamic_matrix(datos->matrix, datos->ancho);
	return 0;
}

