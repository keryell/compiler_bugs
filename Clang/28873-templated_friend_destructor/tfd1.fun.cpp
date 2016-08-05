template <typename T>
class b {
public:
  void f() {
  }
};

template <typename T>
class a {
  friend void b<T>::f();
};


int main() {

  a<int> v;

  return 0;
}
