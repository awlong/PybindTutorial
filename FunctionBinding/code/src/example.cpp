#include <vector>
#include <iostream>
#include <string>
#include <cstdio>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py=pybind11;

int foo(int i=1, int j=3)
{
	return i + j;
}

float foo(float i, float j)
{
	return i + j;
}

void print_vector(const std::vector<int> &vec) {
	for(auto item : vec)
		std::cout << item << " ";
}

void print_list(py::list list) {
	for(auto item : list)
		std::cout << item << " ";
}


void print_dict(const std::map<std::string,int> &dict) {
	for(auto item : dict)
		std::cout << "key=" << item.first << ","
					 << "value=" << item.second << std::endl;
}

void print_dict_py(py::dict dict) {
	for(auto item : dict)
		std::cout << "key=" << std::string(py::str(item.first)) << ","
					 << "value=" << std::string(py::str(item.second)) << std::endl;
}

PYBIND11_PLUGIN(example)
{
	py::module m("example", "C++ example library with pybind11");
	m.def("foo", (int(*)(int,int))&foo, py::arg("i") = 1, py::arg("j") = 3);
	m.def("foo", (float(*)(float,float))&foo, py::arg("i"), py::arg("j"));
	m.def("print_vector",&print_vector);
	m.def("print_list",&print_list);
	m.def("print_dict",&print_dict);
	m.def("print_dict_py",&print_dict_py);
	return m.ptr();
}
