#include <math.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include"flow_memory.h"
#include"flow_definition.h"
#include <chrono>
using namespace std;

// constructor of the system class
systems::systems( char *a_entrance )
{
	int i, j;
	char g[100];

	ifstream fp(a_entrance);				// initializes the ponterio for reading the file with the system data
	if( fp.is_open() && fp.good() )		// checks whether the opening of the input file was successful
	{
		fp >> g >> g >> g;
		fp >> n >> se >> vn;			// reading of the number of buses, the substation bus and the nominal voltage
		nl = n - 1;						// number of lines
		nl1 = nl - 1;					// number of lines subtracted from a unit
		emax = 100;						// flow stop criterion, loss variation [W]
		nitermax = 10;					// flow stop criterion, max iterations.
		nflow = 0;						// reset the counter of the number of load flows executed
		vn = vn * 1000;					// nominal voltage in (V)
		vn2 = vn * vn;					// squated nominal voltage
		d = create_vector(n);				// reserves memory for the vectors that store system information
		order = create_vector(n);
		nio = create_vector(nl);
		ni = create_vector(nl);
		nfo = create_vector(nl);
		nf = create_vector(nl);
		R = create_vector_f(nl);
		L = create_vector_f(nl);
		Z2 = create_vector_f(nl);
		P = create_vector_f(n);
		Q = create_vector_f(n);
		V2 = create_vector_f(n);
		I2 = create_vector_f(nl);
		Pi = create_vector_f(n);
		Qi = create_vector_f(n);

		fp >> g >> g >> g >> g;
		for( i = 0; i < n; i++ )		// resets the vector d, which indicates the number of lines connected to each bus
			d[i] = 0;
		for( i = 0; i < nl; i++ )		// reading line information
		{
			fp >> nio[i] >> nfo[i];		// reading of the starting and ending bus of the line
			fp >> R[i] >> L[i];			// line resistance and reactance reading (Ohm)
			ni[i] = nio[i];				// copy the initial bus, leaving the original info in the 'nio' vector
			nf[i] = nfo[i];				// copy the final bus, leaving the original info in the 'nfo' vector
			Z2[i] = R[i] * R[i]			// calculates the square of the line impedance
			      + L[i] * L[i];
			d[ni[i]] = d[ni[i]] + 1;	// increments the number of lines counter to the initial bus
			d[nf[i]] = d[nf[i]] + 1;	// increments the number of lines counter to the final bus
			// cout << "R[i]\t" << R[i] << endl;
		}

		dmax = 0;						// resets the variable of the maximum number of lines connected to a bus for the entire system
		for( i = 0; i < n; i++ )		// evaluates all buses
			if(d[i] > dmax)				// if the number of lines connected to the bus is greater than the value of dmax ...
				dmax = d[i];			// updates the dmax variable
		D = create_matrix(n,dmax);		// reserves memory for matrix D, with size: n rows and dmax columns
		for( i = 0; i < n; i++ )		// resets the vector d to traverse the columns of matrix D
			d[i] = 0;
		for( i = 0; i < nl; i++ )		// evaluates all lines
		{
			D[ni[i]][d[ni[i]]] = i;		// in the line corresponding to the starting bus of line 'i', in the column indicated by counter 'd' for this bus, store line 'i'
			d[ni[i]] = d[ni[i]] + 1;	// increments the 'd' counter to the starting bus of the 'i' line
			D[nf[i]][d[nf[i]]] = i;		// in the line corresponding to the end bus of line 'i', in the column indicated by counter 'd' for this bus, store line 'i'
			d[nf[i]] = d[nf[i]] + 1;	// increments the 'd' counter to the end bus of the 'i' line
		}
		
		fp >> g >> g >> g;				// load data reading
		stre = 0;						// resets the variable that stores the total active power of the loads
		stim = 0;						// resets the variable that stores the total reactive power of the loads
		for( i = 0; i < n; i++ )		// for all buses
		{
			fp >> j;					// bus reading
			fp >> P[j] >> Q[j];			// reading of the active and reactive power of the load at that bus
			P[j] = P[j] * 1000;			// conversion of active power from (kW) to (W)
			Q[j] = Q[j] * 1000;			// conversion of reactive power from (kVAr) to (VAr)
			stre = stre + P[j];			// updates the sum of the active power of the loads
			stim = stim + Q[j];			// updates the sum of the reactive power of the loads
			// cout << "P[j]\t" << P[j] << endl;
		}
		fp.close();						// closes the file with system data
	}
}

