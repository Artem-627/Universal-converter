#include "App.h"
#include "BigFraction.h"
#include "BigNumber.h"
#include "Bigint.h"
#include <QApplication>
#include <cstdint>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Window window;

  // std::string number;
  // int from_base, to_base;
  //
  // std::cin >> number;
  // std::cin >> from_base >> to_base;
  //
  // BigNumber num(number, from_base);
  //
  // std::cout << from_base << ":   " << num << '\n';
  // std::cout << to_base << ":   " << num.Convert(to_base) << '\n';

  // BigInt num_1({3, 2}, 1, 10);
  // BigInt num_2({2, 3, 4}, 0, 10);
  // BigInt num_3({3, 4}, 0, 10);
  // BigInt num_4({3, 2}, 0, 10);
  //
  // BigFraction frac_1(num_1, num_2);
  // BigFraction frac_2(num_3, num_4);
  //
  // std::cout << frac_1 + frac_2 << '\n';

  return QApplication::exec();
}
