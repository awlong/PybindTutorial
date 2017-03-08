# Binding C++ objects to Python

## Simple example

Let's define a basic class with 3 properties, a public `id`, a public `rw_name` and a private `pr_name`.

```
class Foo
{
public:
	Foo(const int &id) : id(id) {
		rw_name = "rwID:" + std::to_string(id);
		pr_name = "prID:" + std::to_string(id);
	}
	
	void setPrivateName(std::string name) { pr_name = name; }
	std::string getPrivateName() { return pr_name; }

	std::string to_string() { ... }
	
	int id;
	std::string rw_name;
private:
	std::string pr_name;
};
```

We can bind a class object to a Python object using the following pybind11 code

```
PYBIND11_PLUGIN(example)
{
	py::module m("example","...");
	...
	py::class_<Foo> foo(m,"Foo");
	foo.def(py::init<const int&>());
	foo.def("__repr__",
		[](const Foo &f) {
			return "<example.Foo with id '" + std::to_string(f.id) + "'>";
		}
	);
	
	foo.def_readwrite("id",&Foo::id);
	foo.def_readwrite("rw_name",&Foo::rw_name);
	foo.def_property("pr_name",&Foo::getPrivateName,&Foo::setPrivateName);
	foo.def("to_string",&Foo::to_string);
	...
	return m.ptr();
}
```

There's a lot to go over here:

* `py::class_<Foo> foo(m,"Foo")` allows us to define a class object in pybind as part of our module `m` with the global name `Foo` (we declare the type for pybind via the template `<Foo>` and give it the name in the argument list.
* `py::init` serves to handle constructor wrapping, here we pass it the types expected for the constructor of `Foo`
* `foo.def("__repr__",...` shows 2 separate things.
	* The "__repr__" function provides a string to be printed when an object is called by the interpretor
	* We can bind lambda functions similar to normal functions
* `foo.def_readwrite` allows us to give direct access to setting/getting a **public** field of the class
* `foo.def_property` allows us to define functions for accessing/mutating a **private** field (we could also individually `def` these but we lose direct access to the variable)

Here's an example run of this code

```
python
>> import example
>> a = example.Foo(2)
>> a
<example.Foo with id '2'>
>> a.to_string()
ID:2 RW_Name:rwID:2 PR_Name:prID:2
>> a.rw_name = 'public'
>> a.pr_name = 'private'
>> a.id = 100
>> print a.to_string()
ID:100 RW_Name:public PR_Name:private
```

## Object inheritance 

Often when we start thinking about OOP, we're interested in using inheritance to reuse code and extend interfaces. Pybind again provides a fairly straightforward means to do object inheritance. Let's define a subclass of our `Foo` class

```
class Bar : public Foo
{
public:
	Bar(const int &id) : Foo(id) { }
	int sq_id() { return id*id; }
};

PYBIND11_PLUGIN(example)
{
	py::module m("example","...");
	...
	py::class_<Foo> foo(m,"Foo");
	...
	py::class_<Bar> bar(m,"Bar",foo);
	bar.def(py::init<const int &>());
	bar.def("__repr__",
		[](const Bar &b) {
			return "<example.Bar with id '" + std::to_string(b.id) + "'>";
		}
	);
	bar.def("sq_id",&Bar::sq_id);
	...
	return m.ptr();
}
```

* `py::class_<Bar> bar(m,"Bar",foo)` - here we've passed our `foo` class object to the constructor of the `bar` class object, which allows pybind to handle the class inheritance
* We can overwrite `def` of our parent class so that different behavior can happen.

with following runtime example:

```
python
>> import example
>> a = example.Foo(1)
>> a
<example.Foo with id '1'>
>> b = example.Bar(2)
>> b
<example.Bar with id '2'>
>> print b.to_string()
ID:2 RW_Name:rwID:2 PR_Name:prID:2
>> b.sq_id()
4L
```

## Virtual methods/inheritance

Virtual methods and inheritance are slightly more complicated and out of the scope of what I have time to talk about today. There's quite a bit of documentation though on this provided at [http://pybind11.readthedocs.io/en/master/advanced/classes.html](http://pybind11.readthedocs.io/en/master/advanced/classes.html)