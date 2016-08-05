Clang disallow the destructor of a templated class as a friend
==============================================================

https://llvm.org/bugs/show_bug.cgi?id=28873

Clang from trunk (4.0.0 r277725) fails to compile the following code
whereas g++ 5.4 does:

template <typename T>
class b {
public:
  ~b() {
  }
};

template <typename T>
class a {
  friend b<T>::~b();
};

int main() {

  a<int> v;

  return 0;
}

clang++ -std=c++14    tfd1.cpp   -o tfd1
tfd1.cpp:10:16: error: expected the class name after '~' to name the enclosing
      class
  friend b<T>::~b();
               ^
tfd1.cpp:16:10: note: in instantiation of template class 'a<int>' requested here
  a<int> v;
         ^
1 error generated.


So I tried different variation of this code that all compile with g++
but not all on Clang. Note that some of them may not be correct C++,
but I do not know.

The Makefile and the samples are attached to test.


make -k CXX=g++
rm -f tfd1 tfd1.1 tfd1.fun tfd2 tfd2.1 tfd2.fun
g++ -std=c++14    tfd1.cpp   -o tfd1
g++ -std=c++14    tfd1.1.cpp   -o tfd1.1
g++ -std=c++14    tfd1.fun.cpp   -o tfd1.fun
g++ -std=c++14    tfd2.cpp   -o tfd2
g++ -std=c++14    tfd2.1.cpp   -o tfd2.1
g++ -std=c++14    tfd2.fun.cpp   -o tfd2.fun


make -k CXX=g++ CXXFLAGS=-std=c++1z
rm -f tfd1 tfd1.1 tfd1.fun tfd2 tfd2.1 tfd2.fun
g++ -std=c++1z    tfd1.cpp   -o tfd1
g++ -std=c++1z    tfd1.1.cpp   -o tfd1.1
g++ -std=c++1z    tfd1.fun.cpp   -o tfd1.fun
g++ -std=c++1z    tfd2.cpp   -o tfd2
g++ -std=c++1z    tfd2.1.cpp   -o tfd2.1
g++ -std=c++1z    tfd2.fun.cpp   -o tfd2.fun


make -k CXX=clang++
clang++ -std=c++14    tfd1.cpp   -o tfd1
tfd1.cpp:10:16: error: expected the class name after '~' to name the enclosing
      class
  friend b<T>::~b();
               ^
tfd1.cpp:16:10: note: in instantiation of template class 'a<int>' requested here
  a<int> v;
         ^
1 error generated.
<builtin>: recipe for target 'tfd1' failed
make: *** [tfd1] Error 1
clang++ -std=c++14    tfd1.1.cpp   -o tfd1.1
tfd1.1.cpp:10:16: error: '~b' is missing exception specification 'noexcept'
  friend b<T>::~b<T>();
               ^
                       noexcept
tfd1.1.cpp:16:10: note: in instantiation of template class 'a<int>' requested
      here
  a<int> v;
         ^
tfd1.1.cpp:4:3: note: previous declaration is here
  ~b() {
  ^
1 error generated.
<builtin>: recipe for target 'tfd1.1' failed
make: *** [tfd1.1] Error 1
clang++ -std=c++14    tfd1.fun.cpp   -o tfd1.fun
clang++ -std=c++14    tfd2.cpp   -o tfd2
tfd2.cpp:11:17: error: expected the class name after '~' to name a destructor
  friend b<U>::~b();
                ^
1 error generated.
<builtin>: recipe for target 'tfd2' failed
make: *** [tfd2] Error 1
clang++ -std=c++14    tfd2.1.cpp   -o tfd2.1
tfd2.1.cpp:11:17: error: destructor name 'b' does not refer to a template
  friend b<U>::~b<U>();
         ~~~~~~ ^
1 error generated.
<builtin>: recipe for target 'tfd2.1' failed
make: *** [tfd2.1] Error 1
clang++ -std=c++14    tfd2.fun.cpp   -o tfd2.fun
tfd2.fun.cpp:11:21: warning: dependent nested name specifier 'b<U>::' for friend
      class declaration is not supported; turning off access control for 'a'
      [-Wunsupported-friend]
  friend void b<U>::f();
              ~~~~~~^
1 warning generated.



make -k CXXFLAGS=-std=c++1z
provides an interesting variant for tfd1.1.cpp requesting the use of
noexcept, which does not solve the problem:

clang++ -std=c++1z    tfd1.cpp   -o tfd1
tfd1.cpp:10:16: error: expected the class name after '~' to name the enclosing
      class
  friend b<T>::~b();
               ^
tfd1.cpp:16:10: note: in instantiation of template class 'a<int>' requested here
  a<int> v;
         ^
1 error generated.
<builtin>: recipe for target 'tfd1' failed
make: *** [tfd1] Error 1
clang++ -std=c++1z    tfd1.1.cpp   -o tfd1.1
tfd1.1.cpp:10:16: error: '~b' is missing exception specification 'noexcept'
  friend b<T>::~b<T>();
               ^
                       noexcept
tfd1.1.cpp:16:10: note: in instantiation of template class 'a<int>' requested
      here
  a<int> v;
         ^
tfd1.1.cpp:4:3: note: previous declaration is here
  ~b() {
  ^
1 error generated.
<builtin>: recipe for target 'tfd1.1' failed
make: *** [tfd1.1] Error 1
clang++ -std=c++1z    tfd1.fun.cpp   -o tfd1.fun
clang++ -std=c++1z    tfd2.cpp   -o tfd2
tfd2.cpp:11:17: error: expected the class name after '~' to name a destructor
  friend b<U>::~b();
                ^
1 error generated.
<builtin>: recipe for target 'tfd2' failed
make: *** [tfd2] Error 1
clang++ -std=c++1z    tfd2.1.cpp   -o tfd2.1
tfd2.1.cpp:11:17: error: destructor name 'b' does not refer to a template
  friend b<U>::~b<U>();
         ~~~~~~ ^
1 error generated.
<builtin>: recipe for target 'tfd2.1' failed
make: *** [tfd2.1] Error 1
clang++ -std=c++1z    tfd2.fun.cpp   -o tfd2.fun
tfd2.fun.cpp:11:21: warning: dependent nested name specifier 'b<U>::' for friend
      class declaration is not supported; turning off access control for 'a'
      [-Wunsupported-friend]
  friend void b<U>::f();
              ~~~~~~^
1 warning generated.

