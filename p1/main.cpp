#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <list>
#include <iomanip>

#include "opsys.h"


double nextExp(double lambda, int ceil)
{
    while (true)
    {
        double x = -log(drand48()) / lambda;
        if (x < ceil) return x;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 6) 
    {
		std::cerr << "ERROR: Invalid arg count\n";
		exit(1);
	}

    int n = atoi(*(argv+1));          /* # of processes to simulate */
    int n_cpu = atoi(*(argv+2));      /* # of CPU-bound processes */
    int seed = atoi(*(argv+3));       /* Rand num gen seed */
    double lambda = atof(*(argv+4));     
    int ceil = atoi(*(argv+5));

    if (n > 259 || n <= 0 || n_cpu > n)
    {
        std::cerr << "ERROR: Invalid process simulation count\n";
        exit(1);
    }

    if (lambda == 0)
    {
        std::cerr << "ERROR: Lambda cannot be zero\n";
        exit(1);
    }

    std::cout << "<<< PROJECT PART I\n"
              << "<<< -- process set (n=" << *(argv+1) << ") with " << *(argv+2)
              << (n_cpu != 1 ? " CPU-bound processes\n" : " CPU-bound process\n") << "<<< -- seed=" << *(argv+3) << "; "
              << "lambda=" << std::fixed << std::setprecision(6) << lambda << "; " 
              << "bound=" << *(argv+5) << "\n";

    srand48( seed );    // Seed rand num generator

    std::list<Process*> processes;

    float cpu_cpu_total = 0, io_cpu_total = 0, cpu_io_total = 0, io_io_total = 0;
    float num_cpu_cpu = 0, num_io_cpu = 0, num_cpu_io = 0, num_io_io = 0;

    
    // Process creation loop
    for (int i = 0; i < n; i++)
    {
        // Determine process type:
        bool is_cpu_bound = false;
        if (i < n_cpu) is_cpu_bound = true;
        
        // Process arrival time
        int p_arrival_time = floor(nextExp(lambda, ceil));
        
        // Burst times
        int num_cpu = std::ceil(drand48() * 32);      // # CPU bursts
        int num_io = num_cpu - 1;
        int num_total = num_cpu + num_io;
        int* burst_times = new int[num_total];

        // fill combined burst times list
        for (int j = 0; j < num_total; j++)
        {
            int x = std::ceil(nextExp(lambda, ceil));

            if (is_cpu_bound)
            {
                if (j % 2 == 0)
                {
                    x = x*4;
                    cpu_cpu_total += x; num_cpu_cpu++;
                } 
                else 
                { cpu_io_total += x; num_cpu_io++; }
            }      
            else {
                if (j % 2 == 1)
                {
                    x *= 8;
                    io_io_total += x; num_io_io++;
                }
                else
                { io_cpu_total += x; num_io_cpu++; }
            }
            
            burst_times[j] = x;
        }

        Process* p = new Process();
        p->id = new char[3];
        std::sprintf(p->id, "%c%d", 'A' + (i / 10), i % 10);
        p->is_cpu_bound = is_cpu_bound;
        p->num_cpu = num_cpu;
        p->burst_index = 0;
        p->burst_times = burst_times;
        p->arrival_time = p_arrival_time;

        processes.push_back(p);
    }  

    float cpu_cpu_avg = num_cpu_cpu != 0 ? (cpu_cpu_total / num_cpu_cpu) : 0;
    float cpu_io_avg = num_cpu_io != 0 ? (cpu_io_total / num_cpu_io) : 0;
    float cpu_avg = std::ceil(1000 * (cpu_cpu_total + io_cpu_total) / (num_cpu_cpu + num_io_cpu)) / 1000;
    float io_cpu_avg = num_io_cpu != 0 ? (io_cpu_total / num_io_cpu) : 0;
    float io_io_avg = num_io_io != 0 ? (io_io_total / num_io_io) : 0;
    float io_avg = std::ceil(1000 * (cpu_io_total + io_io_total) / (num_cpu_io + num_io_io)) / 1000;

    for (Process* p : processes)
    {
        // Process name

        if (p->is_cpu_bound) std::cout << "CPU-bound";
        else std::cout << "I/O-bound";
        std::cout << " process " << p->id << ": arrival time " << p->arrival_time << "ms; "
            << p->num_cpu << (p->num_cpu != 1 ? " CPU bursts:\n" : " CPU burst:\n");
        for (int j = 0; j < p->num_cpu * 2 - 1; ++j)
        {
            if (j % 2 == 0)
                std::cout << "==> CPU burst " << p->burst_times[j] << "ms";
            else std::cout << " ==> I/O burst " << p->burst_times[j] << "ms\n";
        }
        std::cout << "\n";
    }

    ofstream simout;
    simout.open("simout.txt");
    simout << "-- number of processes: " << n << "\n";
    simout << "-- number of CPU-bound processes: " << n_cpu << "\n";
    simout << "-- number of I/O-bound processes: " << (n - n_cpu) << "\n";
    simout << "-- CPU-bound average CPU burst time: " 
           << std::fixed << std::setprecision(3) << std::ceil(1000 * cpu_cpu_avg) / 1000 << " ms\n";
    simout << "-- I/O-bound average CPU burst time: " 
           << std::fixed << std::setprecision(3) << std::ceil(1000 * io_cpu_avg) / 1000 << " ms\n";
    simout << "-- overall average CPU burst time: " 
           << std::fixed << std::setprecision(3) << cpu_avg << " ms\n";
    simout << "-- CPU-bound average I/O burst time: " 
           << std::fixed << std::setprecision(3) << std::ceil(1000 * cpu_io_avg) / 1000 << " ms\n";
    simout << "-- I/O-bound average I/O burst time: " 
           << std::fixed << std::setprecision(3) << std::ceil(1000 * io_io_avg) / 1000 << " ms\n";
    simout << "-- overall average I/O burst time: " 
           << std::fixed << std::setprecision(3) << io_avg << " ms\n";
    simout.close();

    for (Process* p : processes)
    {
        delete [] p->id;
        delete [] p->burst_times;
        delete p;
    }

    return 0;
}


// num processes
