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

std::string minutesToStr(std::chrono::minutes minutes) {
  using namespace std::chrono;
  using namespace std::chrono_literals;
  auto h = duration_cast<hours>(minutes);
  auto m = duration_cast<std::chrono::minutes>(minutes % 1h);
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << h.count() << ":" << std::setw(2)
      << std::setfill('0') << m.count();
  return oss.str();
}

#endif  // UTILS_HPP_
