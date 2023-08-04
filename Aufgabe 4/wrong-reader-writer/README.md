## INTRODUCTION AND WARNING

This package contains a program which is intentionally WRONG!!

This illustrates an INCORRECT (but multithreaded) "solution" to 
the Reader/Writer Problem.  No semaphores are used, so the writers 
create problems which the readers detect.

This is part of the Betriebssystem Praktikum at the 
Darmstadt University of Applied Sciences.
 
## CONTENTS

You should have received the following files:

  1.  `Makefile`			      <-- A Makefile, with comments in German (mostly).
  2.  `database.cpp`
  3.  `database.h`              <-- Source code for the database singleton class
  4.  `reader-writer-threads.cpp`
  5.  `reader-writer-threads.h` <-- Souce code for the reader & writer agents (threads)
  6.  `reader-writer.cpp`       <-- Souce code for "main" (the program itself).
  7.  `stopwatch.h`             <-- Source code for measuring elapsed time. 
                                Note that there is NO `stopwatch.cpp` file!
  8.  `README.md`               <--  This file.
  9.  `inputFile.txt`           <-- Test Input (please make a backup copy!).


## INSTALLATION and EXECUTION

### TL;DR

The program (named `reader-writer`) can be built by using the standard linux/unix utility named `make`. 

In other words, enter `make` on the command line to build the program (or to be sure you have a clean build, enter `make clean` first, and when that is finished, enter `make`).

If you then run the program with no arguments, 
it will tell you what arguments it expects.


### Longer Explanation

Consider the following transcript (lines entered by the user start with $,
lines that do not start with $ are output from the shell).

(If you're unfamiliar with the commands `time, head` or `tail`, 
look them up with `man time`, `man head` and `man tail`).


```console

$ make
clang++ -c -g -std=c++11 -Wall -MMD -fmessage-length=0 -o database.o database.cpp
clang++ -c -g -std=c++11 -Wall -MMD -fmessage-length=0 -o reader-writer-threads.o reader-writer-threads.cpp
clang++ -c -g -std=c++11 -Wall -MMD -fmessage-length=0 -o reader-writer.o reader-writer.cpp
clang++ -o reader-writer database.o reader-writer-threads.o  reader-writer.o  -lpthread
touch outputFile.txt

$ ./reader-writer
Usage: ./reader-writer numSeconds numThreads percentReaders
        Where 0 < numSeconds < 86400 (1 day) and 0 < numThreads < 1024 and 0.0 <= percentReaders <= 100.0

$ time ./reader-writer 1 1 50
In ./reader-writer, seconds = 1, threads = 1, % readers = 50, ready to go...
Finished creating all threads...
Worker 0 starting...
WORKER 0 READER  finished test 1, result = 1, sleeping 1 seconds
WORKER 0 finished. Returning after 1 tests, 1 reads and 0 writes.
Database Statistics
--------------------
 Readers: number of read operations  = 1
 Writers: number of write operations = 0
WARNING: Since time was only 1 seconds, the statistics above mean NOTHING.
Simulation finished! Database is OK!
./reader-writer 1 1 50  0.05s user 0.01s system 5% cpu 1.079 total

$ time ./reader-writer 120 100 80 >outputLog.txt  2>errorLog.txt
./reader-writer 120 100 80 > outputLog.txt 2> errorLog.txt  655.11s user 558.37s system 963% cpu 2:05.98 total

$ head outputLog.txt
In ./reader-writer, seconds = 120, threads = 100, % readers = 80, ready to go...
Worker 1 starting...
Worker Worker 4 starting...
Worker 05 starting...
Worker 7 starting...Worker 6 starting...
Worker 9 starting...Worker 10 starting...
Worker 3 starting...
Worker Worker 11 starting...Worker 14 starting...

$ tail outputLog.txt 
WORKER 7 finished. Returning after 42 tests, 36 reads and 6 writes.
WORKER 87 finished. Returning after 33 tests, 26 reads and 7 writes.
WORKER 57 finished. Returning after 41 tests, 34 reads and 7 writes.
WORKER 96 finished. Returning after 25 tests, 15 reads and 10 writes.
WORKER 41 finished. Returning after 34 tests, 25 reads and 9 writes.
Database Statistics
--------------------
 Readers: number of read operations  = 3018
 Writers: number of write operations = 733
Simulation finished! Database is NOT OK!!!

```

Now please try the commands yourself - you should get similar, but _not identical_ results. 

Also experiment with different values for `numSeconds`, `numThreads` and `percentReaders`.


## CONCLUSION

The program has been tested for with the clang compiler, version 9.0.0, 
and with g++, version 9.2.0 on a 64 bit Linux.  

Update: In 2021 this program has been retested with clang and g++, both at version 11.1.0 (coincidentally (?)).

Updated Update: In 2022 and in 2023, this program has been retested with clang and g++ under macOS.  It still works (as well as it ever did).

Please do not hesitate to contact me if you have questions.

(Prof. Dr. Ronald Moore - FBI - H-DA.DE - <ronald.moore@h-da.de>)
