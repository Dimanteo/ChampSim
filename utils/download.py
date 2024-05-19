import sys
import subprocess
import os

def main():
    if len(sys.argv) < 2:
        print("Enter destination folder")
        exit(-1)
    tracelist = 'tracelist.txt'
    if len(sys.argv) == 3:
        tracelist = sys.argv[2]
    with open(tracelist, 'r') as file:
        traces = file.read().split()
    url = 'https://dpc3.compas.cs.stonybrook.edu/champsim-traces/speccpu/'
    for trace in traces:
        trace_url = url + trace
        dst_path = os.path.join(sys.argv[1], trace)
        subprocess.run(['curl', trace_url, '-s', '-o', dst_path])

if __name__ == '__main__':
    main()
