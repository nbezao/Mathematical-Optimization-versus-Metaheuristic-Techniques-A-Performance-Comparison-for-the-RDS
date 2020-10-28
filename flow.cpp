#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
#include <iomanip>
#include"flow_definition.h"
#include"flow_memory.h"

using namespace std;

int main(int argc, char* argv[])
{
	clock_t start, final;									// variables to calculate the program execution time
	//double timeP;

	cout.precision(6);										// sets the price to print float variables
	cout.setf(ios::fixed);
	if (argc < 2)
		cout << "It is necessary to pass the file with the system data to simulate as a program argument" << endl;
	else
	{
		ifstream fp(argv[1]);								// initializes the ponterio for reading the file ARQUIVO_CONFIGURATION
		if (fp.is_open() && fp.good())						// checks whether the opening of the input file was successful
		{
			fp.close();
			systems sp(argv[1]);							// creates the 'sp' instance of the 'system' class with the input file
			start = clock();								// initializes the runtime
			for (int i = 1; i <= 1000000; i++) {
				sp.ordering();							// performs the ordering of system lines
				sp.flow_power();						// performs the load flow

				// cout << "loss" << sp.psere/1000 << endl;	// prints active power losses (kW)
				// cout << "iteration: " << sp.ni << endl;	// prints the number of iterations of the load flow
			}
			final = clock();								// prints the runtime
			cout << setprecision(20) << "time\t" << ((float)(final - start)) / ((float)CLOCKS_PER_SEC) << " s." << endl;
			cout << setprecision(8);
			sp.timeP = ((float)(final - start)) / ((float)CLOCKS_PER_SEC);
			sp.report();									// show the report
		}
		else
			cout << "Error reading file" << endl;
	}
		//std::chrono::duration<double> elapsed_seconds = final - start;
	// double elapsed_seconds = (final - start);
	//std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	// std::cout << "elapsed time: " << elapsed_seconds << "s\n";
	return(0);
}
