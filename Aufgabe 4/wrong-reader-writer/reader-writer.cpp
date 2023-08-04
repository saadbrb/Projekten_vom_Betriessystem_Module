#include <iostream>     // for std::cout, cin, etc.
#include <thread>       // for std::thread
#include <cassert>      // for assert()
#include <cstdlib>      // for atoi()
#include <vector>       // for std::vector

#include "database.h"
#include "reader-writer-threads.h"

const int maxThreads = 1024; // increase if you want, meant (only) as sanity check.
const int maxSeconds = (60*60*24); // one day, also just a sanity check.

/********************************************* * main entry point *********************************************/
int main(int argc, char *argv[])
{
    if ( argc != 4 ) {
        std::cerr << "Usage: "
                  << argv[0]
                  << " numSeconds numThreads percentReaders\n" 
                  << "        Where 0 < numSeconds < " << maxSeconds << " (1 day)"
                  << " and 0 < numThreads < " << maxThreads 
                  << " and 0.0 <= percentReaders <= 100.0\n";
        return( -1 );
    }

    int numSeconds = atoi( argv[1] );
    int numThreads = atoi( argv[2] );
    double percentReaders = atof( argv[3] ) / 100.0;
    
    std::cout << "In " << argv[0]
              << ", seconds = " << numSeconds
              << ", threads = " << numThreads
              << ", % readers = "    << percentReaders * 100.0
              << ", ready to go..." << std::endl;

    // The following is strictly speaking misuse of the assert macro,
    // but our target user base will understand the cryptic error message.
    assert( (0 < numSeconds) && (numSeconds < (60*60*24)) ); // max 1 day
    assert( (0.0 <= percentReaders) && (percentReaders <= 1.0));
    assert( (0 < numThreads) && (numThreads < maxThreads));

    // Create reader-writer threads.
    std::vector< std::thread > worker( numThreads );

    // Let them run.
    for ( int t = 0; t < numThreads; ++t ) {
        worker[ t ] = std::thread( readerwriter, t, percentReaders, numSeconds );
    }; // end for 0 <= t < numThreads
    std::cout <<  "Finished creating all threads..." << std::endl;

    // While the threads are running, we have nothing to do,
    // but wait for writers and readers to finish:
    for ( std::thread& w : worker )
        w.join();

    // Wrap up and go home
    theDatabase.printStatistics();
    if (numSeconds < 60)
        std::cout << "WARNING: Since time was only "
                  << numSeconds << " seconds, the statistics above mean NOTHING."
                  << std::endl;

    std::cout << "Simulation finished! Database is "
              << ( theDatabase.ok() ? "OK!" : "NOT OK!!!" ) << std::endl;

    return( theDatabase.ok() ? 0 :  -2 );
    
} // end main

