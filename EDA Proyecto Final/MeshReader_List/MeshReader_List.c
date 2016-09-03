/*
 * MeshReader_List.c
 *
 *  Created on: 14/05/2016
 *      Author: Mariana
 *
 *      Modificaci�n 14 de marzo
 */

#include <stdio.h>
#include <stdlib.h>
#include "List.h"

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
	List Lista_Triangulos; //lista de triangulo
	List Lista_Coordenadas;//lista de coordenadas
};

typedef struct malla* Mesh;

Mesh crear_Mesh (){ //crear el Mesh y crear la lista
	Mesh m = (Mesh) malloc (sizeof(struct malla));
	list_create(m->Lista_Coordenadas);
	list_create(m->Lista_Triangulos);
	m->nVert = 0;
	m->nFaces = 0;
	return m;
};

void open_Mesh(char * filename, Mesh m_malla){
	FILE * archivoMesh = fopen(filename, "rt");
	if (archivoMesh == NULL){
		printf("El archivo no se pudo abrir\n");
		return;
	} else{
		printf("El archivo s� se puede abrir\n");
		fscanf(archivoMesh, "ply\n");
		fscanf(archivoMesh, "format ascii\n");
		fscanf(archivoMesh, "element vertex");
		fscanf(archivoMesh, " %d\n", &m_malla->nVert);
		printf("Le� el n�mero de v�rtices\n");
		fscanf(archivoMesh, "property float x\n");
		fscanf(archivoMesh, "property float y\n");
		fscanf(archivoMesh, "property float z\n");
		fscanf(archivoMesh, "element face");
		fscanf(archivoMesh, " %d\n", &m_malla->nFaces);
		printf("Le� el n�mero de caras\n");
		fscanf(archivoMesh, "property list uchar char vertex_indices\n");
		fscanf(archivoMesh, "end_header\n");

		int i = 0;
		COORD Coordenadas = (COORD) malloc(sizeof(struct coordenadas));
		printf("Crea el nodo coordenadas\n");
		for (i = 0; i < m_malla->nVert; i++){
			fscanf(archivoMesh, "%f %f %f\n", &Coordenadas->x, &Coordenadas->y, &Coordenadas->z);
			printf("Le� el #%d rengl�n\n", i);
			list_add(m_malla->Lista_Coordenadas, Coordenadas);
		}

		Triangle Triangulo = (Triangle) malloc(sizeof(struct tri));
		for (i = 0; i < m_malla->nFaces; i++){
			fscanf(archivoMesh, "3 %d %d %d\n", &Triangulo->p1, &Triangulo->p2, &Triangulo->p3);
			list_add(m_malla->Lista_Triangulos, Triangulo);
		}
		printf("El archivo se ley� correctamente\n");
		fclose(archivoMesh);
	}
	return;
}

int main (){
	setbuf(stdout, NULL);
	Mesh m_malla = crear_Mesh();
	open_Mesh("prueba.txt", m_malla);

	return 0;
}
