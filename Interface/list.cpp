#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/list.hpp>
#include <boost/python/iterator.hpp>
 
#define LENGTH 216000

using namespace std;
namespace py = boost::python;

int pixels[LENGTH];

//https://gist.github.com/octavifs/5362272
py::list vec_to_list(vector<int> vec) {
	typename vector<int>::iterator iter;
	py::list list;
	for (iter = vec.begin(); iter != vec.end(); ++iter) {
		list.append(*iter);
	}
	return list;
}

py::list run() {
    py::list a;
    // a.append(0);
    // a *= LENGTH;
    for (int i = 0; i < LENGTH; ++i) {
        a.append(pixels[i]);
    }
    return a;
}

BOOST_PYTHON_MODULE(lst) {
    py::def("run", run);
}
