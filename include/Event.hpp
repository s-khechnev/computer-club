#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <chrono>
#include <string>

namespace event {

using time_point =
    std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>;

class Base {
 public:
  enum class Id {
    Come = 1,
    Seat = 2,
    Wait = 3,
    LeftByHimself = 4,
    Left = 11,
    SeatAfterWait = 12,
    Error = 13,
  };

  time_point getTime() const { return time; }
  Id getId() const { return id; }
  virtual ~Base() = default;

 protected:
  Base(time_point time, Id id) : time(time), id(id) {}

 private:
  const time_point time;
  const Id id;
};

class BaseWithName : public Base {
 public:
  const std::string& getName() const { return name; }

 protected:
  BaseWithName(time_point time, Id id, const std::string& name)
      : Base(time, id), name(name) {}

 private:
  const std::string name;
};

class BaseWithNameTableNum : public BaseWithName {
 public:
  unsigned getTableNum() const { return tableNum; }

 protected:
  BaseWithNameTableNum(time_point time, Id id, const std::string& name,
                       unsigned tableNum)
      : BaseWithName(time, id, name), tableNum(tableNum) {}

 private:
  const unsigned tableNum;
};

struct Come final : public BaseWithName {
  Come(time_point time, const std::string& name)
      : BaseWithName(time, Id::Come, name) {}
};

struct Seat final : public BaseWithNameTableNum {
  Seat(time_point time, const std::string& name, unsigned tableNum)
      : BaseWithNameTableNum(time, Id::Seat, name, tableNum) {}
};

struct Wait final : public BaseWithName {
  Wait(time_point time, const std::string& name)
      : BaseWithName(time, Id::Wait, name) {}
};

struct LeftByHimself final : public BaseWithName {
  LeftByHimself(time_point time, const std::string& name)
      : BaseWithName(time, Id::LeftByHimself, name) {}
};

struct Left final : public BaseWithName {
  Left(time_point time, const std::string& name)
      : BaseWithName(time, Id::Left, name) {}
};

struct SeatAfterWait final : public BaseWithNameTableNum {
  SeatAfterWait(time_point time, const std::string& name, unsigned tableNum)
      : BaseWithNameTableNum(time, Id::SeatAfterWait, name, tableNum) {}
};

class Error final : public Base {
 public:
  Error(time_point time, const std::string& error)
      : Base(time, Id::Error), error(error) {}

  const std::string& getError() const { return error; }

 private:
  std::string error;
};

}  // namespace event

#endif  // EVENT_HPP_
