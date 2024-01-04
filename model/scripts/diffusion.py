import sys
import re
sys.path.append('../')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.point import Point

BALL_Y = 0.5
BALL_Z = 0.5
VOLS = 20
DELTA_T = 5e-3


class DifussionAnimation(Animation):
    
    def initialize(self):
        self.parse_trajectory()
        self.parse_heat()
        
        parser = GeometryParser.from_VTK('data/diffusion/diffusion.vtk')
        self.geometry = parser.parse()
        self.polys = list()
        
        for pid, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            color = self.heat_color(pid, 0)
            self.add_object(polyhedron, opacity=0.1, color=color)
            
        self.update_camera(focalpoint=[12,0.5,0.5],
                           distance=20,
                           azimuth=29.590688121719797,
                           elevation=142.41973867008952,
                           roll=1.4924594399496642)

    def parse_trajectory(self):
        self.times = list()
        self.trajectory = list()
        regex = re.compile('(.*?)[\t\s]+(.*)')

        self.points = list()
        self.times.append(list())
            
        with open('data/diffusion/x.dat', 'r') as _file:
                datax = _file.readlines()
                    
        for idx in xrange(len(datax)):
            time, x = map(float, regex.findall(datax[idx])[0])
            self.points.append((x,BALL_Y,BALL_Z))
            self.times[0].append(time)
            
        x0, y0, z0 = self.points[0]
        self.ball = Point(x0, y0, z0, 0.08)
        self.add_animated_object(self.ball, self.animate_ball, color=(0,0,1))
        self.point_idx = 1
        
    def parse_heat(self):
        self.heat = [[]]
        
        regex = re.compile('(.*?)[\t\s]+(.*)')
        
        for i in xrange(1, VOLS+1):
            self.heat.append(list())
            
            with open('data/diffusion/u_alg[%d].dat' % i, 'r') as _file:
                    data = _file.readlines()
                    
            for idx in xrange(len(data)):
                _, heat = map(float, regex.findall(data[idx])[0])
                heat = (heat + 1) / 2
                self.heat[i].append(heat)      
                
    def heat_color(self, pid, t):
        x = self.heat[pid][t]
        return (x, 1-x, 0)  
        
    def animate_ball(self, ball, frame_no):
        x = self.points[self.point_idx][0] - self.points[self.point_idx-1][0]
        y = self.points[self.point_idx][1] - self.points[self.point_idx-1][1]
        z = self.points[self.point_idx][2] - self.points[self.point_idx-1][2]                
        ball.transform(translate=(x,y,z))
        self.point_idx += 1
        if self.point_idx >= len(self.points): StopAnimation()
            
    def on_frame(self, frame_no):
        for pid, polyhedron in self.geometry:
            color = self.heat_color(pid, frame_no)
            #if pid == 20: print color
            polyhedron.update_properties(color=color, opacity=0.1)

        
def run_animation():
    animation = DifussionAnimation(640, 480)
    animation.run(delay=20, framerate=60, save_to='diffusion')


if __name__ == '__main__':
    run_animation()

