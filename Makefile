install:
	# Setup environment
	mkdir environment

	# Setup Python
	sudo apt-get install python-dev python-scipy python-numpy 
	sudo pip install gym keras theano

	# Setup misc
	sudo apt-get install dosbox nvidia-364 git g++

	git clone https://github.com/openai/gym.git environment/gym
	git clone https://github.com/sherjilozair/dqn.git environment/dqn

# compile:
# 	# Boost Libraries
# 	g++ -std=c++11 setup/x11_find_window.cpp -I/usr/include/python3.5 -I/usr/local/include -lboost_python3 -lpython3.5m -o environment/getwindow.so -shared -fPIC -lGL -lGLU -lXrandr -lXext -lX11
# 	g++ -std=c++11 setup/x11_getframe.cpp -I/usr/include/python3.5 -I/usr/local/include -lboost_python3 -lpython3.5m -o environment/getframe.so -shared -fPIC -lGL -lGLU -lXrandr -lXext -lX11
# 	g++ -std=c++11 setup/x11_send_event.cpp -I/usr/include/python3.5 -I/usr/local/include -lboost_python3 -lpython3.5m -o environment/sendpress.so -shared -fPIC -lGL -lGLU -lXrandr -lXext -lX11
# 	g++ -std=c++11 setup/x11_fast_forward.cpp -I/usr/include/python3.5 -I/usr/local/include -lboost_python3 -lpython3.5m -o environment/fastforward.so -shared -fPIC -lGL -lGLU -lXrandr -lXext -lX11

config:
	cp setup/dosbox-0.74.conf ~/.dosbox/dosbox-0.74.conf
	cp setup/mapper-0.74.map ~/.dosbox/mapper-0.74.map

clean:
	rm -rf environment