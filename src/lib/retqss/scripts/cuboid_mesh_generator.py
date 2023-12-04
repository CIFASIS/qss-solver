#!/usr/bin/python2.7

import sys

from math import ceil
from argparse import ArgumentParser


VTK_VOXEL = 11


def parse_options():
    parser = ArgumentParser()

    parser.add_argument('-x', '--x-coords', dest='x', action='store',
                        type=float, nargs=2, default=[0,10],
                        help='x coordinates')

    parser.add_argument('-y', '--y-coords', dest='y', action='store',
                        type=float, nargs=2, default=[0,10],
                        help='y coordinates')

    parser.add_argument('-z', '--z-coords', dest='z', action='store',
                        type=float, nargs=2, default=[0,10],
                        help='z coordinates')

    parser.add_argument('-c', '--cuboid-dimensions', dest='box_dim', action='store',
                        type=float, nargs=3, default=[1,1,1],
                        help='cuboid dimensions')

    options = parser.parse_args()

    return options


def point_idx(geom_dim, d, box_dim, i, j, k):
    x, y, z = geom_dim
    lx, ly, lz = box_dim

    px = x[0] + i*lx
    py = y[0] + j*ly
    pz = z[0] + k*lz

    return d[(px,py,pz)]


def generate_geometry(box_dim, geom_dim):
    print '# vtk DataFile Version 2.0'    
    print 'Unstructured Grid'
    print 'ASCII'
    print 'DATASET UNSTRUCTURED_GRID'

    lx, ly, lz = box_dim
    x, y, z = geom_dim

    length = x[1] - x[0]
    width = y[1] - y[0]
    height = z[1] - z[0]

    nx = int(1 + ceil(length/lx))
    ny = int(1 + ceil(width/ly))
    nz = int(1 + ceil(height/lz))
    points = nx*ny*nz

    print 'POINTS {} double'.format(points)

    index = 0
    point_dict = dict()
    for i in xrange(nx):
        for j in xrange(ny):
            for k in xrange(nz):
                px = x[0] + i*lx
                py = y[0] + j*ly
                pz = z[0] + k*lz
                print '{} {} {}'.format(px,py,pz)

                point_dict[(px,py,pz)] = index
                index += 1

    print

    cells = (nx-1)*(ny-1)*(nz-1)
    cells_size = 9*cells
    print 'CELLS {} {}'.format(cells, cells_size)
    for i in xrange(1,nx):
        for j in xrange(1,ny):
            for k in xrange(1,nz):
                p0 = point_idx(geom_dim, point_dict, box_dim, i-1, j-1, k-1)
                p1 = point_idx(geom_dim, point_dict, box_dim, i, j-1, k-1)
                p2 = point_idx(geom_dim, point_dict, box_dim, i-1, j, k-1)
                p3 = point_idx(geom_dim, point_dict, box_dim, i, j, k-1)
                p4 = point_idx(geom_dim, point_dict, box_dim, i-1, j-1, k)
                p5 = point_idx(geom_dim, point_dict, box_dim, i, j-1, k)
                p6 = point_idx(geom_dim, point_dict, box_dim, i-1, j, k)
                p7 = point_idx(geom_dim, point_dict, box_dim, i, j, k)
                print '{} {} {} {} {} {} {} {} {}'.format(8, p0, p1, p2, p3, p4, p5, p6, p7)

    print

    print 'CELL_TYPES {}'.format(cells)
    for _ in xrange(cells):
        print VTK_VOXEL

if __name__ == '__main__':
    options = parse_options()
    geom_dim = (options.x, options.y, options.z)
    box_dim = options.box_dim

    generate_geometry(box_dim, geom_dim)
