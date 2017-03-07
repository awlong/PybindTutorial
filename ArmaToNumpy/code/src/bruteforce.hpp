#include <armadillo>
#include "pyarma.hpp"
#include <algorithm>

inline
double bruteforce(arma::mat& A, arma::mat& B) {
	arma::uvec perm = arma::regspace<arma::uvec>(0,A.n_rows-1);
	double minD = arma::datum::inf;
	do {
		double d = arma::norm(A - B(perm,perm),"fro");
		if(d < minD) {
			minD = d;
		}
	}while(std::next_permutation(perm.begin(),perm.end()));
	return minD;
}

inline
double bruteforce(py::array a, py::array b)
{
	arma::mat A = mat_py_to_c(a);
	arma::mat B = mat_py_to_c(b);
	return bruteforce(A, B);
}

