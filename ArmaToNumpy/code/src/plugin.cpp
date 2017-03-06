#include "pyarma.cpp"
#include "bruteforce.cpp"

PYBIND11_PLUGIN(cpp_bfmatch)
{
	py::module m("cpp_bfmatch", "C++ implementation of brute force match using Armadillo");
	m.def("bruteforce",(double(*)(py::arrat,py::array))&bruteforce);
	return m.ptr();
}
