import ctypes
import cv2
import numpy as np 
import time

update = ctypes.CDLL('./sf_frame_lib.so').update_frame
init = ctypes.CDLL('./sf_frame_lib.so').start_drawing
close = ctypes.CDLL('./sf_frame_lib.so').stop_drawing
n_bytes = 153600 # The stride of the surface times the height of the image from DE.c
update.restype = ctypes.POINTER(ctypes.c_ubyte * n_bytes) # c_ubyte is equal to unsigned char

init()

i = 0
t_end = time.time() + 10
while time.time() < t_end:
	new_frame = update().contents
	img = np.ctypeslib.as_array(new_frame)
#	img = list(new_frame)
#	img = np.array(img)
	img = np.reshape(img, (240,320,2)) # Don't ask me why this works, has to do with 16bit RGB
#	img = np.delete(img, 3, 2) # Don't do this, really show and not needed for the grayscale conv
	gray_image = cv2.cvtColor(img, cv2.COLOR_BGR5652GRAY) # use this for 16 bit RGB
	i += 1
print(i/10) 

#	img = np.ctypeslib.as_array(new_frame)
#	img = np.reshape(img, (240,320,4)) 
#	img = np.delete(img, 3, 2) # Delete the alpha component 
#	cv2.imwrite('./test_images/test' + str(i) + '.png', img)

close()
