//1. Loesung
#include <iostream>         // for std::cin, cerr, cout ...
#include <thread>           // for std::this_thread
#include <chrono>           // for std::chrono...
#include <cassert>      // for assert()
#include <semaphore>

#include "database.h"
#include "reader-writer-threads.h"


// ******** reader & writer threads ********

// The worker thread
std::counting_semaphore db{1};
std::counting_semaphore mutex{1};
int rc = 0;

void readerwriter( int workerID, double percentReader, int numSeconds ) {

    assert( (0.0 <= percentReader) && (percentReader <= 1.0));

    std::cout << "Worker " << workerID << " starting..." << std::endl;

    int tests = 0;
    int reads = 0;
    int writes = 0;
    auto startTime = std::chrono::steady_clock::now();
    std::chrono::seconds maxTime( numSeconds );
    while ( ( std::chrono::steady_clock::now() - startTime ) < maxTime ) {

        // Choose a random number to decide if we're a Reader or a Writer (at the moment)
        const int maxRandomNumber = 100*1000;
        int randomNumber = randomInt( maxRandomNumber );
        bool reader = ( randomNumber < (percentReader * maxRandomNumber));
        bool result;

        if ( reader ) {
            mutex.acquire();
            rc++;
            if (rc == 1)
                db.acquire();
            mutex.release();
            result = theDatabase.read( workerID );
            mutex.acquire();
            rc--;
            if(rc == 0)
                db.release();
            mutex.release();
            ++reads;
        } else // if writer
        {
            db.acquire();
            result = theDatabase.write( workerID );
            db.release();
            ++writes;
        }; // end if writer
        ++tests;

        // NON-CRITICAL AREAD
        // Sleep a while...
        int numSeconds2sleep = randomInt( 3 ); // i.e. either 0, 1 or 2
        std::chrono::seconds randomSeconds( numSeconds2sleep );
        std::cout << "WORKER " << workerID
                  << ( reader ? " READER " : " WRITER ")
                  << " finished test " << tests
                  << ", result = " << result
                  << ", sleeping " << numSeconds2sleep
                  << " seconds " << std::endl;
        if ( 0 < numSeconds2sleep ) std::this_thread::sleep_for ( randomSeconds );

    } // repeat until time used is up

    std::cout << "WORKER " << workerID
              << " finished. Returning after " << tests << " tests, "
              << reads << " reads and "
              << writes << " writes."
              << std::endl;

} // end worker function

