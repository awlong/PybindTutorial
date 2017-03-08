# Binding functions from C++ to Python

## Simple example

pybind11 provides a number of macros to ease the process of binding between C++ and Python. Here's a simple function example:

```
#include <pybind11/pybind11.h>
int foo(int i, int j)
{
	return i + j;
}

PYBIND11_PLUGIN(example)
{
	py::module m("example", "C++ example library with pybind11");
	m.def("foo", &foo, "A simple function example");
	
	return m.ptr();
}
```

Build a shared library from this code (I use cmake and have provided a CMakeLists.txt file for each tutorial dir)
```c++ -O3 -shared -std=c++11 -I <path-to-pybind11>/include `python-config --cflags --ldflags` example.cpp -o example.so```

and we can call our code from within Python as:

```
python
>> import example
>> help(example)
Help on module example:

NAME
    example - C++ example library with pybind11

FILE
    /home/andrewlong/Workspace/PybindTutorial/FunctionBinding/example.so

FUNCTIONS
    foo(...)
        foo(arg0: int, arg1: int) -> int
>> example.foo(1,1)
2L
```
### What's happening here:

`PYBIND11_PLUGIN()` is a macro that creates the function used by Python's `import` statement.

`py::module m(modname,docstring)` creates a module inside of the library.

`module::def()` call generates the code to expose the function to Python.

`return m.ptr()` returns the internal Python object describing this module to the interpreter.

Not too complicated huh?

## Function arguments and overloading

Often when using a function in Python, the arguments to a function will have a name that you can set (for instance `numpy.array([],dtype=float)`). We can expose these variables in the same way with pybind by modifying our `module::def()` line with the `py::arg` tags.

### Keyword arguments
```
m.def("foo", &foo, py::arg("i"), py::arg("j"));
```

We can then call it in the same way as above:

```
python
>> import example
>> example.foo(i=1,j=3)
4L
```

### Default arguments
Default arguments can be done in a similar way:

```
#include <pybind11/pybind11.h>
int foo(int i=1, int j=3)
{
	return i + j;
}

PYBIND11_PLUGIN(example)
{
	py::module m("example", "C++ example library with pybind11");
	m.def("foo", &foo, py::arg("i")=1, py::arg("j")=3);
	
	return m.ptr();
}
```

yielding

```
python
>> import example
>> example.foo()
4L
>> example.foo(j=10)
11L
```

### Overloading functions

C/C++ doesn't have the dynamic typing that Python has, which can lead to some significant problems in this case:

```
>>> example.foo(1.1,1.2)
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: foo(): incompatible function arguments. The following argument types are supported:
    1. (i: int=1L, j: int=3L) -> int
```

To combat this problem, we may need to overload our C++ functions to handle these different types. Overloading is equally easy to work with in pybind11:

```
#include <pybind11/pybind11.h>
int foo(int i=1, int j=3)
{
	return i + j;
}

float foo(float i, float j)
{
	return i + j;
}

PYBIND11_PLUGIN(example)
{
	py::module m("example", "C++ example library with pybind11");
	m.def("foo", (int(*)(int,int))&foo, py::arg("i")=1, py::arg("j")=3);
	m.def("foo", (float(*)(float, float))&foo, py::arg("i"), py::arg("j"));
	
	return m.ptr();
}
```

Here we cast each reference `&foo` as it's corresponding function pointer `(int(*)(int,int))` or `(float(*)(float,float))` to ensure that the compiler knows which function to call for these different cases.

```
python
>> import example
>> example.foo(1.1,1.2)
2.3000001907348633
>> example.foo(1,1)
2L
```

## Using Python types as arguments

If all we could do was pass along `int/float/string` variables, this wouldn't be super useful still. Pybind11 includes automatic binding between Python's `list/set/dict/tuple` types to STL containers in C++11. There's a couple different ways we can do it:

### Automated casting of variables
In C++ I'm used to std::vector and std::map, and pybind11 allows us to seamlessly handle the conversion:

```
void print_vector(const std::vector<int> &vec) {
	for(auto item : vec)
		std::cout << item << " ";
}
void print_dict(const std::map<std::string, int> &dict) {
	for(auto item : dict)
		std::cout << "key=" << item.first << ","
				  << "value=" << item.second << std::endl;
}
PYBIND11_PLUGIN(example)
{
	...
	m.def("print_vector", &print_vector);
	m.def("print_dict", &print_dict);
	...
}
```

```
python
>> import example
>> example.print_vector([1,2,5,6])
1 2 5 6
>> example.print_dict({'A':1, 'B':4, 'FF':2})
key=A,value=1
key=B,value=4
key=FF,value=2
```

I find this to be pretty simple for almost all of my applications.

### Operating on native Python objects
Pybind11 exposes the `py::list/py::set/py::dict` directly to C++ as well. Again, pybind11 tries to make this as straight forward as possible.

```
void print_list(py::list list) {
	for(auto item : list)
		std::cout << item << " ";
}
void print_dict_py(py::dict dict) {
	for(auto item : dict)
		std::cout << "key=" << std::string(py::str(item.first)) << ","
				  << "value=" << std::string(py::str(item.second)) << std::endl;
}
PYBIND11_PLUGIN(example)
{
	...
	m.def("print_list", &print_list);
	m.def("print_dict_py", &print_dict_py);
	...
}
```

```
python
>> import example
>> example.print_list([1,2,3.4,'tmp'])
1 2 3.5 tmp
>> example.print_dict_py({'A':1, 'B':4, 'FF':2})
key=A,value=1
key=B,value=4
key=FF,value=2
```

