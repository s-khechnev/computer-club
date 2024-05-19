#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <chrono>
#include <iostream>
#include <string>

#include "Utils.hpp"

namespace event {

using time_point =
    std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>;

enum class OutId { Left = 11, Sit, Error };

std::ostream& operator<<(std::ostream& os, OutId id) {
  os << static_cast<int>(id);
  return os;
}

enum class Id {
  Come = 1,
  Sit,
  Wait,
  Left,
};

std::ostream& operator<<(std::ostream& os, Id id) {
  os << static_cast<int>(id);
  return os;
}

class Base {
 public:
  time_point getTime() const { return time; }
  Id getId() const { return id; }
  const std::string& getName() const { return name; }

  virtual ~Base() = default;
  friend std::ostream& operator<<(std::ostream& os, const Base& p);
  virtual void dump(std::ostream& os) const {
    os << timeToStr(getTime()) << " " << getId() << " " << getName();
  }

 protected:
  Base(time_point time, Id id, std::string name)
      : time(time), id(id), name(std::move(name)) {}

 private:
  const time_point time;
  const Id id;
  const std::string name;
};

class BaseWithTableNum : public Base {
 public:
  unsigned getTableNum() const { return tableNum; }

  void dump(std::ostream& os) const override {
    os << timeToStr(getTime()) << " " << getId() << " " << getName() << " "
       << getTableNum();
  }

 protected:
  BaseWithTableNum(time_point time, Id id, std::string name, unsigned tableNum)
      : Base(time, id, std::move(name)), tableNum(tableNum) {}

 private:
  const unsigned tableNum;
};

struct Come final : public Base {
  Come(time_point time, std::string name)
      : Base(time, Id::Come, std::move(name)) {}
};

struct Sit final : public BaseWithTableNum {
  Sit(time_point time, std::string name, unsigned tableNum)
      : BaseWithTableNum(time, Id::Sit, std::move(name), tableNum) {}
};

struct Wait final : public Base {
  Wait(time_point time, std::string name)
      : Base(time, Id::Wait, std::move(name)) {}
};

struct Left final : public Base {
  Left(time_point time, std::string name)
      : Base(time, Id::Left, std::move(name)) {}
};

std::ostream& operator<<(std::ostream& os, const Base& b) {
  b.dump(os);
  return os;
}

}  // namespace event

#endif  // EVENT_HPP_
