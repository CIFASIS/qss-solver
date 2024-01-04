import sys
import re
import random

# TODO: add your proper path to mlab-tools
sys.path.append('/home/lucio/repo/mlab-tools')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.primitive import Cone
from collections import OrderedDict
import vtk
import random
import math
from tvtk.api import tvtk

N = 25
PATH = 'data/periodic'

TIME_UNIT = 0.1

PERIODIC_VOLS = [11, 22, 31, 33, 44, 55, 62, 66, 110, 121, 124, 132, 176, 186, 187, 198, 209, 217, 220, 231]




class Boid(object):

    DEFAULT_COLOR = (0,0,0)
    DEFAULT_RADIUS = 1

    def __init__(self, ID, radius=None):
        self.id = ID
        self.data = list()

        self.radius = 1.2# radius or DEFAULT_RADIUS
        self.height = 2*radius
        self.color = (random.random(), random.random(), random.random())

        self.pos_index = 0
        self.vel_index = 0

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

    def add_to(self, animation):
        x0, y0, z0 = self.data[0]['position']
        vx0, vy0, vz0 = self.data[0]['velocity']

        self.primitive = Cone(
                            self.radius,
                            self.height,
                            center=(x0, y0, z0),
                            direction=(vx0, vy0, vz0))

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
        if new_vel != prev_vel:
            primitive.set_direction(new_vel)


class PeriodicAnimation(Animation):

    def initialize(self):
        parser = GeometryParser.from_VTK('%s/cube_240.vtk' % PATH)
        self.geometry = parser.parse()
        self.polys = list()
        self.time = 0
        
        for pid, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            if pid in PERIODIC_VOLS:
                self.add_object(polyhedron, opacity=0.02, color=(1,0,0))
            else:
                self.add_object(polyhedron, opacity=0.05)
                
        self.set_camera_parameters()
        self.initialize_balls()

    def set_camera_parameters(self):
        focalpoint = [0,-30,120]
        distance = 500
        azimuth = 40
        elevation = 90
        roll = -90

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

        for i in range(1, N+1):    
            ball = self.balls[i-1]
            method = getattr(ball, 'add_%s' % data_type)
                
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
                
    def initialize_balls(self):
        self.balls = [Boid(i,1) for i in xrange(N)]
        
        self.read_data('position')
        self.read_data('velocity')

        for ball in self.balls:
            ball.add_to(self)
            
    def on_frame(self, frame_no):
        self.time += TIME_UNIT
        print 'Time: %.1f' % self.time

        
def run_animation():
    animation = PeriodicAnimation(640, 480)
    animation.run(delay=10, framerate=30, save_to='periodic')


if __name__ == '__main__':
    run_animation()

