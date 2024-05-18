#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <chrono>
#include <iomanip>
#include <string>

#include "Event.hpp"

inline unsigned calculateProfit(std::chrono::minutes usage,
                                unsigned pricePerHour) {
  auto hoursUsed =
      std::chrono::duration_cast<std::chrono::hours>(usage).count();
  return (hoursUsed + 1) * pricePerHour;  // +1 to up rounding
}

std::string timeToStr(event::time_point time) {
  using namespace std::chrono;
  std::time_t timeT = system_clock::to_time_t(time);
  std::tm tm = *std::gmtime(&timeT);
  std::stringstream ss;
  ss << std::put_time(&tm, "%H:%M");
  return ss.str();
}

#endif  // UTILS_HPP_
