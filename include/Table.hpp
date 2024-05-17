#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <cassert>
#include <chrono>

#include "Utils.hpp"

class Table {
 public:
  Table(unsigned pricePerHour) : pricePerHour(pricePerHour) {}

  void occupy(std::chrono::minutes time) {
    assert(!isBusy);

    isBusy = true;
    startUsage = time;
  }

  void release(std::chrono::minutes time) {
    assert(isBusy);

    isBusy = false;
    auto usage = time - startUsage;
    profit += calculateProfit(usage, pricePerHour);
    totalUsage += usage;
    startUsage = std::chrono::minutes{0};
  }

  bool busy() const { return isBusy; }
  unsigned getProfit() const { return profit; }
  std::chrono::minutes getTotalUsage() const { return totalUsage; }

 private:
  const unsigned pricePerHour = 0;
  unsigned profit = 0;
  bool isBusy = false;
  std::chrono::minutes totalUsage{0};
  std::chrono::minutes startUsage{0};
};

#endif  // TABLE_HPP_
