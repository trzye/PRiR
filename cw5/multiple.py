#!/usr/bin/python
# File: multiplication.py
# Authors:
#  VItalii Vanovschi -- base parallel program sum_primes.py),
#  R. Szmurlo        -- base program mult_for_students.py),
#  Michal Jereczek, Jagoda Łopatka   -- merging above programs
# Desc: This program demonstrates parallel computations with pp module
# It calculates multiplication of given matrix A and vector X
# Parallel Python Software: http://www.parallelpython.com

import sys
import time
import pp


def mnoz(dane, i):
    A = dane[0]
    X = dane[1]

    ncols = len(A[0])

    s = 0
    for c in range(0, ncols):
        s += A[i][c] * X[c][0]
        time.sleep(0.1)

    return s


def read(fname):
    f = open(fname, "r")
    nr = int(f.readline())
    nc = int(f.readline())

    A = [[0] * nc for x in xrange(nr)]
    r = 0
    c = 0
    for i in range(0, nr * nc):
        A[r][c] = float(f.readline())
        c += 1
        if c == nc:
            c = 0
            r += 1

    return A


fnameA = sys.argv[2] if len(sys.argv) > 2 else "A.dat"
fnameX = sys.argv[3] if len(sys.argv) > 3 else "XX.dat"

A = read(fnameA)
X = read(fnameX)

print """Usage: python multiplication.py [ncpus] [fnameA] [fnameX]
    [ncpus]  - the number of workers to run in parallel (default number of processors in the system),
    [fnameA] - matrix A file name (default A.dat),
    [ncpus]  - vector X file name (default XX.dat),
"""

# tuple of all parallel python servers to connect with
# ppservers = ()
# ppservers = ("194.29.146.152:6655",)

if len(sys.argv) > 1:
    ncpus = int(sys.argv[1])
    # Creates jobserver with ncpus workers
    job_server = pp.Server(ncpus, ppservers=ppservers)
else:
    # Creates jobserver with automatically detected number of workers
    job_server = pp.Server(ppservers=ppservers)

print "Starting pp with", job_server.get_ncpus(), "workers"

# # [START] UNCOMMENT IF YOU WANT TO CHECK IT OUT WITH ONE JOB
# # Submit a job of calulating sum_primes(100) for execution.
# # sum_primes - the function
# # ([A, X], 0) - tuple with arguments for sum_primes
# # (mnoz,) - tuple with functions on which function sum_primes depends
# # ("time",) - tuple with module names which must be imported before sum_primes execution
# # Execution starts as soon as one of the workers will become available
# job1 = job_server.submit(mnoz, ([A, X], 0), (), ("time",))
#
# # Retrieves the result calculated by job1
# # The value of job1() is the same as mnoz(0)
# # If the job has not been finished yet, execution will wait here until result is available
# result = job1()
#
# print "Calculation for A[0] is", result
# # [END] UNCOMMENT IF YOU WANT TO CHECK IT OUT WITH ONE WORKER

start_time = time.time()

# The following submits nrows jobs and then retrieves the results
nrows = len(A)
y = [None] * len(A)

jobs = [(i, job_server.submit(mnoz, ([A, X], i), (), ("time",))) for i in xrange(nrows)]
for i, job in jobs:
    y[i] = job()
    # print 'job for', i, 'done'

# print "Wynik y= ", y

print "Time elapsed: ", time.time() - start_time, "s"
job_server.print_stats()

# Parallel Python Software: http://www.parallelpython.com