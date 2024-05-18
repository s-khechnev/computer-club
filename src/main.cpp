#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "ComputerClub.hpp"
#include "Event.hpp"
#include "Parser.hpp"

void printTime(event::time_point time) {
  using namespace std::chrono;
  std::time_t timeT = system_clock::to_time_t(time);
  std::tm localTime = *std::gmtime(&timeT);
  std::cout << std::put_time(&localTime, "%H:%M") << "\n";
}

void printError(event::time_point time, std::string error) {
  using namespace std::chrono;
  std::time_t timeT = system_clock::to_time_t(time);
  std::tm localTime = *std::gmtime(&timeT);
  std::cout << std::put_time(&localTime, "%H:%M") << " " << 13 << " " << error
            << "\n";
}

int main() {
  ComputerClub club;
  std::vector<std::unique_ptr<event::Base>> events;
  try {
    std::tie(club, events) = parser::parse("123.txt");
  } catch (const std::runtime_error& err) {
    std::cout << err.what() << "\n";
    return 1;
  }

  printTime(club.getOpenTime());

  for (auto& e : events) {
    switch (e->getId()) {
      case event::Id::Come: {
        auto* event = static_cast<event::Come*>(e.get());
        if (club.containsClient(event->getName())) {
          printError(event->getTime(), "YouShallNotPass");
          continue;
        }
        if (event->getTime() < club.getOpenTime()) {
          printError(event->getTime(), "NotOpenYet");
          continue;
        }

        club.addClient(event->getName());
        continue;
      }
      case event::Id::Seat: {
        auto* event = static_cast<event::Seat*>(e.get());
        if (club.isTableBusy(event->getTableNum())) {
          printError(event->getTime(), "PlaceIsBusy");
          continue;
        }
        if (!club.containsClient(event->getName())) {
          printError(event->getTime(), "ClientUnknown");
          continue;
        }

        club.occupyTable(event->getName(), event->getTableNum(),
                         event->getTime());
        continue;
      }
      case event::Id::Wait: {
        auto* event = static_cast<event::Wait*>(e.get());
        if (club.getFreeTables() > 0) {
          printError(event->getTime(), "ICanWaitNoLonger!");
          continue;
        }
        if (club.getQueueSize() > club.getTableNum()) {
          std::cout << "I go home 11 \n";
          continue;
        }
        club.clientGoWait(event->getName(), event->getTime());
        continue;
      }
      case event::Id::Left: {
        auto* event = static_cast<event::Left*>(e.get());
        if (!club.containsClient(event->getName())) {
          printError(event->getTime(), "ClientUnknown");
          continue;
        }

        unsigned tableN = club.removeClient(event->getName(), event->getTime());
        if (tableN != 0)
          club.inviteWaiting(event->getName(), tableN, event->getTime());

        // 12

        continue;
      }
    }
  }

  club.close();
  club.printStat();

  printTime(club.getCloseTime());

  return 0;
}
