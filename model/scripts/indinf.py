import sys
import re, os
import random

# TODO: add your proper path to mlab-tools
sys.path.append('/home/lucio/repo/mlab-tools')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.primitive import Sphere
from collections import OrderedDict
import vtk
import math
from tvtk.api import tvtk

N = 200
PATH = 'data/indinf'

TIME_UNIT = 0.2

INFECTED = 0
NOT_INFECTED = 1
RECOVERED = 2
DEAD = 3
DETECTED = 4
ISOLATED = 5


class Person(object):

    INFECTED_COLOR = (1,0,0)
    NOT_INFECTED_COLOR = (0,0,1)
    RECOVERED_COLOR = (0,1,0)
    DETECTED_COLOR = (0.5,0,1)
    ISOLATED_COLOR = (1,0.65,0)

    DEFAULT_RADIUS = 0.5

    def __init__(self, ID, status):
        self.id = ID
        self.status = status
        self.data = list()
        self.pos_index = 0
        self.vel_index = 0
        self.acc_index = 0

    def add_position(self, point):
        if len(self.data) <= self.pos_index:
            self.data.append(dict())
        time_data = self.data[self.pos_index]
        time_data['position'] = point
        self.pos_index += 1

    def add_velocity(self, point):
        if len(self.data) <= self.vel_index:
            self.data.append(dict())
        time_data = self.data[self.vel_index]
        time_data['velocity'] = point
        self.vel_index += 1

    def get_color(self, idx):
        status = self.status[idx]
        if status == INFECTED:
            color = self.INFECTED_COLOR
        elif status == NOT_INFECTED:
            color = self.NOT_INFECTED_COLOR
        elif status == ISOLATED:
            color = self.ISOLATED_COLOR
        elif status == DETECTED:
            color = self.DETECTED_COLOR
        else:
            color = self.RECOVERED_COLOR
        return color

    def add_to(self, animation):
        x0, y0, z0 = self.data[0]['position']

        self.primitive = Sphere(
                            radius=self.DEFAULT_RADIUS,
                            center=(x0, y0, z0))

        color = self.get_color(0)

        animation.add_animated_object(
                            self.primitive,
                            self.animate,
                            color=color)

    def animate(self, primitive, frame_no):
        if frame_no >= len(self.data): StopAnimation()

        new_pos = self.data[frame_no]['position']
        primitive.set_center(new_pos)

        color = self.get_color(frame_no-1)

        primitive.update_properties(color=color)



class Volume(object):

    INFECTED_COLOR = (1,0,0)
    NOT_INFECTED_COLOR = (1,1,1)

    def __init__(self, pid, poly, status):
        self.pid = pid
        self.poly = poly
        self.status = status

    def add_to(self, animation):
        self.animation = animation

        new_status = self.status[0]
        if new_status == INFECTED:
            color = self.INFECTED_COLOR
        elif new_status == NOT_INFECTED:
            color = self.NOT_INFECTED_COLOR        

        animation.add_static_object(
                            self.poly,
                            #self.animate,
                            color=color,
                            opacity=0.1)

    def update(self, new_status):
        if new_status == INFECTED:
            color = self.INFECTED_COLOR
        elif new_status == NOT_INFECTED:
            color = self.NOT_INFECTED_COLOR

        self.animation.remove_object(self.poly)
        self.animation.add_static_object(self.poly, color=color, opacity=0.1)
    
    def animate(self, primitive, frame_no):
        new_status = self.status[frame_no-1]

        if new_status == INFECTED:
            color = self.INFECTED_COLOR
        elif new_status == NOT_INFECTED:
            color = self.NOT_INFECTED_COLOR
    
        self.poly.update_properties(color=color, opacity=0.1)



