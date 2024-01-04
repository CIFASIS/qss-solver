import sys
sys.path.append('../')

from mlab_tools.animation import Animation, StopAnimation
from mlab_tools.geometry import GeometryParser
from mlab_tools.polyline import AnimatedPolyLine


class CubeMeshAnimation(Animation):
    
    """This example shows how to use a VTK geometry file to render a 3D polyhedron mesh."""
    
    def initialize(self):
        self.parse_volumes()
        self.parse_trajectory()
        
        parser = GeometryParser.from_VTK('data/cube.vtk')
        self.geometry = parser.parse()
        self.polys = list()
        
        for _, polyhedron in self.geometry:
            self.polys.append(polyhedron)
            self.add_object(polyhedron, opacity=0.1)
            
        self.update_camera(focalpoint=[0,-30,120],
                           distance=2700,
                           azimuth=0,
                           elevation=90,
                           roll=-90)
            
    def parse_volumes(self):
        self.volumes0 = list()
        self.volumes1 = list()
        
        with open('data/helix_2/retQSS_volumes-0', 'r') as _file:
            lines = _file.readlines()
            for line in lines:
                fields = line.split()
                time = float(fields[0])
                volume = fields[1]
                self.volumes0.append((time, volume))
        with open('data/helix_2/retQSS_volumes-1', 'r') as _file:
            lines = _file.readlines()
            for line in lines:
                fields = line.split()
                time = float(fields[0])
                volume = fields[1]
                self.volumes1.append((time, volume))
                
        self.current_volume_idx0 = 0
        self.last_polyhedron0 = None
        self.current_volume_idx1 = 0
        self.last_polyhedron1 = None
                
    def parse_trajectory(self):
        points0 = list()
        points1 = list()
        self.times0 = list()
        self.times1 = list()
        
        with open('data/helix_2/retQSS_trajectory-0', 'r') as _file:
            lines = _file.readlines()
            for line in lines:
                time, x, y, z = map(float, line.split()[:-1])
                points0.append((x,y,z))
                self.times0.append(time)
        with open('data/helix_2/retQSS_trajectory-1', 'r') as _file:
            lines = _file.readlines()
            for line in lines:
                time, x, y, z = map(float, line.split()[:-1])
                points1.append((x,y,z))
                self.times1.append(time)
                
        self.trajectory0 = AnimatedPolyLine(points0)
        self.add_object(self.trajectory0, color=(0,0,1))
                        
        self.trajectory1 = AnimatedPolyLine(points1)
        self.add_object(self.trajectory1, color=(0,1,0))                
            
    def on_frame(self, frame_no):
        current_point_idx0 = self.trajectory0.current_point()
        time0 = self.times0[current_point_idx0]
        current_point_idx1 = self.trajectory1.current_point()
        time1 = self.times1[current_point_idx1]
        
        for poly in self.polys:
            poly.transform(rotate=(0,0,0.3))
        self.trajectory0.transform(rotate=(0,0,0.3))
        self.trajectory1.transform(rotate=(0,0,0.3))
        
        dist = self.get_camera().parameters()['distance']
        if dist > 525:
            self.update_camera(distance=-10)
        
        while self.current_volume_idx0 < len(self.volumes0) and\
              time0 >= self.volumes0[self.current_volume_idx0][0]:
            if self.last_polyhedron0 is not None:
                self.last_polyhedron0.update_properties(color=(0.5,0,0), opacity=0.05)
                
            volume_name = self.volumes0[self.current_volume_idx0][1]
            #print 'Entering volume {}...'.format(volume_name)
            
            polyhedron = self.geometry.get_polyhedron(volume_name)
            polyhedron.update_properties(color=(0.7,0,0), opacity=0.3)
            self.last_polyhedron0 = polyhedron
            
            self.current_volume_idx0 += 1
            
        while self.current_volume_idx1 < len(self.volumes1) and\
              time1 >= self.volumes1[self.current_volume_idx1][0]:
            if self.last_polyhedron1 is not None:
                self.last_polyhedron1.update_properties(color=(0.5,0,0), opacity=0.05)
                
            volume_name = self.volumes1[self.current_volume_idx1][1]
            #print 'Entering volume {}...'.format(volume_name)
            
            polyhedron = self.geometry.get_polyhedron(volume_name)
            polyhedron.update_properties(color=(0.7,0,0), opacity=0.3)
            self.last_polyhedron1 = polyhedron
            
            self.current_volume_idx1 += 1
            
        if current_point_idx0 == self.trajectory0.num_points()-1:
            StopAnimation()
        if current_point_idx1 == self.trajectory1.num_points()-1:
            StopAnimation()
        

def run_animation():
    animation = CubeMeshAnimation(640, 480)
    animation.run(delay=20, save_to='cube')


if __name__ == '__main__':
    run_animation()

