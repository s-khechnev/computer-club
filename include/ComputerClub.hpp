#ifndef COMPUTERCLUB_HPP_
#define COMPUTERCLUB_HPP_

#include <chrono>

struct ComputerClub {
  using time_point =
      std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>;

  unsigned tableNum = 0;
  time_point openTime, closeTime;
  unsigned pricePerHour = 0;
};

#endif  // COMPUTERCLUB_HPP_
