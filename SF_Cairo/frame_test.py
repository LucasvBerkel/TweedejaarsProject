import ctypes
import cv2
import numpy as np 
import time

# Maybe create a seperate SF update version?
update = ctypes.CDLL('./sf_frame_lib.so').update_frame_SF
init = ctypes.CDLL('./sf_frame_lib.so').start_drawing
close = ctypes.CDLL('./sf_frame_lib.so').stop_drawing
n_bytes = 153600 # The stride of the surface times the height of the image from DE.c
update.restype = ctypes.POINTER(ctypes.c_ubyte * n_bytes) # c_ubyte is equal to unsigned char

init()

i = 0
seconds = 1
t_end = time.time() + seconds
while time.time() < t_end:
	new_frame = update().contents
	img = np.ctypeslib.as_array(new_frame)
	img = np.reshape(img, (240,320,2)) # Don't ask me why this works, has to do with 16bit RGB
#	img = np.delete(img, 3, 2) # Don't do this, really slow and not needed for the grayscale conv
	gray_image = cv2.cvtColor(img, cv2.COLOR_BGR5552GRAY) # use this for 16 bit RGB
	cv2.imwrite('./test_images/test' + str(i) + '.png', gray_image)
	i += 1
print(i/seconds) 

#	img = np.ctypeslib.as_array(new_frame)
#	img = np.reshape(img, (240,320,4)) 
#	img = np.delete(img, 3, 2) # Delete the alpha component 
#	cv2.imwrite('./test_images/test' + str(i) + '.png', img)

close()
