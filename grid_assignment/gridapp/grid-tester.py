#!/usr/bin/python3

# Created by Ryan Stewart 2020-03-17

# Please feel free to modify this script and improve it. The parsing definitely could use love
# Also feel free to make this csv better

import sys
import threading
import subprocess
import csv
import json
from time import sleep
from functools import reduce

lock = threading.Semaphore()
counter = 0

def write_data(out, name):
    with open("{}-{}.txt".format(out, name), "w") as f:
        for k in results[name].keys():
            if name == "threads":
                f.write(",THREAD #,{} SECONDS,DATA INTEGRITY\n".format(k.upper()))
            else:
                f.write(",GRID SIZE,{} SECONDS,DATA INTEGRITY\n".format(k.upper()))
            for i in range(len(results[name][k])):
                for j in range(tests):
                    temp = results[name][k][i][j]
                    good = "MAINTAINED"
                    if not temp[1]:
                        good = "VIOLATION"
                    if "threads":
                        f.write("TEST {},{},{},{}\n".format(j+1, i+1, temp[0], good))
                    else:
                        f.write("TEST {},{},{},{}\n".format(j+1, i+2, temp[0], good))
            f.write(",,,\n")

# Pretty print thread count for threads
def pretty_print(last, char):
    llc = len(str(last))
    lc = len(str(char))
    print("\b"*llc + str(c) + " "*(llc - lc) + "\b"*(llc - lc), end="")
    sys.stdout.flush()

def parse(result):
    result = result.decode().split("\n")
    secs = 0
    data = True
    for i in result:
        if i.strip().lower().startswith("secs"):
            secs = int(i.strip().split()[2])
        elif "integrity" in i.strip().lower():
            if "violation" in i.strip().lower():
                data = False
    return secs, data

def grid_size(name, gran, size, test_num, dictionary):
    global lock
    global counter
    try:
        secs, data = parse(subprocess.check_output([name, str(size), "10", "-"+gran]))
    except FileNotFoundError:
        print("Failed to run command")
        sys.exit(1)
    dictionary["size"][gran][size-2][test_num] = (secs, data)
    # Count down to say thread is done
    lock.acquire()
    counter -= 1
    lock.release()

def threads(name, gran, thread_num, test_num, dictionary):
    global lock
    global counter
    #secs, data = parse(subprocess.check_output([name, "10", str(thread_num), "-"+gran]))
    try:
        secs, data = parse(subprocess.check_output([name, str(thread_num), "10", "-"+gran]))
    except FileNotFoundError:
        print("Failed to run command")
        sys.exit(1)
    dictionary["threads"][gran][thread_num-1][test_num] = (secs, data)
    # Count down to say thread is done
    lock.acquire()
    counter -= 1
    lock.release()

# Start the threads for the tests
def init_tests(test_type, tests, start, end):
    global counter
    for i in range(start, end+1):
        for j in range(tests):
            for k in results[test_type]:
                t = None
                if test_type == "threads":
                    t = threading.Thread(target=threads, args=(name, k, i, j, results))
                else:
                    t = threading.Thread(target=grid_size, args=(name, k, i, j, results))
                # Make sure to acquire lock to count threads running
                lock.acquire()
                counter += 1
                lock.release()
                t.start()

# Get program name and results file name
name = "./gridapp" # Things will go horribly if the name is wrong
out = "results"
if len(sys.argv) == 1:
    print("Using program {}".format(name))
if len(sys.argv) > 1:
    if not sys.argv[1].startswith("./") and not sys.argv[1].startswith("/"):
        name = "./"+sys.argv[1]
    else:
        name = sys.argv[1]
if len(sys.argv) > 2:
    out = sys.argv[2]

tests = 5
print("Initializing data structure")

# Define dict to hold all values
results = {"size": {"none": [], "row": [], "grid": [], "cell": []},
           "threads": {"none": [], "row": [], "grid": [], "cell": []}}

# Initialize grids so each thread has a slot that can be accessed (No need for locks)
for k in results["threads"]:
    results["threads"][k] = [[0 for i in range(tests)] for i in range(10)]

for k in results["size"]:
    results["size"][k] = [[0 for i in range(tests)] for i in range(2,11)]

print("Starting all threads")
init_tests("threads", tests, 1, 10)
init_tests("size", tests, 2, 10)

# Wait until all threads before moving on
print("Waiting on threads to finish")
lock.acquire()
last_count = counter
print("Threads remaining: {}".format(counter), end="")
lock.release()
sys.stdout.flush()
while True:
    c = 0
    lock.acquire()
    c = counter
    if counter == 0:
        lock.release()
        break
    lock.release()
    pretty_print(last_count, c)
    last_count = c
    sleep(0.1)
pretty_print(last_count, 0)
print("Threads done! Writing data")

print("Writing raw data as json for potential parsing")
with open("raw-data.txt", "w") as f:
    f.write(json.dumps(results, indent=2, sort_keys=True))

print("Writing out thread data")
[write_data(out, k) for k in results.keys()]
