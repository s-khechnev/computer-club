#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <chrono>

inline unsigned calculateProfit(std::chrono::minutes usage,
                                unsigned pricePerHour) {
  auto hoursUsed =
      std::chrono::duration_cast<std::chrono::hours>(usage).count();
  return (hoursUsed + 1) * pricePerHour;  // +1 to up rounding
}

#endif  // UTILS_HPP_
