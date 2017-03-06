import numpy as np
import itertools
import timeit
from cpp_bfmatch import bruteforce as cpp_bruteforce

def py_bruteforce(A,B):
	N = A.shape[0]
	min_d = np.inf
	for perm in itertools.permutations(range(N)):
		Bp = np.take(np.take(B, perm, axis=0), perm, axis=1)
		d = np.linalg.norm(A-Bp,'fro')
		if d < min_d:
			min_d = d
	return min_d

N = 7
Ntrials = 20
setup_str = '''
from __main__ import py_bruteforce
import numpy as np
import itertools
from cpp_bfmatch import bruteforce as cpp_bruteforce
N=%d
A = np.random.rand(N,N)
B = np.random.rand(N,N)
''' % N 
print setup_str

total_time = timeit.timeit('py_bruteforce(A,B)',setup=setup_str,number=Ntrials)
print total_time/Ntrials
total_time = timeit.timeit('cpp_bruteforce(A,B)',setup=setup_str,number=Ntrials)
print total_time/Ntrials

