import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt

import collections
import numpy as np
import pandas as pd

PATH_RETQSS = '/home/lucio/data/repo/retqss/build/tmp_md/3d/p/retqss'
PATH_ABORIA = '/home/lucio/data/repo/retqss/build/tmp_md/3d/p/aboria' 
STEPS = 1000


def read_trajectory(p, method=None):
    method = method or 'retqss'
    method = method.lower()

    values = list()

    if method == 'retqss':
        path = PATH_RETQSS
    else:
        path = PATH_ABORIA

    for i in xrange(STEPS):
        filename = '%s/%d.dat' % (path,i)
        found = False
        with open(filename, 'r') as f:
            lines = f.readlines()

        pid = 0
        for j in xrange(0, len(lines), 2):
            row = map(float, lines[j].strip().split(' '))

            if len(row) > 3 and row[0] == p:
                values.append(row[1:])
                found = True
                break
            elif len(row) == 3 and pid == p:
                values.append(row)
                found = True
                break

            pid += 1        
    
        #if not found:
        #    values.append(values[-1])    

    values = np.array(values)

    return values


def plot_trajectory(p):
    retqss_values = read_trajectory(p, 'retqss')
    aboria_values = read_trajectory(p, 'aboria')
    
    n = min(len(retqss_values), len(aboria_values))
    time = range(n)
    retqss_values = retqss_values[:n]
    aboria_values = aboria_values[:n]

    fig, ax = plt.subplots(3, figsize=(30, 7), sharex=True)

    err = ax[0].plot(time, retqss_values[:,0], label='x (retQSS)', marker='o', linewidth='1', markersize=2, color='blue')
    err = ax[0].plot(time, aboria_values[:,0], label='x (Aboria)', marker='x', linewidth='1', markersize=1, color='red')

    err = ax[1].plot(time, retqss_values[:,1], label='y (retQSS)', marker='o', linewidth='1', markersize=2, color='blue')
    err = ax[1].plot(time, aboria_values[:,1], label='y (Aboria)', marker='x', linewidth='1', markersize=1, color='red')

    err = ax[2].plot(time, retqss_values[:,2], label='z (retQSS)', marker='o', linewidth='1', markersize=2, color='blue')
    err = ax[2].plot(time, aboria_values[:,2], label='z (Aboria)', marker='x', linewidth='1', markersize=1, color='red')

    ax[0].set_xlabel(u'Time')
    #ax1.xaxis.set_major_locator(mpl.ticker.MaxNLocator(8, integer=True))
    #ax1.xaxis.set_major_formatter(FormatStrFormatter('%d'))
    ax[0].set_ylabel('x')
    ax[1].set_ylabel('y')
    ax[2].set_ylabel('z')

    ax[0].legend()

    plt.savefig('trajectory_%d.png' % p, bbox_inches='tight')    
    plt.close()

if __name__ == '__main__':
    plot_trajectory(6)
    plot_trajectory(15)
    plot_trajectory(26)
    plot_trajectory(90)
    plot_trajectory(42)
      
