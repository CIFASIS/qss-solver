import sys
import re
import random

# TODO: add your proper path to mlab-tools
sys.path.append('/home/lucio/repo/mlab-tools')

from mlab_tools.animation import Animation, StopAnimation, StopAndRemove
from mlab_tools.geometry import GeometryParser
from mlab_tools.primitive import Cone, Cylinder
from mlab_tools.polyline import PolyLine
from collections import OrderedDict
import vtk
import math
from tvtk.api import tvtk

N = 100
PATH = 'data/boids_obstacles'

TIME_UNIT = 0.1

CUBE_GEOM = 'cube'
TORUS_GEOM = 'torus'
SPHERE_GEOM = 'sphere'


class Boid(object):

    DEFAULT_COLOR = (0,0,0)
    DEFAULT_RADIUS = 1

    def __init__(self, ID, radius=None):
        self.id = ID
        self.data = list()

        self.radius = radius or DEFAULT_RADIUS
        self.height = 2*radius

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

        if new_vel[0] == 0 and new_vel[1] == 0 and new_vel[2] == 0:
            StopAndRemove()

        if new_vel != prev_vel:
            primitive.set_direction(new_vel)


class BoidsAnimation(Animation):

    def __init__(self, width, height, geometry):
        Animation.__init__(self, width, height)
        self.geom_type = geometry
    
    def initialize(self):
        parser = GeometryParser.from_VTK('%s/cube_240_surface.vtk' % (PATH))
        self.geometry = parser.parse()
        
        for pid, polyhedron in self.geometry:
            self.add_object(polyhedron, opacity=0.1)

        parser = GeometryParser.from_VTK('%s/cube_240.vtk' % (PATH))
        geometry = parser.parse()

        self.read_obstacles()

        for pid, polyhedron in geometry:
            if pid in self.obstacles:
                self.add_object(polyhedron, opacity=0.1, color=(1,0,0))
                
        self.time = 0
        self.set_camera_parameters()
        self.initialize_boids()

    def read_obstacles(self):
        with open('%s/obstacles.dat' % PATH, 'r') as _file:
            lines = _file.readlines()
        self.obstacles = map(lambda line: float(line.strip()), lines)

    def set_camera_parameters(self):
        if self.geom_type == CUBE_GEOM:
            focalpoint = [0,-30,120]
            distance = 500
            azimuth = 45
            elevation = 90
            roll = -90
        elif self.geom_type == TORUS_GEOM:
            focalpoint = [-5.34,15,-24.28]
            distance = 100
            azimuth = 93
            elevation = 147
            roll = 0.46
        elif self.geom_type == SPHERE_GEOM:
            focalpoint = [0,0,0]
            distance = 0
            azimuth = 180
            elevation = 90
            roll = 90
        else:
            raise RuntimeError('Geometry %s not supported!' % self.geom_type)

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
            boid = self.boids[i-1]
            method = getattr(boid, 'add_%s' % data_type)
                
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

                if dx == 0 and dy == 0 and dz == 0:
                    print i

                method((dx,dy,dz))
                
    def initialize_boids(self):
        if self.geom_type == CUBE_GEOM:
            radius = 0.7
        elif self.geom_type == TORUS_GEOM:
            radius = 0.15
        elif self.geom_type == SPHERE_GEOM:
            radius = 0.004

        self.boids = [Boid(i,radius) for i in xrange(N)]
        
        self.read_data('position')
        self.read_data('velocity')

        for boid in self.boids:
            boid.add_to(self)
            
    def on_frame(self, frame_no):
        self.time += TIME_UNIT
        print 'Time: %.1f' % self.time
            
        
def run_animation():
    geometry = TORUS_GEOM
    if len(sys.argv) > 1:
        geometry = sys.argv[1]

    animation = BoidsAnimation(640, 480, geometry)
    animation.run(delay=10, framerate=30, save_to='boids_obstacles')


if __name__ == '__main__':
    run_animation()

