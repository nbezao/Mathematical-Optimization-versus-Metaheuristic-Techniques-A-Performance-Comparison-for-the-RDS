#ifndef FLOW_DEFINITION_H
#define FLOW_DEFINITION_H

class system							// system class definition
{
	public:
		system( char *a_input );		// class constructor using as input a file with the information
		~system();						// class destructor
		int ordering();				// function that makes the ordering of lines necessary before turning the load flow
		float flow_power();			// function that runs the load flow
		int report();				// function that shows a report with the results of the load flow

		int n;							// number of system bus
		int se;							// identifies the substation bus
		int nl;							// number of system lines
		int nl1;						// number of system lines subtracting one
		int nitermax;					// maximum number of iterations for load flow convergence
		float emax;						// maximum loss delta (W) for load flow convergence
		int niter;						// load flow iteration counter
		int nflow;						// counter of the number of load flows performed
		int nfila;						// auxiliary variable in ordering
		int dmax;						// indicates the maximum number of lines connected to a bus for the entire system
		float vn;						// rated system voltage (V)
		float vn2;						// squared system voltage
		int *nio, *ni;					// pointer that points to the vector of initial nodes of each line
		int *nfo, *nf;					// pointer that points to the end node vector of each line
		int *d;							// pointer that points to the vector that indicates for each node how many lines it has connected
		int *order;						// pointer that points to the vector that indicates the ordering of lines
		float *R, *L, *Z2;				// line resistances and reactances
		float *V2;						// squared bar stresses
		float *P, *Q;					// load power (W, VAr)
		float *I2;						// squared current in bars (A)
		float *Pi, *Qi;					// active and reactive power in the bars: sum of the loads and powers downstream (kW, kVAr)
		float stre, stim;				// variables that store total active and reactive demand (W, VAr)
		float psere;					// variable that stores active power losses (W)
		float vmin;						// variable that stores the minimum system voltage, determined with the results of the load flow
		int **D;						// point that points to the matrix that identifies, in the columns of the line k-th of the matrix, which are the lines connected to node k
		double tempoP;					// time required to execute the power flow
		double tempoPTOTAL;				// total time required to execute the power flow
};

#endif