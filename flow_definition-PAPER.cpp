#include <math.h>
#include <iostream>
#include <fstream>
#include"fluxo_memoria-PAPER.h"
#include"fluxo_definition-PAPER.h"
#include <chrono>
using namespace std;

// system class constructor
system::system( char *a_input )
{
	int i, j;
	char g[100];

	ifstream fp(a_input);				// initializes the ponterio for reading the file with the system data
	if( fp.is_open() && fp.good() )		// checks if the input file was opened successfully
	{
		fp >> g >> g >> g;
		fp >> n >> se >> vn;			// reading of the number of bus, the substation bus and the nominal voltage
		nl = n - 1;						// number of lines
		nl1 = nl - 1;					// number of lines subtracting one
		emax = 100;						// flow stop criterion, loss variation [W]
		nitermax = 10;					// flow stop criterion, max iterations.
		nflow = 0;						// reset the counter of the number of load flows executed
		vn = vn * 1000;					// rated voltage in (V)
		vn2 = vn * vn;					// rated voltage squared
		d = crear_vector(n);				// reserves memory for the vectors that store system information
		order = crear_vector(n);
		nio = crear_vector(nl);
		ni = crear_vector(nl);
		nfo = crear_vector(nl);
		nf = crear_vector(nl);
		R = crear_vector_f(nl);
		L = crear_vector_f(nl);
		Z2 = crear_vector_f(nl);
		P = crear_vector_f(n);
		Q = crear_vector_f(n);
		V2 = crear_vector_f(n);
		I2 = crear_vector_f(nl);
		Pi = crear_vector_f(n);
		Qi = crear_vector_f(n);

		fp >> g >> g >> g >> g;
		for( i = 0; i < n; i++ )		// resets the vector d, which indicates the number of lines connected to each bar
			d[i] = 0;
		for( i = 0; i < nl; i++ )		// reading line information
		{
			fp >> nio[i] >> nfo[i];		// reading of the start and end node of the line
			fp >> R[i] >> L[i];			// line resistance and reactance reading (Ohm)
			ni[i] = nio[i];				// copy the initial node, leaving the original info in the 'nio' vector
			nf[i] = nfo[i];				// copy the final node, leaving the original info in the vector 'nfo'
			Z2[i] = R[i] * R[i]			// calculates the square of the line impedance
			      + L[i] * L[i];
			d[ni[i]] = d[ni[i]] + 1;	// increments the number of lines counter to the initial node
			d[nf[i]] = d[nf[i]] + 1;	// increments the number of lines counter to the final node
			// cout << "R[i]\t" << R[i] << endl;
		}

		dmax = 0;						// resets the variable of the maximum number of lines connected to a bus for the entire system
		for( i = 0; i < n; i++ )		// evaluates all nodes
			if(d[i] > dmax)				// if the number of lines connected to the node is greater than the value of dmax ...
				dmax = d[i];			// updates the dmax variable
		D = crear_matrice(n,dmax);		// reserves memory for matrix D, with size: n rows and dmax columns
		for( i = 0; i < n; i++ )		// resets the vector d to traverse the columns of matrix D
			d[i] = 0;
		for( i = 0; i < nl; i++ )		// evaluates all lines
		{
			D[ni[i]][d[ni[i]]] = i;		// in the line corresponding to the starting node of line 'i', in the column indicated by the counter 'd' for this node, store the line 'i'
			d[ni[i]] = d[ni[i]] + 1;	// increments the 'd' counter to the starting node of the 'i' line
			D[nf[i]][d[nf[i]]] = i;		// in the line corresponding to the end node of line 'i', in the column indicated by counter 'd' for this node, store line 'i'
			d[nf[i]] = d[nf[i]] + 1;	// increments the 'd' counter to the end node of the 'i' line
		}
		
		fp >> g >> g >> g;				// load data reading
		stre = 0;						// resets the variable that stores the total active power of the loads
		stim = 0;						// resets the variable that stores the total reactive power of the loads
		for( i = 0; i < n; i++ )		// for all of us
		{
			fp >> j;					// node reading
			fp >> P[j] >> Q[j];			// reading of the active and reactive power of the load at that node
			P[j] = P[j] * 1000;			// conversion of active power from (kW) to (W)
			Q[j] = Q[j] * 1000;			// conversion of reactive power from (kVAr) to (VAr)
			stre = stre + P[j];			// updates the sum of the active power of the loads
			stim = stim + Q[j];			// updates the sum of the reactive power of the loads
			// cout << "P[j]\t" << P[j] << endl;
		}
		fp.close();						// closes the file with system data
	}
}

