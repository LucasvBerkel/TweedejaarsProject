# Compile with:
# g++ greet_binding.cpp -I/usr/include/python3.5 -I/usr/local/include -lboost_python3 -lpython3.5m -o greet.so -shared -fPIC
# Run with:
# LD_LIBRARY_PATH=/usr/local/lib python3.5 ./hello.py 

import lst, numpy, time

start = time.time()

for i in range(1,100000):
	a = lst.run()
	b = numpy.asarray(a)
	print('\rFrame %d | Average: %fFPS' % (i, 1/((time.time()-start)/i)), end="")
	
print()
