#ifndef MANAGER_HPP_
#define MANAGER_HPP_

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "ComputerClub.hpp"
#include "Event.hpp"
#include "Utils.hpp"

class Manager {
 public:
  Manager(ComputerClub club) : club(club) {}

  void processEvents(const std::vector<std::unique_ptr<event::Base>>& events) {
    std::cout << timeToStr(club.getOpenTime()) << "\n";

    for (auto& e : events) {
      switch (e->getId()) {
        case event::Id::Come:
          handleEvent(dynamic_cast<event::Come&>(*e));
          break;
        case event::Id::Sit:
          handleEvent(dynamic_cast<event::Sit&>(*e));
          break;
        case event::Id::Wait:
          handleEvent(dynamic_cast<event::Wait&>(*e));
          break;
        case event::Id::Left:
          handleEvent(dynamic_cast<event::Left&>(*e));
          break;
      }
    }
  }

  void closeClub() {
    auto clients = club.getClients();
    std::sort(clients.begin(), clients.end(),
              [](auto x, auto y) { return x.first.get() < y.first.get(); });
    for (auto& [client, tableN] : clients) {
      if (club.isClientPlaying(client)) {
        club.releaseTable(client, tableN, club.getCloseTime());
        printClientLeft(club.getCloseTime(), client);
      }
    }

    std::cout << timeToStr(club.getCloseTime()) << "\n";
  }

  void printStatistics() { club.printStatistics(); }

 private:
  void handleEvent(event::Come& event) {
    std::cout << timeToStr(event.getTime()) << " " << event.getId() << " "
              << event.getName() << "\n";
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

  void handleEvent(event::Sit& event) {
    std::cout << timeToStr(event.getTime()) << " " << event.getId() << " "
              << event.getName() << " " << event.getTableNum() << "\n";
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
    std::cout << timeToStr(event.getTime()) << " " << event.getId() << " "
              << event.getName() << "\n";
    if (club.getFreeTables() > 0) {
      printError(event.getTime(), "ICanWaitNoLonger!");
      return;
    }
    if (club.getQueueSize() > club.getNumTables()) {
      printClientLeft(event.getTime(), event.getName());
      return;
    }

    club.clientGoWait(event.getName(), event.getTime());
    return;
  }

  void handleEvent(event::Left& event) {
    std::cout << timeToStr(event.getTime()) << " " << event.getId() << " "
              << event.getName() << "\n";
    if (!club.containsClient(event.getName())) {
      printError(event.getTime(), "ClientUnknown");
      return;
    }

    unsigned tableN = club.removeClient(event.getName(), event.getTime());
    if (tableN != 0 && club.getQueueSize() != 0) {
      club.inviteWaiting(event.getName(), tableN, event.getTime());
      printClientSitDown(event.getTime(), event.getName(), tableN);
    }

    return;
  }

  void printClientLeft(event::time_point time, const std::string& name) {
    constexpr int outId = 11;
    std::cout << timeToStr(time) << " " << outId << " " << name << "\n";
  }

  void printClientSitDown(event::time_point time, const std::string& name,
                          unsigned tableN) {
    constexpr int outId = 12;
    std::cout << timeToStr(time) << " " << outId << " " << name << " " << tableN
              << "\n";
  }

  void printError(event::time_point time, std::string error) {
    constexpr int outId = 13;
    std::cout << timeToStr(time) << " " << outId << " " << error << "\n";
  }

 private:
  ComputerClub club;
};

#endif  // MANAGER_HPP_
