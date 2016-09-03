/*
 * MeshReader.c
 *
 *  Created on: May 9, 2016
 *      Author: MartinR
 *
 *  Primera revision
 *  Lunes Mayo 9
 *  Segund revisi�n- 10 de mayo
 *	Tercera revisi�n- 11 de mayo
 *	Cuarta revisi�n- 12 de mayo
 *	Quinta revision - 13 mayo (cabio a apuntadores y pase por referencia)
 *	Sexta revision- 14 de mayo (el read file no funciona)
 */
//#include <List.h>

#include <stdio.h>
#include <stdlib.h>

typedef enum{FALSE, TRUE}Bool;

//Estructura para las cordenadas de los puntos
struct coordenadas {
	float x; //punto para el eje x
	float y; //punto para el eje y
	float z; //punto para el eje z
};

struct tri {
	int p1; //indice de la coordenada del primer punto
	int p2; //indice de la coordenada del segundo punto
	int p3; //indice de la coordenada del tercer punto.
};

typedef struct coordenadas* COORD;
typedef struct tri* Triangle;

struct malla {
	int nVert; //cantidad de vertices que hay en el mesh
	int nFaces; //cantidad de triangulos que hay en el mesh
	Triangle *arrayTri; //cambio a apuntadores directo
	COORD *arrayCord; //cambio a apuntadores directo
};

typedef struct malla* Mesh;

//Funcion para la copia de una malla a una nueva malla
Mesh copy_mesh(Mesh m) {

	Mesh new_m; //Crear la nueva nueva estructura malla a copiar
	new_m = m; //Igualar la nueva malla con la que se desea copiar
	new_m->arrayCord = m->arrayCord; //obtener el mismo arreglo de coordenadas
	new_m->arrayTri = m->arrayTri; //obtener el mismo arreglo de triangulos
	new_m->nFaces = m->nFaces; //obtener el numero de caras
	new_m->nVert = m->nVert; //obtener el numero de vertices

	return new_m;
}

//agrega un nuevo set de coordenadas a la malla
void add_vect(Mesh m, float x, float y, float z){
    int i = m->nVert; //igualar el indice a la cantidad de puntos que hay
    m->arrayCord = (COORD*)realloc(m->arrayCord, sizeof(COORD) + 1); //aumentar el tama�o con realloc
    m->arrayCord[i]->x = x; //nueva coordenada en x
    m->arrayCord[i]->y = y; //nueva coordenada en y
    m->arrayCord[i]->z = z; //nueva coordenada en z

    m->nVert++; //aumentar el tama�o de coordenadas en el arreglo
    return; //No regresa nada
}

//Agrega una nuevo triangulo basado en los puntos que ya existen en la malla
void add_triangle(Mesh m, int point1, int point2, int point3){
    int i = m->nFaces; //Igualar el indice a la cantidad de triangulos que existen en el arreglo
    printf("(:\n");
    m->arrayTri = (Triangle*)realloc(m->arrayTri, sizeof(Triangle) + 1); //Aumentar el tama�o del arreglo para el nuevo triangulo
    printf("(:\n");
    m->arrayTri[i+1]->p1 = point1; //primer punto del nuevo triangulo
    m->arrayTri[i+1]->p2 = point2; //segundo punto del nuevo triangulo
    m->arrayTri[i+1]->p3 = point3; //tercer punto del nuevo triangulo
    m->nFaces++; //Aumentar el numero de caras en la malla
    return; //No regresa nada
}

//Mueve un conjunto de coordenadas cierta valor dado, este valor puede ser tanto negativo como positivo
void move_Points(Mesh m, float move, int punto){

    m->arrayCord[punto]->x += move; //Actualizar la posicion de x
    m->arrayCord[punto]->y += move; //Actualizar la posicion de y
    m->arrayCord[punto]->z += move; //Actualizar la posicion de z

    return; //No regresa nada
}

//Para imprimir COORDENADAS
void imprimir_COORD (Mesh m){
	printf("\nN�mero de puntos\n\t\tx\ty\tz\n");
	int i;
	for ( i = 0; i < m->nVert; i++){
		printf("PUNTO #%d\t%.4f\t%.4f\t%.4f\n", i, m->arrayCord[i]->x, m->arrayCord[i]->y, m->arrayCord[i]->z);
	}
}

//Imprimir Triangulo
void imprimir_TRIANGULO (Mesh m){
	printf("\nTri�ngulos\n\n");
	int i;
	for( i = 0; i < m->nFaces; i++){
		printf("TRI�NGULO #%d\t%d\t%d\t%d\n", i, m->arrayTri[i]->p1, m->arrayTri[i]->p2, m->arrayTri[i]->p3);
	}
}

