#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

namespace {

template<typename T>
class Series {
  public:
  Series() = default;
  Series(std::vector<T> data) : data(std::move(data)) {}
  Series(std::string name) : name(std::move(name)) {}
  Series(std::string name, std::vector<T> data) : name(std::move(name)), data(std::move(data)) {}
  Series(const Series&) = default;
  Series(Series&&) = default;
  Series& operator=(const Series&) = default;
  Series& operator=(Series&&) = default;

  std::string get_name() const {
    return name;
  }

  const std::vector<T>& get_data() const {
    return data;
  }

  std::vector<T>& get_data() {
    return data;
  }

  Series<T> add(const Series<T>& other) {
    if (data.size() != other.data.size()) {
      throw std::runtime_error("Cannot add series with different sizes");
    }

    return apply_binary(other, std::plus<T>());
  }

  Series<T> multiply(const Series<T>& other) {
    if (data.size() != other.data.size()) {
      throw std::runtime_error("Cannot multiply series with different sizes");
    }

    return apply_binary(other, std::multiplies<T>());
  }

  private:
  std::string name = "Unnamed";
  std::vector<T> data;

  template<typename Func>
  Series<T> apply_binary(const Series<T>& other, Func&& binary_op) {
    Series<T> result(name);
    result.data.reserve(data.size());
    std::transform(data.begin(), data.end(), other.data.begin(), std::back_inserter(result.data), binary_op);
    return result;
  }
};

template<typename T>
void print_series(const Series<T>& series) {
  std::cout << series.get_name() << ": ";
  for (const auto& value : series.get_data()) {
    std::cout << value << " ";
  }
  std::cout << std::endl;
}

template <typename LHS, typename RHS, typename RET>
Series<RET> add(const Series<LHS>& left, const Series<RHS>& right) {
  Series<RET> result(left.get_name());
  std::transform(left.get_data().begin(), left.get_data().end(), right.get_data().begin(), std::back_inserter(result.get_data()), [](const auto& l, const auto& r) {return l + r;});
  return result;
}


}

int main() {
  auto series1 = Series(std::vector<int>({1, 2, 3, 4, 5}));
  auto series2 = Series(series1);
  auto series3 = series1.add(series2);
  auto series4 = add<int, int, double>(series1, series2);
  print_series(series1);
  print_series(series2);
  print_series(series3);
  print_series(series4);
  print_series(series1.multiply(series2));

  return EXIT_SUCCESS;
}