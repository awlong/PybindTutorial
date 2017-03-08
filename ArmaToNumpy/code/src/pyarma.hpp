#pragma once
#include <armadillo>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py=pybind11;

typedef py::array_t<double, py::array::f_style | py::array::forcecast> pyarr_d;
//py::array_t<double, py::array::c_style | py::array::forcecast>& b)

inline
arma::mat py_to_mat(pyarr_d& pmat)
{
	py::buffer_info info = pmat.request();
	arma::mat amat = arma::mat(reinterpret_cast<arma::mat::elem_type*>(info.ptr),info.shape[0],info.shape[1]);
	return amat;
}

inline 
py::array_t<double> mat_to_py(arma::mat &mat)
{
	py::buffer_info buffer(
		mat.memptr(),
		sizeof(double),
		py::format_descriptor<double>::format(),
		2,
		{ mat.n_rows, mat.n_cols },
		{ sizeof(double), sizeof(double) * mat.n_rows }
	);
	return py::array_t<double>(buffer);	
}

inline
py::array_t<arma::sword> uvec_to_py(arma::uvec &vec)
{
	py::buffer_info buffer(
		vec.memptr(),
		sizeof(arma::sword),
		py::format_descriptor<arma::sword>::format(),
		1,
		{ vec.n_elem },
		{ sizeof(arma::sword) }
	);
	return py::array_t<arma::sword>(buffer);
}
