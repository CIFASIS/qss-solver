from PIL import Image
import cv2
import os

alpha = 0.4

width = 360
height = 430
framerate = 25

frame_template = 'boids_hand_OK_2.vtk_%d.png' 
hand_filename = 'hand.png'

video = cv2.VideoWriter('hand.avi', cv2.cv.CV_FOURCC(*'XVID'), framerate,  (width, height))

idx = 1

hand = Image.open(hand_filename)



while True:
    image = frame_template % idx
    try:
        x = open(image, 'r')
        x.close()
    except Exception:
        break
        
    frame = Image.open(image)
    new_frame = Image.blend(frame, hand, alpha=alpha)
    new_frame.save('newframe_%d.png' % idx) 
    
    idx += 1
    
for i in xrange(1, idx):    
    new_frame = cv2.imread('newframe_%d.png' % i)
    video.write(new_frame)
    
video.release()

os.system('ffmpeg -i hand.avi hand.mp4')

os.system('rm -rf newframe*') 