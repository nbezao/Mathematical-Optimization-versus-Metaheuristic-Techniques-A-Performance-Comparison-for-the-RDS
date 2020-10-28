#ifndef flow_memory_H
#define flow_memory_H

using namespace std;
// definition of functions to reserve memory of vectors and matrices
int *create_vector( int n );
float *create_vector_f( int n );
int **create_matrix( int n, int m );
float **create_matrix_f( int n, int m );
// definition of functions to print vectors and matrices
int print_vector( int *vector, int n );
int print_vector( float *vector, int n );
int print_matrix( int **matrix, int n, int m );
int print_matrix( float **matrix, int n, int m );
// definition of functions to free memory of vectors and matrices
int release_matrix( int **matrix, int n );
int release_matrix( float **matrix, int n );

#endif
