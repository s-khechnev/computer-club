#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <chrono>
#include <fstream>
#include <iomanip>
#include <vector>

#include "ComputerClub.hpp"
#include "Event.hpp"

namespace parser {

unsigned stoui(const std::string& str) {
  std::size_t idx;
  unsigned long result = std::stoul(str, &idx);
  if (idx != str.size()) throw std::invalid_argument{"invalid str: " + str};
  if (result > std::numeric_limits<unsigned>::max())
    throw std::out_of_range{"overflow"};
  return result;
}

unsigned parseNaturalNum(const std::string& str) {
  unsigned result = stoui(str);
  if (result == 0) throw std::out_of_range{"number must be greater than 0"};
  return result;
}

std::vector<std::string_view> split(std::string_view str, char delim) {
  std::vector<std::string_view> result;
  auto left = str.begin();
  for (auto it = left, end = str.end(); it != end; ++it) {
    if (*it == delim) {
      result.emplace_back(&*left, it - left);
      left = it + 1;
    }
  }
  if (left != str.end()) result.emplace_back(&*left, str.end() - left);
  return result;
}

event::time_point parseTime(const std::string_view str) {
  std::tm t;
  std::istringstream in{std::string{str}};

  in >> std::get_time(&t, "%H:%M");
  if (in.fail()) throw std::invalid_argument{"invalid time format"};
  return event::time_point{std::chrono::hours{t.tm_hour} +
                           std::chrono::minutes{t.tm_min}};
};

event::Id parseEventId(const std::string_view str) {
  if (str == "1") return event::Id::Come;
  if (str == "2") return event::Id::Seat;
  if (str == "3") return event::Id::Wait;
  if (str == "4") return event::Id::Left;
  throw std::runtime_error{"unknown event id"};
}

bool validateClientName(const std::string_view str) {
  return std::all_of(str.begin(), str.end(), [](unsigned char c) {
    return std::islower(c) || std::isdigit(c) || c == '_';
  });
}

std::pair<ComputerClub, std::vector<std::unique_ptr<event::Base>>> parse(
    const std::string& filename) {
  std::ifstream istream{filename};
  if (!istream.is_open())
    throw std::ifstream::failure{"unable to open input file"};

  unsigned tableNum, pricePerHour;
  event::time_point openTime, closeTime;
  std::vector<std::unique_ptr<event::Base>> events;
  std::string line;
  try {
    std::getline(istream, line);
    tableNum = parseNaturalNum(line);

    std::getline(istream, line);
    std::vector strs = split(line, ' ');

    if (strs.size() != 2) throw std::runtime_error{"expected two time points"};
    openTime = parseTime(strs[0]);
    closeTime = parseTime(strs[1]);

    std::getline(istream, line);
    pricePerHour = parseNaturalNum(line);

    while (std::getline(istream, line)) {
      auto strs = split(line, ' ');

      constexpr int minLength = 3;
      if (strs.size() < minLength)
        throw std::runtime_error{"invalid event's format"};

      auto time = parseTime(strs[0]);
      auto eventId = parseEventId(strs[1]);

      std::string clientName{strs[2]};
      if (!validateClientName(strs[2]))
        throw std::runtime_error{"invalid client name"};

      switch (eventId) {
        case event::Id::Come: {
          events.push_back(
              std::move(std::make_unique<event::Come>(time, clientName)));
          break;
        }
        case event::Id::Seat: {
          unsigned tableNum = parseNaturalNum(std::string{strs[3]});
          events.push_back(
              std::make_unique<event::Seat>(time, clientName, tableNum));
          break;
        }
        case event::Id::Wait: {
          events.push_back(std::make_unique<event::Wait>(time, clientName));
          break;
        }
        case event::Id::Left: {
          events.push_back(std::make_unique<event::Left>(time, clientName));
          break;
        }
      }
    }

  } catch (...) {
    throw std::runtime_error{line};
  }

  return std::make_pair(
      ComputerClub{tableNum, openTime, closeTime, pricePerHour},
      std::move(events));
}

}  // namespace parser

#endif  // PARSER_HPP_
