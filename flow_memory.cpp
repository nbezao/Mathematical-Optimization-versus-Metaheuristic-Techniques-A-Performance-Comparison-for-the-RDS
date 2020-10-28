#include <math.h>
#include <iostream>
using namespace std;

// reserves memory for integer vector of size n
int *create_vector( int n )
{
	int *vector;

	vector = new int[n];
	return vector;
}

// memory buffer for size n float vector
float *create_vector_f( int n )
{
	float *vector;

	vector = new float[n];
	return vector;
}

// reserves memory for entire matrix of size n x m
int **create_matrix( int n, int m )
{
	int i;
	int **matrix;

	matrix= new int*[n];
	for(i=0;i<n;i++)
		matrix[i] = new int[m];
	return matrix;
}

// reserve memory for float matrix of size n x m
float **create_matrix_f( int n, int m )
{
	int i;
	float **matrix;

	matrix = new float*[n];
	for(i=0;i<n;i++)
		matrix[i] = new float[m];
	return matrix;
}

// shows on screen vector of size n
int print_vector( int *vector, int n )
{
	int i;

	for(i=0;i<n;i++)
	{
		cout << i << "\t";
		cout << vector[i] << "\t" << endl;
	}
	return 0;
}

// shows on screen vector of size n
int print_vector( float *vector, int n )
{
	int i;

	for(i=0;i<n;i++)
	{
		cout << i << "\t";
		cout << vector[i] << "\t" << endl;
	}
	return 0;
}

// shows on screen matrix of size n x m
int print_matrix( int **matrix, int n, int m )
{
	int i, j;

	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			cout << matrix[i][j] << "\t";
		cout << endl;
	}
	return 0;
}

// shows on screen matrix of size n x m
int print_matrix( float **matrix, int n, int m )
{
	int i, j;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < m; j++)
			cout << sqrt(matrix[i][j]/3) << "\t";
			// cout << matrix[i][j] << "\t";
		cout << endl;
	}
	return 0;
}

// releases matrix memory of size n x m
int release_matrix( int **matrix, int n )
{
	int i;

	for(i=0;i<n;i++)
		delete[] matrix[i];
	delete[] matrix;
	return 0;
}

// releases matrix memory of size n x m
int release_matrix( float **matrix, int n )
{
	int i;

	for(i = 0; i < n; i++)
		delete[] matrix[i];
	delete[] matrix;
	return 0;
}
