import sys
import re
import random
sys.path.append('../')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.point import Point

N = 5


class BounceBlocksAnimation(Animation):
    
    def initialize(self):
        self.parse_trajectory()
        
        parser = GeometryParser.from_VTK('data/bounce_blocks/bounce_blocks.vtk')
        self.geometry = parser.parse()
        self.polys = list()
        
        for pid, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            if pid % 17 == 0:
                self.add_object(polyhedron, color=(1,0,0))
            else:
                self.add_object(polyhedron, opacity=0.1)
            
        self.update_camera(focalpoint=[0,-30,120],
                           distance=800,
                           azimuth=0,
                           elevation=90,
                           roll=-90)

    def parse_trajectory(self):
        self.balls = list()
        self.points = list()
        self.times = list()
        self.trajectory = list()
        self.point_idx = list()
        colors = list()
        
        regex = re.compile('(.*?)[\t\s]+(.*)')

        for i in range(1, N+1):    
            self.points.append(list())
            self.times.append(list())
            color = (random.random(), random.random(), random.random())
            colors.append(color)
                
            with open('data/bounce_blocks/x[%d].dat' % i, 'r') as _file:
                    datax = _file.readlines()
            with open('data/bounce_blocks/y[%d].dat' % i, 'r') as _file:
                    datay = _file.readlines()
            with open('data/bounce_blocks/z[%d].dat' % i, 'r') as _file:
                    dataz = _file.readlines()
                        
            for idx in xrange(len(datax)):
                time, x = map(float, regex.findall(datax[idx])[0])
                y = float(regex.findall(datay[idx])[0][1])
                z = float(regex.findall(dataz[idx])[0][1])
                
                self.points[i-1].append((x,y,z))
                self.times[i-1].append(time)
                
            x0, y0, z0 = self.points[i-1][0]
            ball = Point(x0, y0, z0, 1)
            ball._i = i-1
            self.balls.append(ball)
            color = (1, 1, 1)
            self.add_animated_object(ball, self.animate_ball, color=color)
            self.point_idx.append(1)
        
    def animate_ball(self, ball, frame_no):
        i = ball._i
        x = self.points[i][self.point_idx[i]][0] - self.points[i][self.point_idx[i]-1][0]
        y = self.points[i][self.point_idx[i]][1] - self.points[i][self.point_idx[i]-1][1]
        z = self.points[i][self.point_idx[i]][2] - self.points[i][self.point_idx[i]-1][2]                
        ball.transform(translate=(x,y,z))
        self.point_idx[i] += 1
        if self.point_idx[i] >= len(self.points[i]): StopAnimation()
            
    def on_frame(self, frame_no):
        dist = self.get_camera().parameters()['distance']
        az = self.get_camera().parameters()['azimuth']
        roll = self.get_camera().parameters()['roll']
        
        #print az, roll #self.get_camera().parameters()
            
        if dist > 525:
            self.update_camera(distance=-10)
        elif 88.8 < az < 90:
            self.update_camera(roll=90-roll, azimuth=91-az)
        elif -91.2 < az < -90:
            self.update_camera(roll=-90-roll, azimuth=-89-az)
        else:
            self.update_camera(azimuth=1)
            
        
def run_animation():
    animation = BounceBlocksAnimation(640, 480)
    animation.run(delay=20)#, framerate=30, save_to='bounce_blocks')


if __name__ == '__main__':
    run_animation()

