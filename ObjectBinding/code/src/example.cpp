#include <iostream>
#include <string>
#include <cstdio>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py=pybind11;

class Foo
{
	public:
		Foo(const int &id) : id(id) { 
			readwrite_name = "rwID:" + std::to_string(id);
			private_name = "prID:" + std::to_string(id);
		}
		
		void setPrivateName(std::string name) {
			private_name = name;
		}
		std::string getPrivateName() {
			return private_name;
		}
		std::string to_string() {
			std::string str = "ID:" + std::to_string(id) +
									" RW_Name:" + readwrite_name +
									" PR_Name:" + private_name;
			return str;
		}
		int id;
		std::string readwrite_name;
	private:
		std::string private_name;
};

class Bar : public Foo
{
public:
	Bar(const int &id) : Foo(id) { }
	int sq_id() {
		return id*id;
	}
};

PYBIND11_PLUGIN(example)
{
	py::module m("example", "C++ example library with pybind11");
	
	py::class_<Foo> foo(m,"Foo");
	foo.def(py::init<const int &>());
	foo.def("__repr__", 
		[](const Foo &f) {
			return "<example.Foo with id '" + std::to_string(f.id) + "'>";
		}
	);
	foo.def_readwrite("id",&Foo::id);
	foo.def_readwrite("rw_name", &Foo::readwrite_name);
	foo.def_property("pr_name", &Foo::getPrivateName, &Foo::setPrivateName);
	foo.def("to_string",&Foo::to_string);

	py::class_<Bar> bar(m,"Bar",foo);
	bar.def(py::init<const int &>());
	bar.def("__repr__",
		[](const Bar &b) {
			return "<example.Bar with id '" + std::to_string(b.id) + "'>";
		}
	);
	bar.def("sq_id",&Bar::sq_id);

	return m.ptr();
}
