#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/list.hpp>
#include <boost/python/iterator.hpp>
 
using namespace std;
namespace py = boost::python;

int pixels[216000];

//https://gist.github.com/octavifs/5362272
py::list toPythonList(vector<int> vec) {
	typename vector<int>::iterator iter;
	py::list list;
	for (iter = vec.begin(); iter != vec.end(); ++iter) {
		list.append(*iter);
	}
	return list;
}

py::list run() {
	vector<int> v(begin(pixels), end(pixels));

	py::list plist = toPythonList(v);

	return plist;
}

BOOST_PYTHON_MODULE(lst) {
    py::def("run", run);
}
