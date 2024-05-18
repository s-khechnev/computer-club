#include <memory>

#include "ComputerClub.hpp"
#include "Event.hpp"
#include "Manager.hpp"
#include "Parser.hpp"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <input file>\n";
    return 1;
  }

  ComputerClub club;
  std::vector<std::unique_ptr<event::Base>> events;

  try {
    std::tie(club, events) = parser::parse(argv[1]);
  } catch (const std::runtime_error& err) {
    std::cout << err.what() << "\n";
    return 1;
  }

  Manager manager{std::move(club)};
  manager.processEvents(events);
  manager.closeClub();
  manager.printStats();

  return 0;
}
