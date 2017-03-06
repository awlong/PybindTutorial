import numpy as np
import itertools
import timeit

def py_bruteforce(A,B):
	N = A.shape[0]
	min_d = np.inf
	for perm in itertools.permutations(range(N)):
		Ap = np.take(np.take(A, perm, axis=0), perm, axis=1)
		d = np.linalg.norm(Ap-B,'fro')
		if d < min_d:
			min_d = d
	return min_d

N = 9
setup_str = '''
from __main__ import py_bruteforce
import numpy as np
import itertools
N=%d
A = np.random.rand(N,N)
B = np.random.rand(N,N)
''' % N 
print setup_str

Ntrials = 10
total_time = timeit.timeit('py_bruteforce(A,B)',setup=setup_str,number=Ntrials)
print total_time/Ntrials
