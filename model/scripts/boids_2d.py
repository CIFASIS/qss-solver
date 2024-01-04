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
import math
from tvtk.api import tvtk

N = 200
PATH = 'data/boids_2d'

TIME_UNIT = 0.1

def rotate(boid, new_dir):
    v1 = vtk.vtkVector3d(boid._dir)
    v2 = vtk.vtkVector3d(new_dir)

    n1 = v1.Norm()
    n2 = v2.Norm()

    ang = math.acos(v1.Dot(v2)/(n1*n2)) * (180./math.pi)
    ax = v1.Cross(v2)

    t = tvtk.Transform()
    t.rotate_wxyz(ang, ax[0], ax[1], ax[2])
    boid.actor.user_transform = t
    boid._dir = new_dir


class Boid(object):

    DEFAULT_COLOR = (0,0,0)
    DEFAULT_RADIUS = 1
    DEFAULT_HEIGHT = 2

    def __init__(self, ID):
        self.id = ID
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

    def add_acceleration(self, point):
        if len(self.data) <= self.acc_index:
            self.data.append(dict())
        time_data = self.data[self.acc_index]
        time_data['acceleration'] = point
        self.acc_index += 1

    def add_to(self, animation):
        x0, y0, z0 = self.data[0]['position']
        vx0, vy0, vz0 = self.data[0]['velocity']

        self.primitive = Cone(
                            self.DEFAULT_RADIUS,
                            self.DEFAULT_HEIGHT,
                            center=(x0, y0, z0),
                            direction=(vx0, vy0, vz0))

        animation.add_animated_object(
                            self.primitive,
                            self.animate,
                            color=self.DEFAULT_COLOR)

    def animate(self, primitive, frame_no):
        if frame_no >= len(self.data): StopAnimation()

        #prev_pos = self.data[frame_no-1]['position']
        new_pos = self.data[frame_no]['position']

        primitive.set_center(new_pos)
        #x = new_pos[0] - prev_pos[0]
        #y = new_pos[1] - prev_pos[1]
        #z = new_pos[2] - prev_pos[2]
        #primitive.transform(translate=(x,y,z))

        prev_vel = self.data[frame_no-1]['velocity']
        new_vel = self.data[frame_no]['velocity']
        if new_vel != prev_vel:
            primitive.set_direction(new_vel)


class BoidsAnimation(Animation):
    
    def initialize(self):
        parser = GeometryParser.from_VTK('%s/boids_2d_shell.vtk' % PATH)
        self.geometry = parser.parse()
        self.polys = list()
        self.time = 0
        
        for pid, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            self.add_object(polyhedron, opacity=0.1)
                
        self.initialize_boids()
            
        self.update_camera(focalpoint=[200,250,10],
                           distance=1000,
                           azimuth=0,
                           elevation=180,
                           roll=0)

    def read_data(self, data_type):
        regex = re.compile('(.*?)[\t\s]+(.*)')

        if data_type == 'position':
            x, y = 'x', 'y'
        elif data_type == 'velocity':
            x, y = 'vx', 'vy'

        for i in range(1, N+1):    
            boid = self.boids[i-1]
            method = getattr(boid, 'add_%s' % data_type)
                
            with open('%s/%s[%d].dat' % (PATH,x,i), 'r') as _file:
                    datax = _file.readlines()
            with open('%s/%s[%d].dat' % (PATH,y,i), 'r') as _file:
                    datay = _file.readlines()
                        
            for idx in xrange(len(datax)):
                time, dx = map(float, regex.findall(datax[idx])[0])
                dy = float(regex.findall(datay[idx])[0][1])
                dz = 15. if data_type == 'position' else 0.

                method((dx,dy,dz))
                
    def initialize_boids(self):
        self.boids = [Boid(i) for i in xrange(N)]
        
        self.read_data('position')
        self.read_data('velocity')

        for boid in self.boids:
            boid.add_to(self)
            
    def on_frame(self, frame_no):
        self.time += TIME_UNIT
        print 'Time: %.1f' % self.time
        
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
            
        
def run_animation():
    animation = BoidsAnimation(640, 480)
    animation.run(delay=10, framerate=30, save_to='boids_2d')


if __name__ == '__main__':
    run_animation()

