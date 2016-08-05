template <typename T>
class b {
public:
  void f() {
  }
};

template <typename T>
class a {
  template <typename U>
  friend void b<U>::f<U>g();
};


int main() {

  a<int> v;

  return 0;
}
