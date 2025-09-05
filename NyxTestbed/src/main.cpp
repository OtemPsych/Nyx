#include <iostream>

#include <nyx/core/assert.h>

void test_assert();

int main() {
  test_assert();
}

void test_assert() {
  try {
    NYX_ASSERT(1 < 2, "Shouldn't throw");
    NYX_ASSERT(1 > 2, "Should throw");
  } catch (std::logic_error& e) {
    std::cerr << e.what();
  }
}