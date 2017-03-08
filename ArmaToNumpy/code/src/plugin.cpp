#include "pyarma.hpp"
#include "bruteforce.hpp"


PYBIND11_PLUGIN(cpp_bfmatch)
{
	py::module m("cpp_bfmatch", "C++ implementation of brute force match using Armadillo");
	m.def("bruteforce",(double(*)(pyarr_d,pyarr_d))&bruteforce);

	m.def("bruteforce_perm",(py::array(*)(pyarr_d,pyarr_d))&bruteforce_perm);
//	py::array_t<double> mat_py_c_py(py::array& b)
	return m.ptr();
}
