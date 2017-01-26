ffi = require("ffi")
torch = require("torch")
image = require("image")
math = require("math")

lib = ffi.load("/Users/rijnderwever/Desktop/NLR/NLR/TweedejaarsProject/SF_Cairo/sf_frame_lib_FULL.so")

-- [[ is a fucking string in lua 
ffi.cdef([[
void start_drawing();
unsigned char* update_screen();
void SF_iteration(); 
void set_key(int key_value);
unsigned char* get_original_screen();
]])

moves = {65361, 65362, 65363}

n_pixels = 448*448*3
lib.start_drawing()

buf = ffi.new("uint8_t[?]", n_pixels)
im = torch.ByteTensor(n_pixels) -- Equal to an uint8 

seconds = 0.58
start = os.clock()
time_end = seconds + start 
i = 0
while os.clock() < time_end do 
	lib.set_key(moves[math.random(#moves)])
	lib.SF_iteration()
	lib.update_screen()
	buf = lib.get_original_screen()
	ffi.copy(im:data(), buf, n_pixels*ffi.sizeof("uint8_t"))
	i = i + 1
	im = torch.reshape(im, 3, 448, 448)
--	print(im)
--	os.execute("sleep 1")

	image.save("test_images/im" .. i .. ".png", im)
end

print(i/seconds)






