import os
import random
import sys
import traceback

MATLAB_CMD = 'octave %s/model/pic/flow_around_plate.m > /dev/null 2>&1'
RETQSS_CMD = '%s/build/%s/%s > /dev/null 2>&1'
OUTPUT_DIR = 'output'

method = sys.argv[1].lower()
N = int(sys.argv[2])

if len(sys.argv) > 3:
    OUTPUT_DIR = sys.argv[3]

path = os.getenv('MMOC_PATH')

sys.path.append('%s/model/pic/scripts')

import to_csv

if method == 'matlab':
    command = MATLAB_CMD % path
else:
    command = RETQSS_CMD % (path, method, method)
    phi_filename = 'PHI[%d].dat'
    rho_filename = 'RHO[%d].dat'
    use_nodes = 'nodes' in method
    _dir = '%s/build/%s' % (path, method)
    tmp_dir = 'tmp_%d' % random.randint(1,100000)
    os.system('mkdir %s' % tmp_dir)
    os.system('cp %s/%s.ini %s' % (_dir,method,tmp_dir))
    os.system('cp -r %s/model/geom %s/' % (path,tmp_dir))

os.system('mkdir -p %s' % OUTPUT_DIR)

try:
    for i in xrange(N):
        print 'Running experiment %d...' % i

        if method == 'matlab':
            os.system(command)
            os.system('mv phi.txt %s/phi_matlab-%d.txt' % (OUTPUT_DIR,i))
            os.system('mv rho.txt %s/rho_matlab-%d.txt' % (OUTPUT_DIR,i))
        else:
            os.chdir(tmp_dir)
            os.system(command)
            os.chdir('..')

            grid = to_csv.get_output_grid('%s/%s' % (tmp_dir,phi_filename), use_nodes=use_nodes)
            to_csv.save_grid_to_file('%s/phi_%s-%d.txt' % (OUTPUT_DIR,method,i), grid)

            grid = to_csv.get_output_grid('%s/%s' % (tmp_dir,rho_filename), use_nodes=use_nodes)
            to_csv.save_grid_to_file('%s/rho_%s-%d.txt' % (OUTPUT_DIR,method,i), grid)
except Exception, e:
    traceback.print_exc(e)

if method != 'matlab':
    os.system('rm -rf %s' % tmp_dir)


