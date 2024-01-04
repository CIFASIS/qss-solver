import os
import random
import sys
import traceback

MATLAB_CMD = 'octave %s/model/pic/flow_around_plate.m > /dev/null 2>&1'
RETQSS_CMD = '%s/build/%s/%s > /dev/null 2>&1'
OUTPUT_DIR = 'output'

DT = 1.0620389946518801e-07
CHECKPOINTS = [50,100,150,200]

method = 'pic_nodes_2_ic'

N = int(sys.argv[1])

if len(sys.argv) > 2:
    OUTPUT_DIR = sys.argv[2]

path = os.getenv('MMOC_PATH')

sys.path.append('%s/model/pic/scripts')

import to_csv

matlab_command = MATLAB_CMD % path
method_command = RETQSS_CMD % (path, method, method)

phi_filename = 'PHI[%d].dat'
rho_filename = 'RHO[%d].dat'
use_nodes = 'nodes' in method
_dir = '%s/build/%s' % (path, method)
tmp_dir = 'tmp_%d' % random.randint(1,100000)
os.system('mkdir -p %s/data' % tmp_dir)
os.system('cp %s/%s.ini %s' % (_dir,method,tmp_dir))
os.system('cp -r %s/model/geom %s/' % (path,tmp_dir))

os.system('mkdir -p %s' % OUTPUT_DIR)

try:
    for i in xrange(N):
        print 'Running experiment %d...' % i

        os.system(matlab_command)
        os.system('cp particles.txt %s/particles-%d.txt' % (OUTPUT_DIR,i))
        os.system('mv particles.txt %s/data/' % tmp_dir)

        for t in CHECKPOINTS:
            os.system('mv phi-T=%d.txt %s/phi_matlab-T=%d-%d.txt' % (t,OUTPUT_DIR,t,i))
            os.system('mv rho-T=%d.txt %s/rho_matlab-T=%d-%d.txt' % (t,OUTPUT_DIR,t,i))

        os.chdir(tmp_dir)
        os.system(method_command)
        os.chdir('..')

        for t in CHECKPOINTS:
            dt = t*DT
    
            grid = to_csv.get_output_grid('%s/%s' % (tmp_dir,phi_filename), time=dt, use_nodes=use_nodes)
            to_csv.save_grid_to_file('%s/phi_%s-T=%d-%d.txt' % (OUTPUT_DIR,method,t,i), grid)

            grid = to_csv.get_output_grid('%s/%s' % (tmp_dir,rho_filename), time=dt, use_nodes=use_nodes)
            to_csv.save_grid_to_file('%s/rho_%s-T=%d-%d.txt' % (OUTPUT_DIR,method,t,i), grid)
except Exception, e:
    traceback.print_exc(e)

os.system('rm -rf %s' % tmp_dir)


