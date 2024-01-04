import sys
import re
import random
sys.path.append('../')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.polyline import AnimatedPolyLine

N = 50

class CubeMeshAnimation(Animation):
    
    """This example shows how to use a VTK geometry file to render a 3D polyhedron mesh."""
    
    def initialize(self):
        self.parse_trajectory()
        
        parser = GeometryParser.from_VTK('data/rays/rays.vtk')
        self.geometry = parser.parse()
        self.polys = list()
        
        for _, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            self.add_object(polyhedron, opacity=0.1)
            
        self.update_camera(focalpoint=[50,0,0],
                           distance=800,
                           azimuth=-51,
                           elevation=94,
                           roll=86)
                
    def parse_trajectory(self):
        points = list()
        self.times = list()
        self.trajectory = list()
        colors = list()
        
        regex = re.compile('(.*?)[\t\s]+(.*)')

        for i in range(1, N+1):    
            points.append(list())
            self.times.append(list())
            color = (random.random(), random.random(), random.random())
            colors.append(color)
                
            with open('data/rays/x[%d].dat' % i, 'r') as _file:
                    datax = _file.readlines()
            with open('data/rays/y[%d].dat' % i, 'r') as _file:
                    datay = _file.readlines()
            with open('data/rays/z[%d].dat' % i, 'r') as _file:
                    dataz = _file.readlines()
                        
            for idx in xrange(len(datax)):
                time, x = map(float, regex.findall(datax[idx])[0])
                y = float(regex.findall(datay[idx])[0][1])
                z = float(regex.findall(dataz[idx])[0][1])
                
                points[i-1].append((x,y,z))
                self.times[i-1].append(time)
                
            trajectory = AnimatedPolyLine(points[i-1])
            self.add_object(trajectory, color=colors[i-1])
            self.trajectory.append(trajectory)
            
    def on_frame(self, frame_no):
        for poly in self.polys:
            poly.transform(rotate=(0,0,-0.6))
        for t in self.trajectory:
            t.transform(rotate=(0,0,-0.6))
        
        dist = self.get_camera().parameters()['distance']
        if dist > 100:
            self.update_camera(distance=-2)
        print self.get_camera().parameters()
        
        for i in range(1, N+1):
            current_point_idx = self.trajectory[i-1].current_point()
            if current_point_idx == self.trajectory[i-1].num_points()-1:
                StopAnimation()
        

def run_animation():
    animation = CubeMeshAnimation(640, 480)
    animation.run(delay=20, save_to='rays')


if __name__ == '__main__':
    run_animation()

