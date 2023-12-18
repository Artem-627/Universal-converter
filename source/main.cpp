#include "Bigint.h"
#include <iostream>

int main() {
  BigInt number1({6, 5, 2, 3, 4}, 0, 10); // 43256
  BigInt number2({4, 5, 3}, 0, 10);       // 354

  std::cout << number1 << " + " << number2 << " = " << number1 + number2;

  return 0;
}
