#!/usr/bin/python2.7

import sys

pic_file = sys.argv[1]
cells = int(sys.argv[2])
try:
    particles = int(sys.argv[3])
except:
    particles = 27000


with open(pic_file) as f:
    content = f.read()

backup = '%s.bkp' % pic_file
with open(backup, 'w') as f:
    f.write(content)

idx = 3*particles + 1
content = content.replace('modelData->event[%d].nLHSDsc += 4;' % idx, 'modelData->event[%d].nLHSDsc += 4*%d;' % (idx,cells))
content = content.replace('modelData->event[%d].nLHSDsc += 2;' % idx, 'modelData->event[%d].nLHSDsc += 2*%d;' % (idx,cells))

idx = 2*particles + 1
content = content.replace('modelData->event[%d].nLHSDsc += 4;' % idx, 'modelData->event[%d].nLHSDsc += 4*%d;' % (idx,cells))
content = content.replace('modelData->event[%d].nLHSDsc += 2;' % idx, 'modelData->event[%d].nLHSDsc += 2*%d;' % (idx,cells))

idx = 2*particles
content = content.replace('modelData->event[%d].nLHSDsc += 4;' % idx, 'modelData->event[%d].nLHSDsc += 4*%d;' % (idx,cells))
content = content.replace('modelData->event[%d].nLHSDsc += 2;' % idx, 'modelData->event[%d].nLHSDsc += 2*%d;' % (idx,cells))

content = content.replace('for(i = 0; i <= 0; i++)', 'for(i0 = 0; i0 <= %d; i0++)' % (cells-1))

with open(pic_file, 'w') as f:
    f.write(content)


