template <typename T>
class b {
public:
  ~b() {
  }
};

template <typename T>
class a {
  template <typename U>
  friend b<U>::~b();
};


int main() {

  a<int> v;

  return 0;
}
