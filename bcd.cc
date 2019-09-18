#include <iostream>
#include "bcd.h"

int main() {
  //
  auto u = bcd::Encode<int16_t, uint16_t, 8U, 8U>(-17);
  bcd::BinPrint<uint16_t, false>(u);
  std::cout << std::endl << bcd::BinString<uint32_t>(1U << 30) << std::endl;
  bcd::BinPrint(bcd::Decode<int16_t, uint16_t, 8U, 8U>(u));
  printf("\n%d", bcd::Decode<uint8_t, uint8_t, 2U, 3U>(0b01001U));
}



