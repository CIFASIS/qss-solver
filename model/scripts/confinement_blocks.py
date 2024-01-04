import sys
import re
import random
sys.path.append('../')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.point import Point

N = 50
MESSIAH = 0
PATH = 'data/confinement_blocks'

ROCK_COLOR     = (0,0,0)
LOCKED_COLOR   = (0.95,0.1,0)
UNLOCKED_COLOR = (0.6,0.95,0.6)

FREE_COLOR = (1,1,1)
CONFINED_COLOR = (1,0,0)
MESSIAH_COLOR = (0,0,1)

LOCK_THRESH = 5
LOCK_TIME = 30
TIME_UNIT = 0.1

LOCKED = 0
UNLOCKED = 1


class BounceBlocksAnimation(Animation):
    
    def initialize(self):
        parser = GeometryParser.from_VTK('%s/confinement_blocks.vtk' % PATH)
        self.geometry = parser.parse()
        self.polys = list()
        self.time = 0
        self.next_lock = LOCK_TIME
        
        for pid, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            polyhedron._particles = list()
            polyhedron._status = UNLOCKED
            if pid % 25 == 0:
                self.add_object(polyhedron, color=ROCK_COLOR)
            elif pid % 27 == 0:
                self.add_object(polyhedron, opacity=0.1, color=LOCKED_COLOR)
                polyhedron._status = LOCKED
            else:
                self.add_object(polyhedron, opacity=0.1)
                
        self.parse_trajectory()
        self.parse_volumes()                
            
        self.update_camera(focalpoint=[0,-30,120],
                           distance=800,
                           azimuth=0,
                           elevation=90,
                           roll=-90)
        
    def parse_volumes(self):
        self.volumes = list()
        self.current_vol = list()

        for i in xrange(1, N+1):
            self.volumes.append(list())
            
            with open('%s/volumeID[%d].dat' % (PATH,i), 'r') as _file:
                data = _file.readlines()
                
            for line in data:
                _, pid = line.split('\t')
                self.volumes[i-1].append(int(float(pid)))
                
            first_id = self.volumes[i-1][0]
            polyhedron = self.geometry.get_polyhedron_by_ID(first_id)
            if i-1 != MESSIAH:
                polyhedron._particles.append(i-1)
            self.current_vol.append(first_id)
            
            if first_id % 27 == 0:
                self.balls[i-1].update_properties(color=CONFINED_COLOR)

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
                
            with open('%s/x[%d].dat' % (PATH,i), 'r') as _file:
                    datax = _file.readlines()
            with open('%s/y[%d].dat' % (PATH,i), 'r') as _file:
                    datay = _file.readlines()
            with open('%s/z[%d].dat' % (PATH,i), 'r') as _file:
                    dataz = _file.readlines()
                        
            for idx in xrange(len(datax)):
                time, x = map(float, regex.findall(datax[idx])[0])
                y = float(regex.findall(datay[idx])[0][1])
                z = float(regex.findall(dataz[idx])[0][1])
                
                self.points[i-1].append((x,y,z))
                self.times[i-1].append(time)
                
            x0, y0, z0 = self.points[i-1][0]
            radius = 2 if i-1 == MESSIAH else 1
            color = MESSIAH_COLOR if i-1 == MESSIAH else FREE_COLOR
            ball = Point(x0, y0, z0, radius)
            ball._i = i-1
            self.balls.append(ball)
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
        
        pid = self.volumes[i][frame_no]
        if i == MESSIAH:
            if pid > 0 and pid % 27 == 0:
                print 'Messiah unlocking volume #%d...' % pid
                polyhedron = self.geometry.get_polyhedron_by_ID(pid)
                polyhedron._status = UNLOCKED
                polyhedron.update_properties(opacity=0.1, color=UNLOCKED_COLOR)
                for j in polyhedron._particles:
                    self.balls[j].update_properties(color=FREE_COLOR)
        elif pid != self.current_vol[i]:
            if self.current_vol[i] > 0:
                prev_vol = self.geometry.get_polyhedron_by_ID(self.current_vol[i])
                prev_vol._particles.remove(i)
            if pid > 0:
                current_vol = self.geometry.get_polyhedron_by_ID(pid)
                current_vol._particles.append(i)
                if pid % 27 == 0 and len(current_vol._particles) >= LOCK_THRESH:
                    print 'Lock threshold reached in volume #%d...' % pid
                    current_vol.update_properties(opacity=0.1, color=UNLOCKED_COLOR)
                    current_vol._status = UNLOCKED
                    for j in current_vol._particles:
                        self.balls[j].update_properties(color=FREE_COLOR)
                elif pid % 27 == 0 and current_vol._status == LOCKED:
                    ball.update_properties(color=CONFINED_COLOR)
                elif self.current_vol[i] > 0 and prev_vol._status == LOCKED:
                    print 'Warning: leaving locked volume!'
                    prev_vol._status = UNLOCKED
                    prev_vol.update_properties(opacity=0.1, color=UNLOCKED_COLOR)
                    ball.update_properties(color=FREE_COLOR)
                    for j in prev_vol._particles:
                        self.balls[j].update_properties(color=FREE_COLOR)                    
            self.current_vol[i] = pid 
            
    def on_frame(self, frame_no):
        self.time += TIME_UNIT
        print 'Time: %.1f' % self.time
        
        dist = self.get_camera().parameters()['distance']
        az = self.get_camera().parameters()['azimuth']
        roll = self.get_camera().parameters()['roll']
        
        if dist > 525:
            self.update_camera(distance=-10)
        elif 88.8 < az < 90:
            self.update_camera(roll=90-roll, azimuth=91-az)
        elif -91.2 < az < -90:
            self.update_camera(roll=-90-roll, azimuth=-89-az)
        else:
            self.update_camera(azimuth=1)
            
        if self.time >= self.next_lock:
            print 'Locking...'
            pid = 27
            while pid <= self.geometry.num_polyhedrons():
                polyhedron = self.geometry.get_polyhedron_by_ID(pid)
                polyhedron._status = LOCKED
                polyhedron.update_properties(opacity=0.1, color=LOCKED_COLOR)
                for i in polyhedron._particles:
                    self.balls[i].update_properties(color=CONFINED_COLOR)                
                pid += 27
            self.next_lock += LOCK_TIME
            print 'Next lock at %.1f' % self.next_lock
            
        
def run_animation():
    animation = BounceBlocksAnimation(640, 480)
    animation.run(delay=20, framerate=30, save_to='confinement_blocks')


if __name__ == '__main__':
    run_animation()

