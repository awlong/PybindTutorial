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
double bruteforce(pyarr_d a, pyarr_d b) {
	arma::mat A = py_to_mat(a);
	arma::mat B = py_to_mat(b);
	return bruteforce(A, B);
}

inline
arma::uvec bruteforce_perm(arma::mat &A, arma::mat &B) {
	arma::uvec perm = arma::regspace<arma::uvec>(0,A.n_rows-1);
	arma::uvec minPerm(A.n_rows);
	double minD = arma::datum::inf;
	do {
		double d = arma::norm(A - B(perm,perm),"fro");
		if(d < minD) {
			minD = d;
			minPerm = perm;
		}
	}while(std::next_permutation(perm.begin(),perm.end()));
	return minPerm;
}

inline
py::array bruteforce_perm(pyarr_d a, pyarr_d b) {
	arma::mat A = py_to_mat(a);
	arma::mat B = py_to_mat(b);
	arma::uvec perm = bruteforce_perm(A,B);
	return uvec_to_py(perm);
}
