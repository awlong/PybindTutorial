#pragma once
#include <armadillo>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

arma::mat& mat_py_to_c(py::array& b)
{
	py::buffer_info info = b.request();
	if(info.format != py::format_descriptor<arma::mat::elem_type>::format())
		throw std::runtime_error("incompatible buffer format");
	
	arma::mat m;
	if(info.ndim == 1) {
	// Vector
		new (&m) arma::mat(reinterpret_cast<arma::mat::elem_type*>(info.ptr),info.shape[0],1);
	} else if((info.strides[0] == info.itemsize) && (info.strides[1] == (info.itemsize*info.shape[0]))) {
	// R-contiguous
		new (&m) arma::mat(reinterpret_cast<arma::mat::elem_type*>(info.ptr),info.shape[0],info.shape[1]);
	} else if((info.strides[1] == info.itemsize) && (info.strides[0] == (info.itemsize*info.shape[1]))) {
	// C-contiguous
		new (&m) arma::mat(reinterpret_cast<arma::mat::elem_type*>(info.ptr),info.shape[1],info.shape[0]);
		arma::inplace_trans(m);
	} else {
		throw std::runtime_error("array not contiguous");
	}
	return m;
}

py::array_t<double> mat_c_to_py(arma::mat &amat)
{
	py::array_t<double> pmat = py::array_t<double>(num_elems);
	py::buffer_info pmat_buffer = pmat.request()
	
	printf("Format:%s\tstride=[%d,%d]\tshape=[%d,%d]\n",
			pmat_buffer.format,
			pmat_buffer.strides[0],
			pmat_buffer.strides[0],
			pmat_buffer.shape[0],
			pmat_buffer.shape[1]);
	//double* ptr = (double*)pmat_buffer.ptr;
	//memcpy(ptr, amat.memptr(), sizeof(double)*num_elems);
	return pmat;
}
