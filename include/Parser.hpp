#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <chrono>
#include <exception>
#include <fstream>
#include <iomanip>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <vector>

#include "ComputerClub.hpp"
#include "Event.hpp"

namespace parser {

unsigned stoui(const std::string& str) {
  std::size_t idx;
  unsigned long result = std::stoul(str, &idx);
  if (idx != str.size()) throw std::invalid_argument{"invalid str"};
  if (result > std::numeric_limits<unsigned>::max())
    throw std::out_of_range{"overflow"};
  return result;
}

unsigned parsePositiveNum(const std::string& str) {
  unsigned result = stoui(str);
  if (result == 0) throw std::out_of_range{"number must be greater than 0"};
  return result;
}

event::time_point parseTime(std::string_view str) {
  std::tm t{};
  std::istringstream in{std::string{str}};
  in >> std::get_time(&t, "%H:%M");
  if (in.fail()) throw std::invalid_argument{"invalid time format"};
  return event::time_point{std::chrono::hours{t.tm_hour} +
                           std::chrono::minutes{t.tm_min}};
};

event::Id parseEventId(std::string_view str) {
  if (str == "1") return event::Id::Come;
  if (str == "2") return event::Id::Sit;
  if (str == "3") return event::Id::Wait;
  if (str == "4") return event::Id::Left;
  throw std::runtime_error{"unknown event id"};
}

bool validateClientName(std::string_view str) {
  return std::all_of(str.begin(), str.end(), [](auto c) {
    return (c >= 'a' && c <= 'z') || std::isdigit(c) || c == '_';
  });
}

auto split(const std::string& line) {
  return line | std::ranges::views::split(' ') |
         std::ranges::views::transform([](auto&& rng) {
           return std::string_view(&*rng.begin(), std::ranges::distance(rng));
         });
}

std::unique_ptr<event::Base> parseEvent(const std::string& line) {
  auto strs = split(line);
  auto itStr = std::ranges::begin(strs);

  auto time = parseTime(*itStr);
  auto eventId = parseEventId(*++itStr);

  std::string clientName{*++itStr};
  if (!validateClientName(clientName))
    throw std::runtime_error{"invalid client name"};

  switch (eventId) {
    case event::Id::Come: {
      return std::make_unique<event::Come>(time, std::move(clientName));
    }
    case event::Id::Sit: {
      unsigned tableNum = parsePositiveNum(std::string{*++itStr});
      return std::make_unique<event::Sit>(time, std::move(clientName),
                                          tableNum);
    }
    case event::Id::Wait: {
      return std::make_unique<event::Wait>(time, std::move(clientName));
    }
    case event::Id::Left: {
      return std::make_unique<event::Left>(time, std::move(clientName));
    }
  }

  if (++itStr != std::ranges::end(strs))
    throw std::runtime_error{"invalid line"};
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
    tableNum = parsePositiveNum(line);

    std::getline(istream, line);
    auto strs = split(line);
    auto itStr = std::ranges::begin(strs);
    openTime = parseTime(*itStr);
    closeTime = parseTime(*++itStr);

    if (++itStr != std::ranges::end(strs))
      throw std::runtime_error{"expected two time points"};

    std::getline(istream, line);
    pricePerHour = parsePositiveNum(line);

    if (std::getline(istream, line)) {
      auto event = parseEvent(line);
      auto prevTimePoint = event->getTime();
      events.push_back(std::move(event));
      while (std::getline(istream, line)) {
        auto event = parseEvent(line);
        if (event->getTime() < prevTimePoint)
          throw std::runtime_error{"invalid event's order"};
        prevTimePoint = event->getTime();

        events.push_back(std::move(event));
      }
    }
  } catch (const std::exception&) {
    throw std::runtime_error{line};
  }

  return std::make_pair(
      ComputerClub{tableNum, openTime, closeTime, pricePerHour},
      std::move(events));
}

}  // namespace parser

#endif  // PARSER_HPP_
