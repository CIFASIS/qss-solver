import sys
import scipy
import scipy.stats
import pandas as pd
import numpy as np
import matplotlib as mpl
import random
# To make matplotlib not to use a display.
mpl.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D

NX = 16
NY = 10

FILENAME_TEMPLATE = '%s/%s_%s-%%d.txt'

ALPHA = 0.01
C = {
    0.1   : 1.22,
    0.05  : 1.36,
    0.025 : 1.48,
    0.01  : 1.63,
    0.005 : 1.73,
    0.001 : 1.95
}



def rmse(a, b):
    return np.sqrt(np.mean((a-b)**2))

def compute_rmses(values):
    rmses = np.zeros((NX,NY))
    values_ref, values_sample = values

    for i in xrange(NX):
        for j in xrange(NY):
            v1 = values_sample[i][j]
            v2 = values_ref[i][j]

            n = min(len(v1), len(v2))
            
            rmses[i,j] = rmse(np.array(v1[:n]), np.array(v2[:n]))

    return rmses

def ks_test(vals1, vals2):
    D, pval = scipy.stats.ks_2samp(vals1, vals2)

    n, m = len(vals1), len(vals2)
    thresh = C[ALPHA] * np.sqrt(float(n+m) / (n*m))

    return pval, D, thresh


def plot_grid(values, filename):
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    xs = range(values.shape[1])
    ys = range(values.shape[0])
    X, Y = np.meshgrid(xs, ys)

    #surf = ax.plot_surface(X, Y, values, rstride=1, cstride=1, linewidth=0.5, antialiased=True, cmap=cm.coolwarm)

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('RMSE(x,y)')

    ax.xaxis.set_ticklabels([])
    ax.yaxis.set_ticklabels([])
    #ax.zaxis.set_ticklabels([])

    #fig.colorbar(surf, shrink=0.5, aspect=5)


    bottom = np.zeros_like(values.ravel())
    width = np.ones_like(bottom)
    depth = np.ones_like(bottom)
    z = np.zeros_like(bottom)

    ax.view_init(elev=10, azim=30)
    ax.bar3d(X.ravel(), Y.ravel(), z, width, depth, values.ravel(), shade=True)

    plt.savefig(filename, bbox_inches='tight')

def read_method(method, path):
    method_phi = FILENAME_TEMPLATE % (path, 'phi', method)
    method_rho = FILENAME_TEMPLATE % (path, 'rho', method)
    n = 0

    phi = np.zeros((NX,NY))
    rho = np.zeros((NX,NY))

    values_phi = [[[] for _ in xrange(NY)] for _ in xrange(NX)]
    values_rho = [[[] for _ in xrange(NY)] for _ in xrange(NX)]

    while True:
        try:
            data = pd.read_csv(method_phi % n, sep=',', header=None).values
            phi += data
            for i in xrange(NX):
                for j in xrange(NY):
                    v1 = data[i,j]
                    values_phi[i][j].append(v1)

            data = pd.read_csv(method_rho % n, sep=',', header=None).values
            rho += data
            for i in xrange(NX):
                for j in xrange(NY):
                    v1 = data[i,j]
                    values_rho[i][j].append(v1)

            n += 1
        except:
            break

    phi /= n
    rho /= n

    return n, phi, rho, values_phi, values_rho


def compute_error(method, path_method, path_matlab):
    n_matlab, phi_matlab, rho_matlab, values_phi_matlab, values_rho_matlab = read_method('matlab', path_matlab)
    n_method, phi_method, rho_method, values_phi_method, values_rho_method = read_method(method, path_method)

    phi_errs = abs(phi_method - phi_matlab)
    rho_errs = abs(rho_method - rho_matlab)

    print 'Number of reference experiments: %d' % n_matlab
    print 'Number of %s experiments: %d' % (method, n_method)
    print

    print 'phi analysis:'
    print 'Reference values:' 
    print '  min: %g - max: %g - avg: %g' % (phi_matlab.min(), phi_matlab.max(), phi_matlab.mean())
    print 'L2 norm: %g' % np.linalg.norm(phi_errs, 2)
    idx = np.unravel_index(phi_errs.argmax(), phi_errs.shape)
    print 'Max error: %g at cell %s' % (phi_errs.max(), idx)
    print 'Avg error: %g' % phi_errs.mean()
    percs = np.percentile(phi_errs, [15,35,55,75,95])
    print '(15,35,55,75,95) percentiles: %s' % percs 
    print

    rmses_phi = compute_rmses((values_phi_matlab, values_phi_method))
    plot_grid(rmses_phi, 'rmses_phi-%s.png' % method)

    print 'RMSEs analysis (phi):'
    print 'L2 norm: %g' % np.linalg.norm(rmses_phi, 2)
    idx = np.unravel_index(rmses_phi.argmax(), rmses_phi.shape)
    print 'Max error: %g at cell %s' % (rmses_phi.max(), idx)
    print 'Avg error: %g' % rmses_phi.mean()
    percs = np.percentile(rmses_phi, [15,35,55,75,95])
    print '(15,35,55,75,95) percentiles: %s' % percs 
    print

    print 'rho analysis:'
    print 'Reference values:' 
    print '  min: %g - max: %g - avg: %g' % (rho_matlab.min(), rho_matlab.max(), rho_matlab.mean())
    print 'L2 norm: %g' % np.linalg.norm(rho_errs, 2)
    idx = np.unravel_index(rho_errs.argmax(), rho_errs.shape)
    print 'Max error: %g at cell %s' % (rho_errs.max(), idx)
    print 'Avg error: %g' % rho_errs.mean()
    percs = np.percentile(rho_errs, [15,35,55,75,95])
    print '(15,35,55,75,95) percentiles: %s' % percs 

    plot_grid(phi_errs, 'phi_errs-%s.png' % method)

    return (values_phi_matlab, values_phi_method), (values_rho_matlab, values_rho_method)
    

if __name__ == '__main__':
    method = sys.argv[1]
    path_method = sys.argv[2]
    path_matlab = sys.argv[3]
    
    values_phi, values_rho = compute_error(method, path_method, path_matlab)

    rmses_phi = compute_rmses(values_phi)
    plot_grid(rmses_phi, 'rmses_phi-%s.png' % method)
