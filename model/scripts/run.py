#!/usr/bin/python2.7

import os
import sys

BASE_PATH = os.environ['MMOC_PATH']
MMOC_PATH = '%s/bin/mmoc' % BASE_PATH
PACKAGES_PATH = '%s/packages/retqss' % BASE_PATH
OUTPUT_PATH = '%s/build' % BASE_PATH


def run_mmoc(model, model_name, debug, code):
    cmd = 'rm -f %s/pkg*' % PACKAGES_PATH
    os.system(cmd)

    cmd = 'mkdir -p %s/%s' % (OUTPUT_PATH, model_name)
    os.system(cmd)

    parallel = '-p' if code=='parallel' else ''

    if debug:
        cmd = '%s %s %s -i %s -o %s/%s/%s -d SD_DBG_Dt' % (MMOC_PATH, model, parallel, PACKAGES_PATH, OUTPUT_PATH, model_name, model_name)
    else:
        cmd = '%s %s %s -i %s -o %s/%s/%s' % (MMOC_PATH, model, parallel, PACKAGES_PATH, OUTPUT_PATH, model_name, model_name)
    os.system(cmd)

def run_gcc(model_name):
    cmd = 'rm -f %s/%s/retQSS*' % (OUTPUT_PATH, model_name)
    os.system(cmd)

    cmd = 'rm -f %s/%s/*.dat' % (OUTPUT_PATH, model_name)
    os.system(cmd)

    cmd = 'rm -f %s/%s/%s' % (OUTPUT_PATH, model_name, model_name)
    os.system(cmd)

    cmd = 'make -f %s/%s/%s.makefile' % (OUTPUT_PATH, model_name, model_name)
    os.system(cmd)

def copy_geometry(model_name):
    cmd = 'mkdir -p %s/%s/geom' % (OUTPUT_PATH, model_name)
    os.system(cmd)

    cmd = 'ln -srf %s/model/geom/* %s/%s/geom' % (BASE_PATH, OUTPUT_PATH, model_name)
    os.system(cmd)

if __name__ == '__main__':
    model = sys.argv[1]
    if model.startswith('pic'):
        model = '%s/model/pic/%s.mo' % (BASE_PATH, sys.argv[1])
    else:
        model = '%s/model/%s.mo' % (BASE_PATH, sys.argv[1])
    mode = 'all-debug'
    code = 'serial'

    if len(sys.argv) > 2:
        mode = sys.argv[2].lower()
    if len(sys.argv) > 3:
        code = sys.argv[3].lower()

    if mode in ['all-debug', 'mmoc-debug']:
        run_mmoc(model, sys.argv[1], True, code)
    elif mode in ['all-release', 'mmoc-release']:
        run_mmoc(model, sys.argv[1], False, code)
    if mode in ['all-debug', 'all-release', 'gcc']:
        run_gcc(sys.argv[1])
    copy_geometry(sys.argv[1])

    if 'indirect_infection' in model or 'seir' in model:
        path = model[:-3].replace('model', 'build')
        filename = '%s/run.py' % path
        executable = os.path.split(path)[-1]
        content = str()

        content += 'import os\n'
        content += 'os.system("rm -rf *.dat")\n'
        content += 'os.system("./%s")\n' % executable
        content += 'os.system("python ../../model/scripts/plot_%s.py %s_output.dat contacts_inf.adjlist contacts_ninf.adjlist")\n' % (executable,executable)

        with open(filename, 'w') as f:
            f.write(content)
