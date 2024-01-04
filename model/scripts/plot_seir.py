import sys
import numpy as np
import matplotlib.pyplot as plt
from argparse import ArgumentParser


VARS = ['E', 'I', 'Npop', 'R_Fatal', 'R_Mild', 'R_Severe', 'Fatal', 'SevereH','S']


def parse_options():
    parser = ArgumentParser()

    parser.add_argument('-m', '--model-alias', dest='model_alias', action='store',
                        type=str, required=True,
                        help='model alias (concatenation of sweeped parameters)')

    parser.add_argument('-p', '--path', dest='path', action='store',
                        type=str, default='.',
                        help='path to model output')

    parser.add_argument('-a', '--add-parameters', dest='add_parameters', action='store_true',
                        help='whether to add sweeped parameters to the plot')

    parser.add_argument('-r', '--plot-recovered', dest='plot_recovered', action='store_true',
                        help='whether to plot recovered curve')

    options = parser.parse_args()

    return options


def plot(data, options):
    N = data['Npop'][0]
    dead = N*data['R_Fatal']
    hospitalized = N*(data['SevereH'] + data['Fatal'])
    recovered = N*(data['R_Mild'] + data['R_Severe'])
    infected = N*data['I']
    exposed = N*data['E']
    times = data['time']

    fig, ax = plt.subplots(nrows=1, ncols=1) 

    ax.plot(times, dead, label='Dead', color='black', linewidth=0.5)
    ax.plot(times, hospitalized, label='Hospitalized', color='lightblue', linewidth=0.5)
    if options.plot_recovered:
        ax.plot(times, recovered, label='Recovered', color='blue', linewidth=0.5)
    ax.plot(times, infected, label='Infected', color='red', linewidth=0.5)
    ax.plot(times, exposed, label='Exposed', color='brown', linewidth=0.5)

    ax.legend()
    ax.set_xlabel('Time [days]')
    ax.set_ylabel('Number of people')

    ax.set_xlim([0,times[-1]])

    if options.add_parameters:
        y_offset = -45
        plt.annotate('Sweeped parameters:', (0,0), (0, y_offset), xycoords='axes fraction', textcoords='offset points', va='top')
        params = get_params(options)
        for param in params:
            y_offset -= 13
            plt.annotate(param, (0,0), (5, y_offset), xycoords='axes fraction', textcoords='offset points', va='top')

    plt.close(fig) 
    fig.savefig('%s.png' % options.model_alias, bbox_inches='tight')

def get_params(options):
    params = options.model_alias.split('_')[1:]
    i = 0
    while i < len(params):
        if '=' not in params[i]:
            params[i+1] = '%s_%s' % (params[i], params[i+1])
            del params[i]
            i += 1
        i += 1        
    filtered = ['method', 'dqrel', 'dqmin', 'particles', 'runs']
    params = filter(lambda p: all(map(lambda s: s not in p, filtered)), params)
    return params

def read(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    values = list()

    for line in lines:
        vs = map(float, line.split('\t'))
        values.append(vs[1])

    return np.array(values)

def read_times(path):
    with open('%s/Npop.dat' % path, 'r') as f:
        lines = f.readlines()

    values = list()

    for line in lines:
        vs = map(float, line.split('\t'))
        values.append(vs[0])

    return np.array(values)


def read_all(path):
    data = dict()

    for var in VARS:
        values = read('%s/%s.dat' % (path,var))
        data[var] = values

    data['time'] = read_times(path)

    return data


if __name__ == '__main__':
    options = parse_options()
    data = read_all(options.path)
    plot(data, options)
