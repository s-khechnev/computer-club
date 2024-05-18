#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <chrono>
#include <iostream>
#include <string>

namespace event {

using time_point =
    std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>;

enum class Id {
  Come = 1,
  Seat,
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

 protected:
  Base(time_point time, Id id, const std::string& name)
      : time(time), id(id), name(name) {}

 private:
  const time_point time;
  const Id id;
  const std::string name;
};

class BaseWithTableNum : public Base {
 public:
  unsigned getTableNum() const { return tableNum; }

 protected:
  BaseWithTableNum(time_point time, Id id, const std::string& name,
                   unsigned tableNum)
      : Base(time, id, name), tableNum(tableNum) {}

 private:
  const unsigned tableNum;
};

struct Come final : public Base {
  Come(time_point time, const std::string& name) : Base(time, Id::Come, name) {}
};

struct Seat final : public BaseWithTableNum {
  Seat(time_point time, const std::string& name, unsigned tableNum)
      : BaseWithTableNum(time, Id::Seat, name, tableNum) {}
};

struct Wait final : public Base {
  Wait(time_point time, const std::string& name) : Base(time, Id::Wait, name) {}
};

struct Left final : public Base {
  Left(time_point time, const std::string& name) : Base(time, Id::Left, name) {}
};

}  // namespace event

#endif  // EVENT_HPP_
