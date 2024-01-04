import sys
import re
sys.path.append('../')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.point import Point


class BallAnimation(Animation):
    
    def initialize(self):
        self.parse_trajectory()
        
        parser = GeometryParser.from_VTK('data/ball/ball.vtk')
        self.geometry = parser.parse()
        self.polys = list()
        
        for _, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            self.add_object(polyhedron, opacity=0.1)
            
        self.update_camera(focalpoint=[8,2.5,15],
                           distance=500,
                           azimuth=-89,
                           elevation=90,
                           roll=-90)
                
    def parse_trajectory(self):
        self.times = list()
        self.trajectory = list()
        regex = re.compile('(.*?)[\t\s]+(.*)')

        self.points = list()
        self.times.append(list())
            
        with open('data/ball/x.dat', 'r') as _file:
                datax = _file.readlines()
        with open('data/ball/y.dat', 'r') as _file:
                datay = _file.readlines()
        with open('data/ball/z.dat', 'r') as _file:
                dataz = _file.readlines()
                    
        for idx in xrange(len(datax)):
            time, x = map(float, regex.findall(datax[idx])[0])
            y = float(regex.findall(datay[idx])[0][1])
            z = float(regex.findall(dataz[idx])[0][1])
            
            self.points.append((x,y,z))
            self.times[0].append(time)
            
        x0, y0, z0 = self.points[0]
        self.ball = Point(x0, y0, z0, 0.3)
        self.add_animated_object(self.ball, self.animate_ball, color=(0,0,1))
        self.point_idx = 1
        
    def animate_ball(self, ball, frame_no):
        x = self.points[self.point_idx][0] - self.points[self.point_idx-1][0]
        y = self.points[self.point_idx][1] - self.points[self.point_idx-1][1]
        z = self.points[self.point_idx][2] - self.points[self.point_idx-1][2]                
        ball.transform(translate=(x,y,z))
        self.point_idx += 1
        if self.point_idx >= len(self.points): StopAnimation()
            
    def on_frame(self, frame_no):
        dist = self.get_camera().parameters()['distance']
        az = self.get_camera().parameters()['azimuth']
        
        if az >= 90:
            StopAnimation()
            
        if dist > 70:
            self.update_camera(distance=-2)
        else:
            self.update_camera(azimuth=0.1)
            #print self.get_camera().parameters()
        
def run_animation():
    animation = BallAnimation(640, 480)
    animation.run(delay=20, framerate=60, save_to='ball2')


if __name__ == '__main__':
    run_animation()

