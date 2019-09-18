
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <string>
#include <type_traits>

namespace bcd {
template <typename IntT, typename BinT, uint8_t t_bits, uint8_t u_bits,
          IntT base = static_cast<IntT>(10U)>
inline constexpr BinT Encode(IntT num) noexcept {
  static_assert(std::is_integral<BinT>::value && std::is_integral<IntT>::value,
                "Both bin_t and int_t must be integral types");
  static_assert(t_bits != 0 && u_bits != 0, "T and U's bits can not be zero");
  static_assert((t_bits + u_bits) <= (sizeof(IntT) * 8U),
                "Invalid T and U size exceeds parameter's");
  static_assert(std::is_unsigned<BinT>::value,
                "Binary type must be unsigned (consistency)");
  constexpr IntT full = ~static_cast<IntT>(0);  // all bits filled
  constexpr uint16_t n_int_bits = sizeof(IntT) * 8U;
  constexpr IntT t_mask = full >> (n_int_bits - t_bits);
  constexpr IntT u_mask = full >> (n_int_bits - u_bits);
  IntT t = (num / base) & t_mask;  // align T's bits to right
                                   // and ignore irrelevant MSBs
  IntT u = std::abs(num % base) & u_mask;
  return static_cast<BinT>((t << u_bits) | u);
}

template <typename IntT, typename BinT, uint8_t t_bits, uint8_t u_bits,
          IntT base = static_cast<IntT>(10U)>
inline constexpr IntT Decode(BinT bin) noexcept {
  static_assert(std::is_integral<BinT>::value && std::is_integral<IntT>::value,
                "Both bin_t and int_t must be integral types");
  static_assert(t_bits != 0 && u_bits != 0, "T and U's bits can not be zero");
  static_assert((t_bits + u_bits) <= (sizeof(IntT) * 8U),
                "Invalid T and U size exceeds parameter's");
  static_assert(std::is_unsigned<BinT>::value,
                "Binary type must be unsigned (consistency)");
  constexpr BinT full = ~static_cast<IntT>(0U);  // all bits filled
  constexpr uint16_t n_int_bits = sizeof(IntT) * 8U;
  constexpr BinT t_mask = (full >> (n_int_bits - t_bits));
  constexpr BinT u_mask = (full >> (n_int_bits - u_bits));
  BinT t = (bin >> u_bits) & t_mask;  // align T's bits to right
                                      // and ignore irrelevant MSBs
  BinT u = std::is_signed<IntT>::value ? std::abs(bin & u_mask)
                                       : (bin & u_mask);  // set

  return static_cast<IntT>(t * base + u);
}

#ifdef __cpp_if_constexpr
#define BCD_CONSTEXPR_IF if constexpr
#else
#define BCD_CONSTEXPR_IF if
#endif

template <typename IntT, bool MsbFirst = true>
void BinPrint(IntT val) {
  static_assert(std::is_integral<IntT>::value, "IntT must be an integral type");
  constexpr uint16_t n_bits = 8 * sizeof(IntT);
  printf("0b");
  BCD_CONSTEXPR_IF(MsbFirst) {
    for (int i = n_bits - 1; i >= 0; i--) {
      printf("%u", (val >> i) & static_cast<IntT>(0b1U));
    }
  }
  else {
    for (int i = 0; i < n_bits; i++) {
      printf("%u", (val >> i) & static_cast<IntT>(0b1U));
    }
  }
};

template <typename IntT, bool MsbFirst = true>
std::string BinString(IntT val) {
  static_assert(std::is_integral<IntT>::value, "IntT must be an integral type");
  constexpr uint16_t n_bits = 8 * sizeof(IntT);
  std::string str;
  str.resize(n_bits + 2);
  str[0] = '0';
  str[1] = 'b';

  for (int i = 0; i < n_bits; i++) {
    str[(MsbFirst ? (n_bits + 2 - i - 1) : i)] =
        ((val >> i) & static_cast<IntT>(0b1U)) ? '1' : '0';
  }
  return std::move(str);
};

};  // namespace bcd
