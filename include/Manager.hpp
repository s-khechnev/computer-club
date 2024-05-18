#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <memory>
#include <vector>

#include "ComputerClub.hpp"
#include "Event.hpp"
#include "Utils.hpp"

class Manager {
 public:
  Manager(ComputerClub club) : club(club) {}

  void processEvents(const std::vector<std::unique_ptr<event::Base>>& events) {
    std::cout << timeToStr(club.getOpenTime());

    for (auto& e : events) {
      switch (e->getId()) {
        case event::Id::Come:
          handleEvent(static_cast<event::Come&>(*e));
          break;
        case event::Id::Seat:
          handleEvent(static_cast<event::Seat&>(*e));
          break;
        case event::Id::Wait:
          handleEvent(static_cast<event::Wait&>(*e));
          break;
        case event::Id::Left:
          handleEvent(static_cast<event::Left&>(*e));
          break;
      }
    }

    std::cout << timeToStr(club.getCloseTime());
  }

 private:
  void handleEvent(event::Come& event) {
    if (club.containsClient(event.getName())) {
      printError(event.getTime(), "YouShallNotPass");
      return;
    }
    if (event.getTime() < club.getOpenTime()) {
      printError(event.getTime(), "NotOpenYet");
      return;
    }

    club.addClient(event.getName());
  }

  void handleEvent(event::Seat& event) {
    if (club.isTableBusy(event.getTableNum())) {
      printError(event.getTime(), "PlaceIsBusy");
      return;
    }
    if (!club.containsClient(event.getName())) {
      printError(event.getTime(), "ClientUnknown");
      return;
    }

    club.occupyTable(event.getName(), event.getTableNum(), event.getTime());
    return;
  }

  void handleEvent(event::Wait& event) {
    if (club.getFreeTables() > 0) {
      printError(event.getTime(), "ICanWaitNoLonger!");
      return;
    }
    if (club.getQueueSize() > club.getTableNum()) {
      std::cout << "I go home 11 \n";
      return;
    }

    club.clientGoWait(event.getName(), event.getTime());
    return;
  }

  void handleEvent(event::Left& event) {
    if (!club.containsClient(event.getName())) {
      printError(event.getTime(), "ClientUnknown");
      return;
    }

    unsigned tableN = club.removeClient(event.getName(), event.getTime());
    if (tableN != 0)
      club.inviteWaiting(event.getName(), tableN, event.getTime());

    // 12

    return;
  }

  void printError(event::time_point time, std::string error) {
    std::cout << timeToStr(time) << " " << 13 << " " << error << "\n";
  }

 private:
  ComputerClub club;
};

#endif  // MANAGER_HPP_
