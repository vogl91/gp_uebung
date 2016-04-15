#include <type_traits>
#include <cmath>
#include <iostream>
using namespace std;

template <size_t N, typename T> struct vec_n {
  using scalar_type               = T;
  static const size_t cardinality = N;
  using this_type                 = vec_n<cardinality, scalar_type>;

  scalar_type data[cardinality];

  scalar_type &operator[](int i) { return data[i]; }
  const scalar_type &operator[](int i) const { return data[i]; }

  template <typename F> void for_each(F func) {
    for (auto i = 0u; i < cardinality; ++i) {
      func(i);
    }
  }
  this_type &operator*=(scalar_type scalar) {
    for_each([this, &scalar](size_t i) { data[i] *= scalar; });
    return *this;
  }
  this_type &operator+=(const this_type &that) {
    for_each([this, &that](size_t i) { data[i] += that.data[i]; });
    return *this;
  }
  this_type &operator-=(const this_type &that) {
    for_each([this, &that](size_t i) { data[i] -= that.data[i]; });
    return *this;
  }
};

// length
template <size_t N, typename T> double length(const vec_n<N, T> &vec) {
  auto scalar_product = vec * vec;
  return sqrt(static_cast<double>(scalar_product));
}

template <size_t N, typename T>
bool operator==(const vec_n<N, T> &lhs, const vec_n<N, T> &rhs) {
  for (auto i = 0; i < vec_n<N, T>::cardinality; ++i) {
    if (lhs[i] != rhs[i])
      return false;
  }
  return true;
}

template <size_t N, typename T>
bool operator!=(const vec_n<N, T> &lhs, const vec_n<N, T> &rhs) {
  for (auto i = 0; i < vec_n<N, T>::cardinality; ++i) {
    if (lhs[i] == rhs[i])
      return false;
  }
  return true;
}

// scalar multiplication
template <size_t N, typename T>
vec_n<N, T> operator*(T scalar, const vec_n<N, T> &vec) {
  return vec_n<N, T>{vec} *= scalar;
}
template <size_t N, typename T>
vec_n<N, T> operator*(const vec_n<N, T> &vec, T scalar) {
  return vec_n<N, T>{vec} *= scalar;
}

// vector addition/subtraction
template <size_t N, typename T>
vec_n<N, T> operator+(const vec_n<N, T> &lhs, const vec_n<N, T> &rhs) {
  return vec_n<N, T>{lhs} += rhs;
}

template <size_t N, typename T>
vec_n<N, T> operator-(const vec_n<N, T> &lhs, const vec_n<N, T> &rhs) {
  return vec_n<N, T>{lhs} -= rhs;
}

// vector multiplication (scalar product)
template <size_t N, typename T>
T operator*(const vec_n<N, T> &lhs, const vec_n<N, T> &rhs) {
  auto result = T{};
  for (auto i = 0u; i < vec_n<N, T>::cardinality; ++i) {
    result += lhs[i] * rhs[i];
  }
  return result;
}

template <typename T> using vec3 = vec_n<3, T>;

// cross product (for 3D Vectors only)
template <typename T>
vec3<T> cross_product(const vec3<T> &lhs, const vec3<T> &rhs) {
  auto result = vec3<T>{};
  result[0]   = lhs[1] * rhs[2] - lhs[2] * rhs[1];
  result[1]   = lhs[2] * rhs[0] - lhs[0] * rhs[2];
  result[2]   = lhs[0] * rhs[1] - lhs[1] * rhs[0];
  return result;
}

constexpr double M_PI = 3.1415926535;
inline double to_degrees(double radians) { return radians * (180.0 / M_PI); }

template <size_t N, typename T>
std::ostream &operator<<(std::ostream &os, const vec_n<N, T> &vec) {
  os << "(";
  for (auto i = 0u; i < vec_n<N, T>::cardinality; ++i) {
    if (i != 0)
      os << ",";
    os << vec[i];
  }
  os << ")";
  return os;
}

// normalize
template <size_t N, typename T, class = typename std::enable_if<
                                    std::is_floating_point<T>::value>::type>
vec_n<N, T> &normalize(vec_n<N, T> &&vec) {
  auto vec_length = length(vec);
  vec.for_each([&vec, &vec_length](size_t i) { vec[i] /= vec_length; });
  return vec;
}

using vec3f = vec3<float>;
using vec3d = vec3<double>;

void a1() {
  cout << "---- a1 ----" << endl;
  auto a = vec3d{{56, 34, 23}};
  auto b = vec3d{{43, 88, 15}};
  auto c = vec3d{{33, 77, 44}};

  cout << "a = " << a << endl;
  cout << "b = " << b << endl;
  cout << "c = " << c << endl;

  auto ab = b - a;
  auto bc = c - b;

  cout << "ab = " << ab << endl;
  cout << "bc = " << bc << endl;

  auto cross_product_ab_bc = cross_product(ab, bc);

  cout << ab << " x " << bc << " = ";
  cout << cross_product_ab_bc << endl;

  if (cross_product_ab_bc == vec3d{{0, 0, 0}}) {
    cout << "liegen auf einer Linie" << endl;
  } else {
    cout << "liegen nicht auf einer Linie" << endl;
  }
}

void a2() {
  cout << "---- a2 ----" << endl;
  auto a = vec3d{{4, -5, -2}};
  auto b = vec3d{{12, -9, 2}};
  auto c = vec3d{{1, -4, 5}};

  auto ab = b - a;
  auto ac = c - a;
  auto bc = c - b;

  cout << "ab = " << ab << endl;
  cout << "ac = " << ac << endl;
  cout << "bc = " << bc << endl;

  cout << "(ab * ac) = " << (ab * ac) << endl;
  cout << "(ab * bc) = " << (ab * bc) << endl;
  cout << "(bc * ac) = " << (bc * ac) << endl;

  if (ab * ac == 0 || ab * bc == 0 || bc * ac == 0) {
    cout << "rechtwinklig" << endl;
  } else {
    cout << "nicht rechtwinklig" << endl;
  }
}

template <size_t N, typename T>
double angle_between(const vec_n<N, T> &lhs, const vec_n<N, T> &rhs) {
  auto scalar_product = lhs * rhs;
  auto cosinus = scalar_product / (length(lhs) * length(rhs));
  return acos(cosinus);
}

void a3() {
  cout << "---- a3 ----" << endl;
  auto v       = vec3d{{-3, 4, 5}};
  auto angle_x = angle_between(v, vec3d{{1, 0, 0}});
  auto angle_y = angle_between(v, vec3d{{0, 1, 0}});
  auto angle_z = angle_between(v, vec3d{{0, 0, 1}});
  cout << "x:" << angle_x << "|" << to_degrees(angle_x) << endl
       << "y:" << angle_y << "|" << to_degrees(angle_y) << endl
       << "z:" << angle_z << "|" << to_degrees(angle_z) << endl;
}

int main(int argc, char const *argv[]) {
  a1();
  a2();
  a3();
  return 0;
}