// system class destructor,
// this function is called automatically at the end of the program execution
system::~system()
{
	release_matrice(D,n);				// libera a memória reservada para a matriz D
	delete[] d;							// libera memória reservada para os vetores utilizados
	delete[] order;
	delete[] ni;
	delete[] nf;
	delete[] R;
	delete[] L;
	delete[] Z2;
	delete[] V2;
	delete[] P;
	delete[] Q;
	delete[] I2;
	delete[] Pi;
	delete[] Qi;
}

// shows a report with the results of the load flow
int system::release()
{
	int i;

	cout << "\nRelease" << endl;
	// cout << nflow << " power flow" << endl;					// prints the number of load streams made
	cout << "niter\t" << niter << endl;							// number of iterations for convergence of the last load flow run
	// cout << "power losses (kW)\t" << psere/1000 << endl;	// active power losses (kW)
		cout << "time (s)\t" << setprecision(20) << tempoP;
	cout << "total time (s)\t" << setprecision(20) << tempoPTOTAL;
	cout << setprecision(8);
	cout << endl;
	vmin = vn2;
	for( i = 0; i < n; i++ )									// calculates the tension of the bus in pu
	{
		if ( V2[i] < vmin)
			vmin = V2[i];
		V2[i] = sqrt( V2[i] ) / vn;
	}
	cout << "vmin (pu) \t" << sqrt( vmin ) / vn << endl;
	cout << "\nvoltage\nbus\tV (pu)" << endl;					// prints voltage of the bus
	imprimir_vetor(V2,n);
	cout << "\ncurrent\nno\tnf\tI (A)" << endl;
	for( i = 0; i < nl; i++ )									// prints current of the bus
		cout << ni[i] << "\t" << nf[i] << "\t" << sqrt( I2[i] / 3 ) << endl;
for( i = 0; i < n; i++ )									// prints the chains of the lines
		cout << i << "\t" << Pi[i]/1000 << "\t" << Qi[i]/1000 << endl;

	// cout << "D" << endl;
	// print_matrice(D,n,dmax);
	// cout << "order" << endl;
	// print_vector(order,nl);
	// for( i = 0; i < nl; i++ )									// prints the chains of the lines
		// cout << ni[order[i]] << "\t" << nf[order[i]] << endl;

	return 0;
}

// function that arranges lines by levels
// must be executed before the load flow,
// the order of the lines is stored in the 'order' vector,
// depending on the system topology the initial nodes
// and final can be exchanged in relation to the initial data.
//
// the ordering is based on the idea of ordering the lines
// starting with those connected to the substation, and then processing
// these lines identifying the final node and adding new ones
// lines connected to that final node; new lines are added
// in the 'order' vector; the process is done until there are no lines
// to process
int system::ordering()
{
	int i, j, k, kv, lin, linaux, w, nfila;

	nfila = 0;								// clears the ordered line counter
	for( j = 0; j < d[se]; j++ )			// evaluates the lines connected to the substation, d [if] indicates this # of lines
	{
		lin = D[se][j];						// identifies a line connected to the substation
		order[nfila] = lin;					// put that line in the order vector
		nfila = nfila + 1;					// increments the ordered line counter
		if( ni[lin] !=  se )				// if the initial node of the line is different from the substation ...
		{									// then make the initial and final node swap
			nf[lin] = ni[lin];				// the start node of the line becomes the end node
			ni[lin] = se;					// the substation becomes the starting node of the line
		}
	}
	kv = -1;								// initializes the counter of lines already processed
	k = 0;									// reset the line counter
	while( k !=  kv )						// until there are lines without processing
	{
		kv = k;								// update the processed line counter
		k = nfila;							// updates the row counter
		for( i = kv; i < k; i++ )			// for lines without processing
		{
			linaux = order[i];				// identifies the line to be processed
			w = nf[linaux];					// identifies the end node of the line without processing ('w')
			for( j = 0; j < d[w]; j++ )		// for lines connected to the final node 'w'
			{
				lin = D[w][j];				// identifies the line connected to the final node 'w'
				if( lin !=  linaux )		// if that line is different from the line without processing ...
				{
					order[nfila] = lin;		// add the line to the order vector
					nfila = nfila + 1;		// increments the ordered line counter
					if( ni[lin] !=  w )		// the end node of the line being processed 'w', must be the end node of the new line without processing
					{						// otherwise make the switch
						nf[lin] = ni[lin];	// the start node of the identified line becomes the end node
						ni[lin] = w;		// the end node of the line being processed 'w', becomes the start node of the identified line
					}
				}
			}
		}
	}
	return 0;
}