class IndInfAnimation(Animation):
    
    def initialize(self):
        parser = GeometryParser.from_VTK('%s/indinf_4.vtk' % PATH)
        self.geometry = parser.parse()
        self.polys = list()
        self.time = 0

        self.read_vol_status()

        
        for pid, polyhedron in self.geometry:
            vol = Volume(pid, polyhedron, self.status[pid])

            vol.add_to(self)
            
            self.polys.append(vol)

        self.initialize_persons()
            
        self.update_camera(focalpoint=[50,50,8],
                           distance=180,
                           azimuth=0,
                           elevation=0,
                           roll=0)

    def read_vol_status(self):
        with open('%s/indirect_infection_output.dat' % PATH, 'r') as _f:
            lines = _f.readlines()
        self.status = dict()
        self.changes = [list()]

        for idx in xrange(2,len(lines)+1,3):
            status = map(int, lines[idx].split(' '))
            for j in xrange(len(status)):
                vstatus = self.status.setdefault(j+1, list())
                vstatus.append(status[j])

        frame_no = 1
        for idx in xrange(2,len(lines)+1,3):
            self.changes.append(list())
            for v in xrange(1,len(self.status)+1):
                if frame_no > 1:
                    last_value = self.status[v][frame_no-2]
                    new_value = self.status[v][frame_no-1]
                    if new_value != last_value:
                        self.changes[frame_no].append((v,new_value))
            frame_no += 1

    def read_data(self, data_type):
        regex = re.compile('(.*?)[\t\s]+(.*)')

        if data_type == 'position':
            x, y = 'x', 'y'
        elif data_type == 'velocity':
            x, y = 'vx', 'vy'

        for i in range(1, N+1):    
            person = self.persons[i-1]
            method = getattr(person, 'add_%s' % data_type)
                
            with open('%s/%s[%d].dat' % (PATH,x,i), 'r') as _file:
                    datax = _file.readlines()
            with open('%s/%s[%d].dat' % (PATH,y,i), 'r') as _file:
                    datay = _file.readlines()
                        
            for idx in xrange(len(datax)):
                time, dx = map(float, regex.findall(datax[idx])[0])
                dy = float(regex.findall(datay[idx])[0][1])
                dz = 0.9 if data_type == 'position' else 0.

                method((dx,dy,dz))
                
    def initialize_persons(self):
        with open('%s/indirect_infection_output.dat' % PATH, 'r') as _f:
            lines = _f.readlines()
        status = dict()
        for idx in xrange(1,len(lines)+1,3):
            values = map(int, lines[idx].split(' '))
            for j in xrange(len(values)):
                pstatus = status.setdefault(j, list())
                pstatus.append(values[j])

        self.persons = [Person(i, status[i]) for i in xrange(N)]
        
        self.read_data('position')
        self.read_data('velocity')

        for person in self.persons:
            person.add_to(self)
            
    def on_frame(self, frame_no):
        self.time += TIME_UNIT
        print 'Time: %.1f' % self.time

        changes = self.changes[frame_no]
        for v,value in changes:
            vol = self.polys[v-1]
            vol.update(value)

        if self.time >= 50:
            StopAnimation()
        
        #dist = self.get_camera().parameters()['distance']
        #az = self.get_camera().parameters()['azimuth']
        #roll = self.get_camera().parameters()['roll']
        
        #if dist > 525:
        #    self.update_camera(distance=-10)

        #print self.get_camera().parameters()
        #elif 88.8 < az < 90:
        #    self.update_camera(roll=90-roll, azimuth=91-az)
        #elif -91.2 < az < -90:
        #    self.update_camera(roll=-90-roll, azimuth=-89-az)
        #else:
        #    self.update_camera(azimuth=1)
            
        
def run_animation(video_name):
    animation = IndInfAnimation(590, 640)
    if video_name:
        animation.run(delay=10, framerate=30, save_to=video_name)
        os.system('ffmpeg -i %s.avi %s.mp4' % (video_name, video_name))
    else:
        animation.run(delay=10, framerate=30)


if __name__ == '__main__':
    video_name = None
    if len(sys.argv) > 1:
        video_name = sys.argv[1].lower()
    run_animation(video_name)

