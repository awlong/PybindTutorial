# Binding Numpy to Armadillo

## [Numpy](http://www.numpy.org/)


## [Armadillo](http://arma.sourceforge.net/)
Armadillo is a C++11 header-only library for linear algebra. It has been developed to have a syntax very close to MATLAB in an attempt to make the transition easier. More importantly, it's been developed for speed. Delayed evaluation attempts to combine several operations to improve performance by reducing redundancy/temporaries. The goal of this library was to improve the conversion of research/prototypes codes into production.

Matrix types are defined by the `arma::mat`, while vectors are `arma::vec`. These default types are `double` precision, though alternatives exist for `float, int, uint, complex`.

## Armadillo Primer

## Example from my research

In my research, I focus on understanding the differences between particulate aggregates, with the goal of designing particles that aggregate into certain shapes. One of our prototypical systems is a hollow icosahedron formed from a species of patchy particles. To compute similarities between aggregates we abstract the particles/bonding between particles to a bonding network and use graph matching to select a minimum permutation between graph structures.

<div style="text-align:center">
<img src="cluster_to_graph.png" width="250" display="block">
</div>

The process of finding minimum permutations is *NP*-hard, as we must solve over all *N!* unique labelings. Now in my research I choose to find a pseudo-optimal permutation because **12!=479,001,600** and I want to compute pairwise distances between **~10^4** graphs (that's **2.4x10^16** permutation evaluations), requiring some greedy heuristics. For this example though, let's look at a case study on brute force graph matching.

Here's an example of Python brute force matching code:
![](py_bruteforce.png)

Here it is in C++ with Armadillo:
![](cpp_bruteforce.png)

The idea is the same, let's iterate over all possible permutations and check for the minimum Frobenius norm (elementwise 2-norm). Let's test these codes using Python's timeit function.

![](N=6_testing.png)
That's an **80-fold speedup** by switching to C++! And this is for 6-node graphs (remember I want 12-nodes for an icosahedron), as we go up the speedup goes down a bit as the time per iteration continues to rise. For 8-nodes:

![](N=8_testing.png)
which is still a **64-fold speedup**. This becomes extremely problematic again when we look at sizes of up to N=12 graphs...

![](N=12_testing.png)