// destructor of the system class,
// this function is called automatically at the end of the program execution
systems::~systems()
{
	release_matrix(D,n);				// frees memory reserved for matrix D
	delete[] d;							// frees reserved memory for the vectors used
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
int systems::report()
{
	int i;

	cout << "\nreport" << endl;
	// cout << nflow << " power flow" << endl;					// prints the number of load streams made
	//cout << "niter\t" << niter << endl;						// number of iterations for convergence of the last load flow run
	cout << "power losses (kW)\t" << psere/1000 << endl;		// active power losses (kW)
	cout << "time (s)\t" << setprecision(20) << timeP;
	//cout << "time total (s)\t" << setprecision(20) << timePTOTAL;
	cout << setprecision(20);
	cout << endl;
	vmin = vn2;
	for( i = 0; i < n; i++ )									// calculates the tension of the buses in pu
	{
		if ( V2[i] < vmin)
			vmin = V2[i];
		V2[i] = sqrt( V2[i] ) / vn;
	}
	//cout << "vmin (pu) \t" << sqrt( vmin ) / vn << endl;
	//cout << "\ntensoes\nbuses\tV (pu)" << endl;				// prints the voltage of the buses
	//print_vector(V2,n);
	//cout << "\ncorrentes\nno\tnf\tI (A)" << endl;
//	for( i = 0; i < nl; i++ )									// prints the currents of the lines
//		cout << ni[i] << "\t" << nf[i] << "\t" << sqrt( I2[i] / 3 ) << endl;
//for( i = 0; i < n; i++ )										// prints the currents of the lines
//		cout << i << "\t" << Pi[i]/1000 << "\t" << Qi[i]/1000 << endl;

	// cout << "D" << endl;
	// print_matrix(D,n,dmax);
	//cout << "order" << endl;
	//print_vector(order,nl);
	// for( i = 0; i < nl; i++ )									// prints the currents of the lines
		// cout << ni[order[i]] << "\t" << nf[order[i]] << endl;

	return 0;
}

// function that performs the ordering of lines by levels must be performed before the load flow,
// the order of the lines is stored in the 'order' vector, depending on the system topology and the initial buses
// and final can be exchanged in relation to the initial data.
//
// the ordering is based on the idea of ordering the lines starting with those connected to the substation, and then processing
// these lines identifying the final bus and adding new lines connected to that final bus; new lines are added
// in the 'order' vector; the process is done until there are no lines to process.
int systems::ordering()
{
	int i, j, k, kv, lin, linaux, w, nrow;

	nrow = 0;								// clears the ordered line counter
	for( j = 0; j < d[se]; j++ )			// evaluates the lines connected to the substation, d [if] indicates this # of lines
	{
		lin = D[se][j];						// identifies a line connected to the substation
		order[nrow] = lin;					// put this line in the order vector
		nrow = nrow + 1;					// increments the ordered line counter
		if( ni[lin] !=  se )				// if the starting bus of the line is different from the substation ...
		{									// then make the initial and final bus swap
			nf[lin] = ni[lin];				// the start bus of the line becomes the end bus
			ni[lin] = se;					// the substation becomes the starting bus of the line
		}
	}
	kv = -1;								// initializes the counter of lines already processed
	k = 0;									// reset the line counter
	while( k !=  kv )						// until there are unprocessed lines
	{
		kv = k;								// update the counter of processed lines
		k = nrow;							// update the row counter
		for( i = kv; i < k; i++ )			// for unprocessed lines
		{                                   
			linaux = order[i];				// identifies the line to be processed
			w = nf[linaux];					// identifies the end bus of the line without processing ('w')
			for( j = 0; j < d[w]; j++ )		// for lines connected to the final bus 'w'
			{                               
				lin = D[w][j];				// identifies the line connected to the final bus 'w'
				if( lin !=  linaux )		// if this line is different from the unprocessed line ...
				{                           
					order[nrow] = lin;		// add the line to the order vector
					nrow = nrow + 1;		// increments the ordered line counter
					if( ni[lin] !=  w )		// the end bus of the line being processed 'w', must be the end bus of the new line without processing
					{						// otherwise make the switch
						nf[lin] = ni[lin];	// the start bus of the identified line becomes the end bus
						ni[lin] = w;		// the end bus of the line being processed 'w', becomes the start bus of the identified line
					}
				}
			}
		}
	}
	return 0;
}

// function that performs the power flow, IMPORTANT: for efficiency reasons the voltages are squared
// and the chains are squared and multiplied by 3 it is preferable to use the values like this and not calculate the square root
// whenever possible, see in the report function how to obtain normal values
// see reference: Cespedes, R. G. (1990). "New method for the analysis of
// distribution networks. "IEEE Transactions on Power Del. vol.5, no.1, pp.391-396, Jan 1990.
float systems::flow_power()
{
	int i, j, k, h;
	double pseo;

	for( i = 0; i < n; i++ )			// initializes the bus voltages
		V2[i] = vn2;                    
	pseo = 0;							// reset the variable that stores the losses from the previous iteration
	psere = 1e10;						// initializes losses
	niter = 0;							// reset the iteration counter

	// run the cycle as long as the difference in losses from two successive iterations is
	// greater than the allowed error and the number of eiterations is less than the maximum number allowed
	while( fabs(psere-pseo) > emax && niter < nitermax )
	{
		niter = niter + 1;				// increment the iteration counter
		pseo = psere;					// updates the power losses from the previous iteration
		for( i = 0; i < n; i++ )		// for all buses
		{								// initializes the power of the bus with the power of the load on that bus
			Pi[i] = P[i];
			Qi[i] = Q[i];
		}

		for( h = nl1; h >= 0; h-- )						// power backward scan according to the order vector
		{                                               
			i = order[h];								// identifies line i according to the order vector
			k = ni[i];									// identifies the starting bus of line i
			j = nf[i];									// identifies the end bus of line i
			I2[i] = ( Pi[j] * Pi[j]						// current of line i squared multiplied by 3
					+ Qi[j] * Qi[j] ) / V2[j];          
			Pi[k] = Pi[k] + Pi[j] + R[i] * I2[i];		// updates the active power of the starting bus with the active power of the final bus and line losses
			Qi[k] = Qi[k] + Qi[j] + L[i] * I2[i];		// updates the reactive power of the initial bus with the reactive power of the final bus and line losses
		}                                               
                                                        
		for( h = 0; h < nl; h++ )						// 'forward' sweeping of voltage according to the order vector
		{                                               
			i = order[h];								// identifies line i according to the order vector
			j = nf[i];									// identifies the end bus of line i
			V2[j] = V2[ni[i]] - I2[i] * Z2[i]			// calculates the square of the voltage of the final bus according to the square of the voltage
					- 2 * (Pi[j] * R[i] + Qi[j] * L[i]);// of the starting bus, current, power flow and line impedance
		}                                               
		psere = Pi[se];									// calculates losses as the active power of the substation
	}
	psere = psere - stre;		// calculates losses as the active power of the substation minus the total active power of the loads
	nflow = nflow + 1;		// increments the counter of the number of load flows performed
	return psere;				// returns active power losses
}
