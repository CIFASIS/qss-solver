import sys
import scipy
import scipy.stats
import pandas as pd
import numpy as np
import matplotlib as mpl
import random
# To make matplotlib not to use a display.
#mpl.use('Agg')
import pylab
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D

NX = 16
NY = 10

FILENAME_TEMPLATE = '%s/%s_%s-%%d.txt'


def rmse(a, b):
    return np.sqrt(np.mean((a-b)**2))

def compute_rmses(values_ref, values_sample):
    rmses = np.zeros((NX,NY))

    for i in xrange(NX):
        for j in xrange(NY):
            v1 = values_sample[i][j]
            v2 = values_ref[i][j]
            rmses[i,j] = rmse(np.array(v1), np.array(v2))

    return rmses

def compute_maes(values_ref, values_sample):
    maes = np.zeros((NX,NY))

    for i in xrange(NX):
        for j in xrange(NY):
            v1 = values_sample[i][j]
            v2 = values_ref[i][j]
            maes[i,j] = abs(np.array(v1) - np.array(v2)).mean()

    return maes

def compute_error_ratios(values_ref, values_sample):
    rmses = np.zeros((NX,NY))

    for i in xrange(NX):
        for j in xrange(NY):
            v1 = values_sample[i][j]
            v2 = values_ref[i][j]
            if v1[0] == 0:
                rmses[i,j] = 0
            else:
                rmses[i,j] = abs((np.array(v1) - np.array(v2)) / np.array(v2)).mean()

    return rmses



def plot_grid(values, filename):
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    xs = range(values.shape[0])
    ys = range(values.shape[1])
    X, Y = np.meshgrid(xs, ys, indexing='ij')

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


def plot_surface(values_ref, values_method, filename, name='phi'):
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    xs = range(values_ref.shape[0])
    ys = range(values_ref.shape[1])
    X, Y = np.meshgrid(xs, ys, indexing='ij')

    surf1 = ax.plot_surface(X, Y, 0.05+values_ref, alpha=1, rstride=1, cstride=1, linewidth=0.5, antialiased=True, cmap=cm.autumn, label='MATLAB')
    surf2 = ax.plot_surface(X, Y, values_method, alpha=1, rstride=1, cstride=1, linewidth=0.5, antialiased=True, cmap=cm.autumn, label='PIC')

    ax.text(18, 4, 0.05, 'MATLAB', color='red')
    ax.text(18, 4, 0, 'retQSS', color='blue')

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel(name)

    ax.view_init(elev=10, azim=30)
    ax.mouse_init()
    #ax.xaxis.set_ticklabels([])
    #ax.yaxis.set_ticklabels([])
    ax.zaxis.set_ticklabels([])

    #fig.colorbar(surf1, shrink=0.5, aspect=5)
    fig.colorbar(surf2, shrink=0.5, aspect=5)

    plt.draw()
    pylab.show()
    #plt.savefig(filename, bbox_inches='tight')

    print 'Reference values:' 
    print '  min: %g - max: %g - avg: %g' % (abs(values_ref).min(), abs(values_ref).max(), abs(values_ref).mean())
    vals = abs(values_ref - values_method)
    idx = np.unravel_index(vals.argmax(), vals.shape)
    print 'Max. distance between %s surfaces: %g at cell %s' % (name, vals.max(), idx)
    print 'Avg. distance between %s surfaces: %g' % (name, vals.mean())
    percs = np.percentile(vals, [15,35,55,75,95])
    print '(15,35,55,75,95) percentiles: %s' % percs 
    print



def read_method(method, path, N):
    method_phi = FILENAME_TEMPLATE % (path, 'phi', method)
    method_rho = FILENAME_TEMPLATE % (path, 'rho', method)
    n = 0

    phi = np.zeros((NX,NY))
    rho = np.zeros((NX,NY))

    values_phi = [[[] for _ in xrange(NY)] for _ in xrange(NX)]
    values_rho = [[[] for _ in xrange(NY)] for _ in xrange(NX)]

    for n in xrange(N):
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

    phi /= N
    rho /= N

    return phi, rho, values_phi, values_rho

def compute_error(method, path, N):
    phi_matlab, rho_matlab, values_phi_matlab, values_rho_matlab = read_method('matlab', path, N)
    phi_method, rho_method, values_phi_method, values_rho_method = read_method(method, path, N)

    rmses_phi = compute_rmses(values_phi_matlab, values_phi_method)
    maes_phi = compute_maes(values_phi_matlab, values_phi_method)
    err_ratios_phi = compute_error_ratios(values_phi_matlab, values_phi_method)
    err_ratios_rho = compute_error_ratios(values_rho_matlab, values_rho_method)

    #plot_grid(rmses_phi, 'rmses_phi-%s.png' % method)
    #plot_grid(maes_phi, 'maes_phi-%s.png' % method)

    plot_surface(phi_matlab, phi_method, 'phi_surface-%s.png' % method)

    print 'Reference values:' 
    print '  min: %g - max: %g - avg: %g' % (phi_matlab.min(), phi_matlab.max(), phi_matlab.mean())
    print

    print 'Error ratios (phi):'
    idx = np.unravel_index(err_ratios_phi.argmax(), err_ratios_phi.shape)
    print 'Max error: %g at cell %s' % (err_ratios_phi.max(), idx)
    print 'Avg error: %g' % err_ratios_phi.mean()
    percs = np.percentile(err_ratios_phi, [15,35,55,75,95])
    print '(15,35,55,75,95) percentiles: %s' % percs 
    print

    print 'Error ratios (rho):'
    idx = np.unravel_index(err_ratios_rho.argmax(), err_ratios_rho.shape)
    print 'Max error: %g at cell %s' % (err_ratios_rho.max(), idx)
    print 'Avg error: %g' % err_ratios_rho.mean()
    percs = np.percentile(err_ratios_rho, [15,35,55,75,95])
    print '(15,35,55,75,95) percentiles: %s' % percs 
    print

    #print err_ratios_rho

    #print 'MAEs analysis (phi):'
    #print 'L2 norm: %g' % np.linalg.norm(maes_phi, 2)
    #idx = np.unravel_index(maes_phi.argmax(), maes_phi.shape)
    #print 'Max error: %g at cell %s' % (maes_phi.max(), idx)
    #print 'Avg error: %g' % maes_phi.mean()
    #percs = np.percentile(maes_phi, [15,35,55,75,95])
    #print '(15,35,55,75,95) percentiles: %s' % percs 
    #print

    #print 'RMSEs analysis (phi):'
    #print 'L2 norm: %g' % np.linalg.norm(rmses_phi, 2)
    #idx = np.unravel_index(rmses_phi.argmax(), rmses_phi.shape)
    #print 'Max error: %g at cell %s' % (rmses_phi.max(), idx)
    #print 'Avg error: %g' % rmses_phi.mean()
    #percs = np.percentile(rmses_phi, [15,35,55,75,95])
    #print '(15,35,55,75,95) percentiles: %s' % percs 
    #print

    return (values_phi_matlab, values_phi_method), (values_rho_matlab, values_rho_method)
    

if __name__ == '__main__':
    method = sys.argv[1]
    path = sys.argv[2]
    N = int(sys.argv[3])
    
    values_phi, values_rho = compute_error(method, path, N)
