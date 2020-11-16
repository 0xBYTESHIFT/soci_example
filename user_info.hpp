#pragma once
#include "db_pool.hpp"
#include "property.hpp"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <numeric>
#include <regex>
#include <vector>

// некоторые вспомогательные ф-ии для преобразования массивов в векторы и
// обратно
template <typename T>
static void extract_integers(const std::string &str, std::vector<T> &result) {
  result.clear();
  using re_iterator = std::regex_iterator<std::string::const_iterator>;
  using re_iterated = re_iterator::value_type;
  std::regex re("([\\+\\-]?\\d+)");
  re_iterator rit(str.begin(), str.end(), re), rend;
  std::transform(rit, rend, std::back_inserter(result),
                 [](const re_iterated &it) { return std::stoi(it[1]); });
}

template <typename T>
static void split_integers(std::string &str, const std::vector<T> &arr) {
  str = "{";
  if (arr.size()) {
    str += std::accumulate(
        arr.begin() + 1, arr.end(), std::to_string(arr[0]),
        [](const std::string &a, T b) { return a + ',' + std::to_string(b); });
  }
  str += "}";
}

// структура таблицы `users'
class user_info {
public:
  property<int> id;
  property<std::tm> birthday;
  property<std::string> firstname, lastname;
  property<std::vector<int>> friends;

  user_info();

  void print();
  void clear();
  user_info &operator=(const user_info &rhs);
};

// для работы со своими типами, в SOCI имеются конвертеры
namespace soci {

template <> struct type_conversion<user_info> {
    typedef values base_type;

  static void from_base(values const &v, indicator ind, user_info &p) {
    if (ind == i_null)
      return;
    try {
      p.id = v.get<int>("id", 0);
      p.birthday = v.get<std::tm>("birthday", {});
      p.firstname = v.get<std::string>("firstname", {});
      p.lastname = v.get<std::string>("lastname", {});

      std::string arr_str = v.get<std::string>("friends", {});
      extract_integers(arr_str, *p.friends);
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  static void to_base(const user_info &p, values &v, indicator &ind) {
    try {
      v.set("id", *p.id);
      v.set("birthday", *p.birthday);
      v.set("firstname", *p.firstname);
      v.set("lastname", *p.lastname);

      std::string arr_str;
      split_integers(arr_str, *p.friends);
      v.set("friends", arr_str);

      ind = i_ok;
      return;
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    ind = i_null;
  }
};

} // namespace soci

user_info::user_info()
    : id(0), birthday(), firstname(), lastname(), friends() {}

void user_info::print() {
  std::cout << "id: " << *id << "\n";
  std::cout << "birthday: " << std::put_time(&(*birthday), "%c %Z") << "\n";
  std::cout << "firstname: " << *firstname << "\n";
  std::cout << "lastname: " << *lastname << "\n";
  std::string arr_str;
  split_integers(arr_str, *friends);
  std::cout << "friends: " << arr_str << "\n";
}

void user_info::clear() {
  id = 0;
  firstname = lastname = "";
  (*friends).clear();
}

user_info &user_info::operator=(const user_info &rhs) {
  if (this != &rhs) {
    id = rhs.id;
    birthday = rhs.birthday;
    firstname = rhs.firstname;
    lastname = rhs.lastname;
    friends = rhs.friends;
  }
  return *this;
}