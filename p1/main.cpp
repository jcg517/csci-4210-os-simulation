#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>


int main(int argc, char* argv[])
{
    if (argc != 6) 
    {
		std::cerr << "ERROR: <error-text-here>\n";
		exit(1);
	}

    int n = *(argv+1);          /* # of processes to simulate */
    int n_cpu = *(argv+2);      /* # of CPU-bound processes */
    int seed = *(argv+3);       /* Rand num gen seed */
    int lambda = *(argv+4);     
    int ceil = *(argv+5);

}