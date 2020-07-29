#include <math.h>
#include <iostream>
using namespace std;

// reserves memory for integer vector of size n
int *crear_vector( int n )
{
	int *vector;

	vector = new int[n];
	return vector;
}

// memory buffer for size n float vector
float *crear_vector_f( int n )
{
	float *vector;

	vector = new float[n];
	return vector;
}

// reserves memory for entire matrice of size n x m
int **crear_matrice( int n, int m )
{
	int i;
	int **matrice;

	matrice= new int*[n];
	for(i=0;i<n;i++)
		matrice[i] = new int[m];
	return matrice;
}

// reserve memory for float matrice of size n x m
float **crear_matrice_f( int n, int m )
{
	int i;
	float **matrice;

	matrice = new float*[n];
	for(i=0;i<n;i++)
		matrice[i] = new float[m];
	return matrice;
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

// shows on the screen matrice of size n x m
int print_matrice( int **matrice, int n, int m )
{
	int i, j;

	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
			cout << matrice[i][j] << "\t";
		cout << endl;
	}
	return 0;
}

// shows on the screen matrice of size n x m
int print_matrice( float **matrice, int n, int m )
{
	int i, j;

	for(i = 0; i < n; i++)
	{
		for(j = 0; j < m; j++)
			cout << sqrt(matrice[i][j]/3) << "\t";
			// cout << matrice[i][j] << "\t";
		cout << endl;
	}
	return 0;
}

// releases matrice memory of size n x m
int release_matrice( int **matrice, int n )
{
	int i;

	for(i=0;i<n;i++)
		delete[] matrice[i];
	delete[] matrice;
	return 0;
}

// releases matrice memory of size n x m
int release_matrice( float **matrice, int n )
{
	int i;

	for(i = 0; i < n; i++)
		delete[] matrice[i];
	delete[] matrice;
	return 0;
}
