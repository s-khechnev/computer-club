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
          handleEvent(static_cast<event::Come&>(*e));
          break;
        case event::Id::Sit:
          handleEvent(static_cast<event::Sit&>(*e));
          break;
        case event::Id::Wait:
          handleEvent(static_cast<event::Wait&>(*e));
          break;
        case event::Id::Left:
          handleEvent(static_cast<event::Left&>(*e));
          break;
      }
    }
  }

  void closeClub() {
    auto clients = club.getClients();
    std::sort(clients.begin(), clients.end(),
              [](auto x, auto y) { return x.first.get() < y.first.get(); });
    for (const auto& [client, tableN] : clients) {
      if (club.isClientPlaying(client))
        club.releaseTable(client, tableN, club.getCloseTime());
      printClientLeft(client, club.getCloseTime());
    }

    std::cout << timeToStr(club.getCloseTime()) << "\n";
  }

  void printStatistics() { club.printStatistics(); }

 private:
  void handleEvent(event::Come& event) {
    std::cout << event << "\n";
    if (club.containsClient(event.getName())) {
      printError("YouShallNotPass", event.getTime());
      return;
    }
    if (event.getTime() < club.getOpenTime()) {
      printError("NotOpenYet", event.getTime());
      return;
    }

    club.addClient(event.getName());
  }

  void handleEvent(event::Sit& event) {
    std::cout << event << "\n";
    if (club.isTableBusy(event.getTableNum())) {
      printError("PlaceIsBusy", event.getTime());
      return;
    }
    if (!club.containsClient(event.getName())) {
      printError("ClientUnknown", event.getTime());
      return;
    }

    club.occupyTable(event.getName(), event.getTableNum(), event.getTime());
    return;
  }

  void handleEvent(event::Wait& event) {
    std::cout << event << "\n";
    if (club.getFreeTables() > 0) {
      printError("ICanWaitNoLonger!", event.getTime());
      return;
    }
    if (club.getQueueSize() > club.getNumTables()) {
      printClientLeft(event.getName(), event.getTime());
      return;
    }

    club.clientGoWait(event.getName(), event.getTime());
    return;
  }

  void handleEvent(event::Left& event) {
    std::cout << event << "\n";
    if (!club.containsClient(event.getName())) {
      printError("ClientUnknown", event.getTime());
      return;
    }

    auto tableN = club.removeClient(event.getName(), event.getTime());
    if (!tableN) return;

    if (auto waitingClient =
            club.occupyTableFromQueue(*tableN, event.getTime()))
      printClientSitDown(*waitingClient, *tableN, event.getTime());
  }

  void printClientLeft(const std::string& name, event::time_point time) {
    std::cout << timeToStr(time) << " " << event::OutId::Left << " " << name
              << "\n";
  }

  void printClientSitDown(const std::string& name, unsigned tableN,
                          event::time_point time) {
    std::cout << timeToStr(time) << " " << event::OutId::Sit << " " << name
              << " " << tableN << "\n";
  }

  void printError(std::string error, event::time_point time) {
    std::cout << timeToStr(time) << " " << event::OutId::Error << " " << error
              << "\n";
  }

 private:
  ComputerClub club;
};

#endif  // MANAGER_HPP_
