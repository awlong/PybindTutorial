#include <armadillo>
#include "pyarma.hpp"
#include <algorithm>

inline
double bruteforce(arma::mat& a, arma::mat& b)
{
	if(a.n_rows != b.n_rows || a.n_cols != b.n_cols)
		throw std::logic_error("Brute force works only for equal sized graphs");
	
	arma::uvec perm = arma::regspace<arma::uvec>(0,a.n_rows-1);

	double minD = arma::datum::inf;
	do
	{
		double d = arma::norm(a - b(perm,perm),"fro");
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

