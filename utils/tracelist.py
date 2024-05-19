import sys
import os

def get_traces(tracelist='tracelist.txt'):
    if len(sys.argv) < 2:
        print("Enter storage folder")
        exit(-1)
    if len(sys.argv) == 3:
        tracelist = sys.argv[2]
    with open(tracelist, 'r') as file:
        return file.read().split()

def get_trace_path(trace):
    return os.path.join(sys.argv[1], trace)
