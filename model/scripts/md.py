import sys
import re
import random

# TODO: add your proper path to mlab-tools
sys.path.append('/home/lucio/data/repo/mlab-tools')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.primitive import Cone
from collections import OrderedDict
import vtk
import math
from tvtk.api import tvtk

N = 100
PATH = 'data/md'

TIME_UNIT = 0.5


def to_01(color):
    r, g, b = color
    return (r/255., g/255., b/255.)


SLOW_2_THRESH = 0.03
MID_THRESH = 0.06
FAST_1_THRESH = 0.09
FAST_2_THRESH = 0.12

COLOR_FAST_2 = to_01((176,17,17))
COLOR_FAST_1 = to_01((180,69,31))
COLOR_MID = to_01((221,159,64))
COLOR_SLOW_2 = to_01((231,216,125))
COLOR_SLOW_1 = to_01((98,161,219)) 


class Particle(object):

    DEFAULT_COLOR = COLOR_SLOW_1
    DEFAULT_RADIUS = 0.007

    def __init__(self, ID, radius=None):
        self.id = ID
        self.data = list()

        self.radius = radius or self.DEFAULT_RADIUS
        self.height = 2*self.radius

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

    def add_acceleration(self, point):
        if len(self.data) <= self.acc_index:
            self.data.append(dict())
        time_data = self.data[self.acc_index]
        time_data['acceleration'] = point
        self.acc_index += 1

    def speed_from(self, vel):
        vx, vy, vz = vel
        return math.sqrt(vx*vx + vy*vy + vz*vz)

    def color_from(self, vel):
        speed = self.speed_from(vel)
        if 0 < speed < SLOW_2_THRESH:
            return COLOR_SLOW_1
        elif SLOW_2_THRESH <= speed < MID_THRESH:
            return COLOR_SLOW_2
        elif MID_THRESH <= speed < FAST_1_THRESH:
            return COLOR_MID
        elif FAST_1_THRESH <= speed < FAST_2_THRESH:
            return COLOR_FAST_1
        else:
            return COLOR_FAST_2 

    def add_to(self, animation):
        x0, y0, z0 = self.data[0]['position']
        vx0, vy0, vz0 = self.data[0]['velocity']

        self.primitive = Sphere(
                            radius=self.radius,
                            center=(x0, y0, z0))

        animation.add_animated_object(
                            self.primitive,
                            self.animate,
                            color=self.DEFAULT_COLOR)

    def animate(self, primitive, frame_no):
        if frame_no >= len(self.data): StopAnimation()

        new_pos = self.data[frame_no]['position']
        primitive.set_center(new_pos)

        prev_vel = self.data[frame_no-1]['velocity']
        new_vel = self.data[frame_no]['velocity']
        #if new_vel != prev_vel:
        #    new_color = self.color_from(new_vel)
        #    primitive.update_properties(color=new_color)


class MDAnimation(Animation):

    def initialize(self):
        parser = GeometryParser.from_VTK('%s/md_box_3d.vtk' % PATH)
        self.geometry = parser.parse()
        self.polys = list()
        self.time = 0
        
        for pid, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            self.add_object(polyhedron, opacity=0.05, color=(1,1,1))
                
        self.set_camera_parameters()
        self.initialize_particles()

    def set_camera_parameters(self):
        focalpoint = [0.5,0.5,0.4] #[0.5, 0.5, 0.05] 
        distance = -1.5 #-2
        azimuth =  -136 # 0
        elevation = 75 #0
        roll = 78 #0

        self.update_camera(focalpoint=focalpoint,
                           distance=distance,
                           azimuth=azimuth,
                           elevation=elevation,
                           roll=roll)

    def read_data(self, data_type):
        regex = re.compile('(.*?)[\t\s]+(.*)')

        if data_type == 'position':
            x, y, z = 'x', 'y', 'z'
        elif data_type == 'velocity':
            x, y, z = 'vx', 'vy', 'vz'
        else:
            x, y, z = 'ax', 'ay', 'az'

        for i in range(1, N+1):    
            particle = self.particles[i-1]
            method = getattr(particle, 'add_%s' % data_type)
                
            with open('%s/%s[%d].dat' % (PATH,x,i), 'r') as _file:
                    datax = _file.readlines()
            with open('%s/%s[%d].dat' % (PATH,y,i), 'r') as _file:
                    datay = _file.readlines()
            with open('%s/%s[%d].dat' % (PATH,z,i), 'r') as _file:
                    dataz = _file.readlines()
                        
            for idx in xrange(len(datax)):
                time, dx = map(float, regex.findall(datax[idx])[0])
                dy = float(regex.findall(datay[idx])[0][1])
                dz = float(regex.findall(dataz[idx])[0][1])

                method((dx,dy,dz))
                
    def initialize_particles(self):
        self.particles = [Particle(i) for i in xrange(N)]
        
        self.read_data('position')
        self.read_data('velocity')

        for particle in self.particles:
            particle.add_to(self)
            
    def on_frame(self, frame_no):
        self.time += TIME_UNIT
        print 'Time: %.1f' % self.time

        #print self.get_camera().parameters()
            
        
def run_animation():
    animation = MDAnimation(640, 480)
    animation.run(delay=10, framerate=30)#, save_to='md')


if __name__ == '__main__':
    run_animation()

