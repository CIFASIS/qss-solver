import sys
import numpy as np
import matplotlib.pyplot as plt
import networkx as nx
import collections


OUTPUT_FILENAME = 'indirect_infection_output.dat'

INFECTED = 0
NOT_INFECTED = 1
RECOVERED = 2
DEAD = 3

DELTA_T = 0.2
TARGET_PERCENT = 25


def plot(data):
    times, values, percents = data

    fig, ax = plt.subplots(nrows=1, ncols=1) 

    infected = map(lambda p: p[INFECTED], percents)
    not_infected = map(lambda p: p[NOT_INFECTED], percents)
    recovered = map(lambda p: p[RECOVERED], percents)

    for idx, p in enumerate(infected):
        if p >= TARGET_PERCENT:
            step = times[idx]
            time = step*DELTA_T
            print '%g%% infection reached at t = %g (timestep = %g)' % (TARGET_PERCENT, time, step) 
            break

    ax.plot(times, infected, label='Infected', color='red', linewidth=0.5)
    ax.plot(times, not_infected, label='Not infected', color='blue', linewidth=0.5)
    ax.plot(times, recovered, label='Recovered', color='green', linewidth=0.5)

    ax.legend()
    ax.set_xlabel('Time')
    ax.set_ylabel('Percentage')

    ax.set_xlim([0,times[-1]])

    plt.close(fig) 
    fig.savefig('curves.png')


def read(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    values = list()
    percents = list()

    for line in lines[0:len(lines):3]:
        vs = map(int, line.split(' '))
        N = sum(vs)
        ps = map(lambda v: 100*(float(v)/N), vs)

        percents.append(ps)
        values.append(vs)

    return range(1, len(values)+1), values, percents

def save_degree_histogram(G):
    filename = '%s_histogram.png' % G.name

    plt.close()
    degree_sequence = sorted([d for d in G.degree().itervalues()], reverse=True)
    degreeCount = collections.Counter(degree_sequence)
    deg, cnt = zip(*degreeCount.items())

    fig, ax = plt.subplots()
    plt.bar(deg, cnt, width=0.80, color='b')

    plt.title("Degree Histogram - %s" % G.name)
    plt.ylabel("Count")
    plt.xlabel("Degree")
    ax.set_xticks([d + 0.4 for d in deg])
    ax.set_xticklabels(deg)
    ax.tick_params(axis='both', which='major', labelsize=8)
    plt.xticks(rotation=90)

    plt.savefig(filename, bbox_inches='tight')

def save_graph_stats(G):
    info = nx.info(G) + '\n'
    filename = '%s.stats' % G.name
    with open(filename, 'w') as _f:
        _f.write(info)


def process_graph(graph_filename):
    idx = graph_filename[::-1].find('.')
    basename = graph_filename[:-idx-1]

    G = nx.read_adjlist(graph_filename)
    G.name = basename 
   
    plt.close()
    nx.draw_networkx(G, node_size=15, width=0.3, with_labels=False)
    plt.axis('off')

    img_filename = '%s.png' %  basename
    plt.savefig(img_filename, bbox_inches='tight')

    save_graph_stats(G)
    save_degree_histogram(G)


if __name__ == '__main__':
    filename = sys.argv[1]
    data = read(filename)
    plot(data)

    graph1 = sys.argv[2]
    process_graph(graph1)

    graph2 = sys.argv[3]
    process_graph(graph2)
