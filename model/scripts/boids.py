import sys
import re
import random
import os

# TODO: add your proper path to mlab-tools
#sys.path.append('path/to/mlab-tools')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.primitive import Cone
from collections import OrderedDict
import vtk
import math
from tvtk.api import tvtk

N = 250
PATH = 'data/boids'

TIME_UNIT = 0.1

CUBE_DATA = {'name':'cube', 'file':'cube_surface.vtk', 'width':640,  'height':480}
TORUS_DATA = {'name':'torus', 'file':'torus_surface.vtk', 'width':640,  'height':480}
SPHERE_DATA = {'name':'sphere', 'file':'sphere_surface.vtk', 'width':640,  'height':480}
HAND_DATA = {'name':'hand', 'file':'hand_OK_2.vtk', 'width':360,  'height':480}


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
        if new_vel != prev_vel:
            primitive.set_direction(new_vel)


class BoidsAnimation(Animation):

    def __init__(self, width, height, geom_data):
        Animation.__init__(self, width, height)
        self.geom_data = geom_data
    
    def initialize(self):
        parser = GeometryParser.from_VTK('%s/%s' % (PATH,self.geom_data['file']))
        self.geometry = parser.parse()
        self.polys = list()
        self.time = 0
        
        for pid, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            self.add_object(polyhedron, opacity=0.1)
                
        self.initialize_boids()
        self.set_camera_parameters()

    def set_camera_parameters(self):
        if self.geom_data['name'] == CUBE_DATA['name']:
            focalpoint = [0,-30,120]
            distance = 800
            azimuth = 0
            elevation = 90
            roll = -90
        elif self.geom_data['name'] == TORUS_DATA['name']:
            focalpoint = [-5.34,15,-24.28]
            distance = 100
            azimuth = 93
            elevation = 147
            roll = 0.46
        elif self.geom_data['name'] == SPHERE_DATA['name']:
            focalpoint = [0,0,0]
            distance = 0
            azimuth = 180
            elevation = 90
            roll = 90
        elif self.geom_data['name'] == HAND_DATA['name']:
            focalpoint = [-3,-5.7,60]
            distance = 170
            azimuth = -160
            elevation = 92
            roll = 92
        else:
            raise RuntimeError('Geometry %s not supported!' % self.geom_data['file'])

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

                method((dx,dy,dz))
                
    def initialize_boids(self):
        if self.geom_data['name'] == CUBE_DATA['name']:
            radius = 1
        elif self.geom_data['name'] == TORUS_DATA['name']:
            radius = 0.15
        elif self.geom_data['name'] == SPHERE_DATA['name']:
            radius = 0.004
        elif self.geom_data['name'] == HAND_DATA['name']:
            radius = 0.2

        self.boids = [Boid(i,radius) for i in xrange(N)]
        
        self.read_data('position')
        self.read_data('velocity')

        for boid in self.boids:
            boid.add_to(self)
            
    def on_frame(self, frame_no):
        self.time += TIME_UNIT
        print 'Time: %.1f' % self.time

        if self.time > 0.4:
            StopAnimation()
            
        
def run_animation():
    geom_data = TORUS_DATA
    if len(sys.argv) > 1:
        geometry = sys.argv[1].lower()
        if 'hand' in geometry:
            # IMPORTANTE: para animaciones de mano, conviene sacar los frames
            # sin la geometria y luego mergearles la mano por separado. El 
            # frame de la mano y el script para hacerlo estan en 
            # data/boids/hand
            geom_data = HAND_DATA
        elif 'sphere' in geometry:
            geom_data = SPHERE_DATA
        if 'cube' in geometry:
            geom_data = CUBE_DATA

    video_name = 'boids_%s' % geom_data['name']

    animation = BoidsAnimation(geom_data['width'], geom_data['height'], geom_data)
    animation.run(delay=10, framerate=30, save_to=video_name)
    os.system('ffmpeg -i %s.avi %s.mp4' % (video_name, video_name))


if __name__ == '__main__':
    run_animation()

