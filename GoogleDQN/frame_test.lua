ffi = require("ffi")
torch = require("torch")
image = require("image")
math = require("math")

lib = ffi.load("/Users/rijnderwever/Desktop/NLR/NLR/TweedejaarsProject/gym-master/gym/envs/space_fortress/darwin/control_frame_lib.so")

-- [[ is a fucking string in lua 
ffi.cdef([[
void start_drawing();
unsigned char* update_screen();
void SF_iteration(); 
void set_key(int key_value);
]])

moves = {65361, 65362, 65363}

n_pixels = 84*84
lib.start_drawing()

buf = ffi.new("uint8_t[?]", n_pixels)
im = torch.ByteTensor(n_pixels) -- Equal to an uint8 



seconds = 30
start = os.clock()
time_end = seconds + start 
i = 0
while os.clock() < time_end do 
	lib.set_key(moves[math.random(#moves)])
	lib.SF_iteration()
	buf = lib.update_screen()
	ffi.copy(im:data(), buf, n_pixels*ffi.sizeof("uint8_t"))
	i = i + 1
--	print(im)
--	im = torch.reshape(im, 84, 84)
--	image.save("test_images/im" .. i .. ".png", im)
end

print(i/seconds)






