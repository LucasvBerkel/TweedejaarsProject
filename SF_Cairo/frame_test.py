import ctypes
import cv2
import numpy as np 
import time
from random import choice
from random import randint

#  A list of GTK hex key values as decimals
#	GDK_KEY_1  49  // Should be 1,2 and 3
#	GDK_KEY_2  65471
#	GDK_KEY_3  65472
#	GDK_KEY_Left  65361
#	GDK_KEY_Up  65362
#	GDK_KEY_space  32 
# RIGHT 				65362

moves = [65361, 65362, 32, 65363]
#moves = [65361, 32, 65363]

# Maybe create a seperate SF update version?
update = ctypes.CDLL('./sf_frame_lib.so').update_frame_SF
init = ctypes.CDLL('./sf_frame_lib.so').start_drawing
close = ctypes.CDLL('./sf_frame_lib.so').stop_drawing
act = ctypes.CDLL('./sf_frame_lib.so').set_key
reset = ctypes.CDLL('./sf_frame_lib.so').reset_sf
# was 153600, should be extactly or around 120000
scale = 1
n_bytes = (448//scale)*(468//scale)*1 # The stride of the surface times the height of the image from DE.c 
update.restype = ctypes.POINTER(ctypes.c_ubyte * n_bytes) # c_ubyte is equal to unsigned char
init()

i = 0
seconds = 1
t_end = time.time() + seconds
while time.time() < t_end:
	new_frame = update().contents
	act(choice(moves)) # Get a random move
	img = np.ctypeslib.as_array(new_frame)
#	img.flatten()
	# Only needed for saving as an image, providing a 1d vector to the network is also okay
	img = np.reshape(img, (468//scale, 448//scale)) # Don't ask me why this works, has to do with 16bit RGB
#	print(img)	
#	img = np.delete(img, 3, 2) # Don't do this, really slow and not needed for the grayscale conv
#	gray_image = cv2.cvtColor(img, cv2.COLOR_BGR5552GRAY) # use this for 16 bit RGB
#	img  = cv2.cvtColor(img, cv2.COLOR_BGR5652RGB)
#	img  = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
	cv2.imwrite('./test_images/test' + str(i) + '.png', img)
	i += 1
print(i/seconds) 

#	img = np.ctypeslib.as_array(new_frame)
#	img = np.reshape(img, (240,320,4)) 
#	img = np.delete(img, 3, 2) # Delete the alpha component 
#	cv2.imwrite('./test_images/test' + str(i) + '.png', img)

close()
