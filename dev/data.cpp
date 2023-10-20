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

  template<typename Func>
  Series<T> apply_binary(const Series<T>& other, Func&& binary_op) {
    std::vector<T> result;
    result.reserve(data.size());
    std::transform(data.begin(), data.end(), other.data.begin(), std::back_inserter(result), binary_op);
    return Series<T>(name, std::move(result));
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
  std::string name;
  std::vector<T> data;
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
  Series<RET> result;
  std::transform(left.get_data().begin(), left.get_data().end(), right.get_data().begin(), std::back_inserter(result.get_data()), [](const auto& l, const auto& r) {return l + r;});
}


}

int main() {
    auto series1 = Series<int>("Series 1", {1, 2, 3, 4, 5});
    auto series2 = Series<int>("Series 2", {1, 2, 3, 4, 5});
    auto series3 = series1.add(series2);
    Series<double> series4 = add(series1, series2);
    print_series(series1);
    print_series(series2);
    print_series(series1.add(series2));
    print_series(s);
    print_series(series1.multiply(series2));

    return EXIT_SUCCESS;
}