// função que executa o fluxo de potência
// IMPORTANTE: por motivos de eficiência as tensões ficam ao quadrado
// e as correntes ficam ao quadrado e multiplicadas por 3
// é preferente usar os valores assim e não calcular a raiz quadrada
// sempre que possível, ver na função do relatório como obter os valores normais
// ver a referência: Cespedes, R. G. (1990). "New method for the analysis of
// distribution networks." IEEE Transactions on Power Del. vol.5, no.1, pp.391-396, Jan 1990
float sistema::fluxo_potencia()
{
	int i, j, k, h;
	double pseo;

	for( i = 0; i < n; i++ )			// inicializa as tensões das barras
		V2[i] = vn2;
	pseo = 0;							// zera a variável que armazena as perdas da iteração anterior
	psere = 1e10;						// inicializa as perdas
	niter = 0;							// zera o contador de iterações

	// executar o ciclo enquanto a diferença das perdas de dois iterações sucessivas seja
	// maior que o erro permitido e o número d eiterações seja menor que o número máximo permitido
	while( fabs(psere-pseo) > emax && niter < nitermax )
	{
		niter = niter + 1;				// incrementa o contador de iterações
		pseo = psere;					// atualiza as perdas de potência da iteração anterior
		for( i = 0; i < n; i++ )		// para todas as barras
		{								// inicializa a potência da barra com a potência da carga nessa barra
			Pi[i] = P[i];
			Qi[i] = Q[i];
		}

		for( h = nl1; h >= 0; h-- )						// varredura 'backward'de potências segundo o vetor ordem
		{
			i = ordem[h];								// identifica a linha i segundo o vetor ordem
			k = ni[i];									// identifica o nó inicial da linha i
			j = nf[i];									// identifica o nó final da linha i
			I2[i] = ( Pi[j] * Pi[j]						// corrente da linha i ao quadrado multiplicada por 3
					+ Qi[j] * Qi[j] ) / V2[j];
			Pi[k] = Pi[k] + Pi[j] + R[i] * I2[i];		// atualiza a potência ativa do nó inicial com a potência ativa do nó final e as perdas da linha
			Qi[k] = Qi[k] + Qi[j] + L[i] * I2[i];		// atualiza a potência reativa do nó inicial com a potência reativa do nó final e as perdas da linha
		}

		for( h = 0; h < nl; h++ )						// varredura 'forrward' de tensões segundo o vetor ordem
		{
			i = ordem[h];								// identifica a linha i segundo o vetor ordem
			j = nf[i];									// identifica o nó final da linha i
			V2[j] = V2[ni[i]] - I2[i] * Z2[i]			// calcula o quadrado da tensão do nó final segundo o quadrado da tensão
					- 2 * (Pi[j] * R[i] + Qi[j] * L[i]);// do nó inicial, da corrente, do fluxo de potência e da impedância da linha
		}
		psere = Pi[se];									// calcula as perdas como a potência ativa da subestação
	}
	psere = psere - stre;		// calcula as perdas como a potência ativa da subestação menos o total de pot~encia ativa das cargas
	nfluxo = nfluxo + 1;		// incrementa o contador do número de fluxos de carga executados
	return psere;				// retorna as perdas de potência ativa
}
