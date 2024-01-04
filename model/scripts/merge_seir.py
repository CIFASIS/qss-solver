import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from argparse import ArgumentParser
import glob
import os


VARS = ['E', 'I', 'Npop', 'R_Fatal', 'R_Mild', 'R_Severe', 'Fatal', 'SevereH','S']


def parse_options():
    parser = ArgumentParser()

    parser.add_argument('-x', '--xlim', dest='xlim', action='store',
                        type=float, default=120,
                        help='x-axis limit')

    parser.add_argument('-y', '--ylim', dest='ylim', action='store',
                        type=float, default=3000,
                        help='y-axis limit')

    parser.add_argument('-p', '--path', dest='path', action='store',
                        type=str, default='.',
                        help='path to SEIR output')

    options = parser.parse_args()

    return options

def plot(data, mode, limits=None):
    fig, ax = plt.subplots(nrows=1, ncols=1) 

    dead_colors = [cm.Reds(x) for x in np.linspace(0.5,50,len(data))]
    exposed_colors = [cm.Greys(x) for x in np.linspace(0.5,50,len(data))]
    infected_colors = [cm.Oranges(x) for x in np.linspace(0.5,50,len(data))]
    hospitalized_colors = [cm.Purples(x) for x in np.linspace(0.5,50,len(data))]

    widths = np.linspace(0.1, 1., len(data))

    i = 0
    n = len(data)
    for instance, instance_data in data.iteritems():
        N = instance_data['Npop'][0]
        dead = N*instance_data['R_Fatal']
        hospitalized = N*(instance_data['SevereH'] + instance_data['Fatal'])
        recovered = N*(instance_data['R_Mild'] + instance_data['R_Severe'])
        infected = N*instance_data['I']
        exposed = N*instance_data['E']
        times = instance_data['time']

        params = get_params(instance)

        if mode in ['all', 'dead']:
            label = 'Dead' if i==n-1 else None
            ax.plot(times, dead, label=label, color=dead_colors[i], linewidth=widths[i])
        if mode in ['all', 'hospitalized']:
            label = 'Hospitalized' if i==n-1 else None
            ax.plot(times, hospitalized, label=label, color=hospitalized_colors[i], linewidth=widths[i])
        if mode in ['all', 'infected']:
            label = 'Infected' if i==n-1 else None
            ax.plot(times, infected, label=label, color=infected_colors[i], linewidth=widths[i])
        if mode in ['all', 'exposed']:
            label = 'Exposed' if i==n-1 else None
            ax.plot(times, exposed, label=label, color=exposed_colors[i], linewidth=widths[i])
        if mode in ['recovered']:
            label = 'Recovered' if i==n-1 else None
            ax.plot(times, recovered, label=label, color=exposed_colors[i], linewidth=widths[i])

        i += 1

    ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.1), ncol=3)
    ax.set_xlabel('Time [days]')
    ax.set_ylabel('Number of people')

    if limits is not None:    
        xlim, ylim = limits
        ax.set_xlim([0,xlim])
        ax.set_ylim([0,ylim])
        fig.savefig('curves_%s_xlim=%g_ylim=%g.png' % (mode,xlim,ylim), bbox_inches='tight')
    else:
        fig.savefig('curves_%s.png' % mode, bbox_inches='tight')

    plt.close(fig) 



def get_params(instance):
    not_params = ['seir','method','particles','dqmin','dqrel','runs']

    def is_param(field):
        return not any(map(lambda x: x in field, not_params))

    fields = instance.split('_')
    params = filter(is_param, fields)

    return '_'.join(params)

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

    for val in glob.glob('%s/*' % path):
        try:
            instance = os.path.split(val)[-1]
            data_path = '%s/1' % val
            instance_data = dict()

            for var in VARS:
                values = read('%s/%s.dat' % (data_path,var))
                instance_data[var] = values

            instance_data['time'] = read_times(data_path)

            data[instance] = instance_data
        except IOError:
            continue

    return data


if __name__ == '__main__':
    options = parse_options()

    data = read_all(options.path)

    plot(data, mode='all')
    plot(data, mode='infected')
    plot(data, mode='infected', limits=(options.xlim, options.ylim))
    plot(data, mode='exposed')
    plot(data, mode='dead')
    plot(data, mode='hospitalized')
    plot(data, mode='recovered')
