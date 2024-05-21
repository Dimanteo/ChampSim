from tracelist import get_traces, get_trace_path
import subprocess

def main():
    warmup = 5000000
    simulation = 20000000
    for trace in get_traces():
        subprocess.Popen([
            '../bin/champsim',
            '--warmup-instructions', str(warmup),
            '--simulation-instructions', str(simulation),
            '--json', trace + '_stats.json',
            get_trace_path(trace)
        ])

if __name__ == '__main__':
    main()