//Funci�n para leer el archivo
void open_Mesh(char *filename, Mesh m_malla) {
	//Creaci�n de la malla que utilizaremos en el programa
	FILE *archivoMesh = fopen(filename, "rt"); //creaci�n del archivo y lectura
	if (archivoMesh == NULL) {
		printf("El archivo no se puede abrir\n");
		return;
	}
	//Reading header
	printf("El archivo s� se pudo abrir\n");
	fscanf(archivoMesh, "ply\nformat asciis\n");
	fscanf(archivoMesh, "element vertex");
	fscanf(archivoMesh, " %d\n", &m_malla->nVert);
//	fscanf(archivoMesh, "element vertex %d\n", &v); //obtener el n�mero de vertices
//	m_malla->nVert = v;
//	fscanf(archivoMesh, "element vertex 6\n");
	fscanf(archivoMesh, "property float x\n");
	fscanf(archivoMesh, "property float y\n");
	fscanf(archivoMesh, "property float z\n");
//	fscanf(archivoMesh, "element face %d\n", &m_malla->nFaces); //obtener n�mero de tri�ngulos
	fscanf(archivoMesh, "element face");
	fscanf(archivoMesh, " %d\n", &m_malla->nFaces);
	fscanf(archivoMesh, "property list uchar char vertex_indices\n");
	fscanf(archivoMesh, "end_header\n");

	//ciclo para obtener v�rtices
	int i = 0;
	m_malla->arrayCord = malloc(m_malla->nVert * sizeof(COORD)); //creamos el arreglo del tama�o obtenido
	for (i = 0; i < m_malla->nVert; i++) {
		fscanf(archivoMesh, "%f %f %f\n", &m_malla->arrayCord[i]->x,
				&m_malla->arrayCord[i]->y, &m_malla->arrayCord[i]->z);
		//		m_malla.arrayCord++;
	}

	//imprimir a ver s� funciona
/*	for (i = 0; i < m_malla.nVert; i++) {
		printf("%f %f %f\n", m_malla.arrayCord[i].x, m_malla.arrayCord[i].y,
				m_malla.arrayCord[i].z);
	}
*/
	//ciclo para obtener tri�ngulos
	m_malla->arrayTri = malloc(m_malla->nFaces * sizeof(Triangle));
	for (i = 0; i < m_malla->nFaces; i++) {
		//fscanf(archivoMesh, "3 %d %d %d\n", &m_malla->arrayTri[i]->p1,
		//		&m_malla->arrayTri[i]->p2, &m_malla->arrayTri[i]->p3);
		//	(m_malla.arrayTri)++;
		fscanf(archivoMesh, "3");
		fscanf(archivoMesh, " %d %d %d\n", &m_malla->arrayTri[i]->p1, &m_malla->arrayTri[i]->p2, &m_malla->arrayTri[i]->p3);
	}
	//imprimir a ver s� funciona
	/*for (i = 0; i < m_malla.nFaces; i++) {
		printf("%d %d %d\n", m_malla.arrayTri[i].p1, m_malla.arrayTri[i].p2,
				m_malla.arrayTri[i].p3);
	}*/

	//printf("N�mero de vertices = %d\n", m_malla.nVert);
	//printf("N�mero de tri�ngulos = %d\n", m_malla.nFaces);
	fclose(archivoMesh);
	return;
}

void escalar (Mesh m, int escala){
	int i;
	for (i = 0; i < m->nVert; i++){
		m->arrayCord[i]->x *= escala;
		m->arrayCord[i]->y *= escala;
		m->arrayCord[i]->z *= escala;
	}
}

void tri_vecinos (Mesh m){
	int i, x;
	printf("\n");
	for (i=0; i<m->nFaces; i++){
		printf("El tr�angulo #%d esta relacionado con:\t", i);
		for (x = 0; x<m->nFaces; x++){
			if (x != i){
			if ((m->arrayTri[i]->p1 == m->arrayTri[x]->p2)||(m->arrayTri[i]->p1 == m->arrayTri[x]->p1) || (m->arrayTri[i]->p1 == m->arrayTri[x]->p3)){
				printf("%d\t",x);
			} else if ((m->arrayTri[i]->p2 == m->arrayTri[x]->p2)||(m->arrayTri[i]->p2 == m->arrayTri[x]->p1) || (m->arrayTri[i]->p2 == m->arrayTri[x]->p3)){
				printf("%d\t",x);
			} else if ((m->arrayTri[i]->p3 == m->arrayTri[x]->p2)||(m->arrayTri[i]->p3 == m->arrayTri[x]->p1) || (m->arrayTri[i]->p3 == m->arrayTri[x]->p3)){
				printf("%d\t",x);
			}
			}
		}
		printf("\n");
	}
}

void find_tri (Mesh m, int point1, int point2, int point3){
	int i;
	for ( i = 0; i < m->nFaces; i++){
		if ((m->arrayTri[i]->p1 == point1) && (m->arrayTri[i]->p2 == point2) && (m->arrayTri[i]->p3 = point3)){
			printf("\nLos puntos indicados pertenecen al tri�ngulo #%d\n", i);
			return;
		}
	}
	printf("\nLos puntos indicados no pertenecen a ning�n tri�ngulo\n");
}

void vertice_tri (Mesh m, int v){
	int i;
	printf("El v�rtice #%d con coordenadas x = %.4f y = %.4f z = %.4f\n", v, m->arrayCord[v]->x, m->arrayCord[v]->y, m->arrayCord[v]->z);
	printf("Es intersectado con los siguientes tri�ngulos:\n");
	for ( i = 0; i < m->nFaces; i++){
		if ((m->arrayTri[i]->p1 == v)||(m->arrayTri[i]->p2 == v)||(m->arrayTri[i]->p3 == v)){
			printf("- %d\n", i);
		}
	}
}

void borrar_ver (Mesh m, int p){

}

int main(){
	setbuf(stdout, NULL);
	Mesh m_malla = NULL;
	char *lol = {"pyramid.txt"};
	open_Mesh(lol, m_malla);
	imprimir_COORD (m_malla);
	imprimir_TRIANGULO (m_malla);
	escalar(m_malla, 2);
	imprimir_COORD (m_malla);
	tri_vecinos (m_malla);
	find_tri(m_malla, 5, 3, 2);
	vertice_tri(m_malla, 3);
	add_vect(m_malla, 1.00, 3.50, 2.00);
	imprimir_COORD (m_malla);
	add_triangle(m_malla, 6, 1, 3);
	imprimir_TRIANGULO(m_malla);
	//Mesh new;
	//new = copy_mesh(m_malla);
	return 0;
}
