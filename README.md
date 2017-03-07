# Binding C++ and Python
========================
## Topics:

* Function binding
* Object oriented code
* Numpy to Armadillo binding

## Why you might be interested ##

Everyone loves Python, but there's some nice things about working in C/C++:

1. Access to memory management directly (no wondering about garbage collection)
2. Compilation and typing allow for greater speed/optimization than interpreted code

But Python is extremely useful for almost all fields and there are already a huge set of libraries out there to improve our prototyping and development timeline. Because of this, we'd like to leverage the benefits of both languages but interoperability requires some work.

## Options out there to consider##
* [pybind11](http://pybind11.readthedocs.io/en/master/intro.html) - "lightweight header-only C++ library" to expose functions/data types between languages
* [Cython](http://cython.org/) - write "Python" code and compile it as an extension module (compiler translates from Cython code to C/C++)
* [SciPy.weave](https://docs.scipy.org/doc/scipy-0.18.1/reference/tutorial/weave.html#introduction) - offers inline C/C++ code execution directly in Python, and compiled numpy calls

```
import scipy.weave
scipy.weave.inline('printf("Hello World");')

creating /tmp/scipy-awlong2-7hS9Hn/...
Hello World
```

* [Boost.python](http://www.boost.org/doc/libs/1_63_0/libs/python/doc/html/index.html) - A mess to learn/bloated but extremely powerful once you get it.


We're going to go over pybind11 as it's the one I'm most familiar with (I started with Boost but moved over as it seemed easier to manage). For simple tasks, scipy.weave.inline() may be the easiest option as you can directly write up a simple script and just walk away without worrying about compilation.
