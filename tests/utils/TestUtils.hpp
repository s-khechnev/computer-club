#ifndef TestUtils_HPP_
#define TestUtils_HPP_

#include <string>

#include "Manager.hpp"
#include "Parser.hpp"

std::string readFile(const std::string& filename) {
  std::ifstream file{filename};
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

int run(const std::string& filename) {
  ComputerClub club;
  std::vector<std::unique_ptr<event::Base>> events;

  try {
    std::tie(club, events) = parser::parse(filename);
  } catch (const std::runtime_error& err) {
    std::cout << err.what() << "\n";
    return 1;
  }

  Manager manager{std::move(club)};
  manager.processEvents(events);
  manager.closeClub();
  manager.printStatistics();

  return 0;
}

#endif  // TestUtils_HPP_
