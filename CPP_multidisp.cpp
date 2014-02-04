#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <utility>
#include <ctime>
#include <cmath>
#include <string.h>


using namespace std;

int main(int argc, char** argv)
{
    time_t start, end;
    double deltatime;
    start = time(0);
    int nthreads;
    if(argc == 1 || strcmp(argv[1],"-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        cout << "Help for " << argv[0] << endl;
        cout << "<argfile>: File containing a list of commands" << endl;
        cout << "<nthreads>: Number of threads to use" << endl;

        return -1;
    }
    else
    {
        nthreads = atoi(argv[2]);
        omp_set_num_threads(nthreads); // Using OpenMP for the parellel processing 
        pair< vector<string>, vector<string> > params;
        
        ifstream cmds(argv[1]);// open the file containing the commands we want to process, ready for reading
        string line;
        vector<string> lines;
        while(!cmds.eof())
        {
            getline(cmds, line, '\n');// Read a line from the file
            lines.push_back(line);
        }
        cmds.close();
        lines.pop_back();

        int tid(0);
        #ifdef __MIC__
        #pragma offloat target(mic)// Should allow the offloading of the following openmp loop onto the mics, hopefully it will then launch the processes there...
        #endif
        #pragma omp parallel for private(tid)
        for(int k=0; k<lines.size();k++)
        {
            tid = omp_get_thread_num();
            system((lines[k].c_str() ));
        }
    }
    end = time(NULL);
    deltatime = difftime(end, start)/60;// Get the time taken in seconds
    cout << "Total time taken: " << deltatime << " s" << endl;
    return 0;
}
