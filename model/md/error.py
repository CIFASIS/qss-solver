import numpy as np
import pandas as pd


STEPS = 1000

PATH_METHOD = '~/data/repo/retqss/build/tmp_md/3d/p/1e-5/retqss'
PATH_BASELINE = '~/data/repo/retqss/build/tmp_md/3d/p/1e-5/baseline'


def rmse(a, b):
    return np.sqrt(np.mean((a-b)**2))

def read_method_step(i, method=None):
    method = method or 'baseline'
    path = PATH_BASELINE if method == 'baseline' else PATH_METHOD
    filename = '%s/%d.dat' % (path, i)
    return pd.read_csv(filename, sep=' ', header=None)

def positions(df):
    positions = list()
    for i in xrange(0, len(df), 2):
        pos = np.array(df.iloc[i])
        positions.append(pos)
    return np.array(positions)

def rmsd(pos1, pos2):
    n = len(pos1)
    return np.sqrt(np.sum((pos1-pos2)**2)/n)


def error_on_step(i):
    df_method = read_method_step(i, 'method')
    df_baseline = read_method_step(i)
    pos_method = positions(df_method)
    pos_baseline = positions(df_baseline)
    return rmsd(pos_method, pos_baseline)



if __name__ == '__main__':
    last_err = error_on_step(STEPS-1)
    print 'Error on last step: %.12e' % last_err

    max_err = -1
    for i in xrange(STEPS):
        err = error_on_step(i)
        if err > max_err:
            max_err = err
            max_step = i

    print 'Max error: %.12e (step %d)' % (max_err, max_step)
