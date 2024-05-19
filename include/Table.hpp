#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <cassert>
#include <chrono>

#include "Event.hpp"
#include "Utils.hpp"

class Table {
 public:
  using time_point = event::time_point;

  Table(unsigned pricePerHour) : pricePerHour(pricePerHour) {}

  void occupy(time_point time) {
    assert(!isBusy);

    isBusy = true;
    startUsage = time;
  }

  void release(time_point time) {
    assert(isBusy);

    isBusy = false;
    auto usage = time - startUsage;
    profit += calculateProfit(usage, pricePerHour);
    totalUsage += usage;
  }

  bool busy() const { return isBusy; }
  unsigned getProfit() const { return profit; }
  std::chrono::minutes getTotalUsage() const { return totalUsage; }

 private:
  const unsigned pricePerHour = 0;
  unsigned profit = 0;
  bool isBusy = false;
  std::chrono::minutes totalUsage{0};
  time_point startUsage;
};

#endif  // TABLE_HPP_
