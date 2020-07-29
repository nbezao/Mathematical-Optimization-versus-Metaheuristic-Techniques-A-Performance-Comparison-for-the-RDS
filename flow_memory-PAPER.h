#ifndef FLOW_MEMORY_H
#define FLOW_MEMORY_H

using namespace std;
// definition of functions to reserve memory of vectors and matrices
int *crear_vector( int n );
float *crear_vector_f( int n );
int **crear_matrice( int n, int m );
float **crear_matrice_f( int n, int m );
// definition of functions to print vectors and matrices
int print_vector( int *vector, int n );
int print_vector( float *vector, int n );
int print_matrice( int **matrice, int n, int m );
int print_matrice( float **matrice, int n, int m );
// definition of functions to free memory of vectors and matrices
int release_matrice( int **matrice, int n );
int release_matrice( float **matrice, int n );

#endif
