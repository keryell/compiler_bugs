template <typename T>
class b {
public:
  ~b() {
  }
};

template <typename T>
class a {
  friend b<T>::~b<T>();
};


int main() {

  a<int> v;

  return 0;
}
