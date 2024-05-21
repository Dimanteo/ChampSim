from tracelist import get_traces, get_trace_path
import subprocess

def main():
    url = 'https://dpc3.compas.cs.stonybrook.edu/champsim-traces/speccpu/'
    for trace in get_traces():
        trace_url = url + trace
        dst_path = get_trace_path(trace)
        subprocess.run(['curl', trace_url, '-s', '-o', dst_path])

if __name__ == '__main__':
    main()
