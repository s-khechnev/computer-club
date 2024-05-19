#ifndef COMPUTERCLUB_HPP_
#define COMPUTERCLUB_HPP_

#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "Table.hpp"

class ComputerClub {
 public:
  using time_point = event::time_point;

  ComputerClub() = default;

  ComputerClub(unsigned tableNum, time_point openTime, time_point closeTime,
               unsigned pricePerHour)
      : openTime(openTime),
        closeTime(closeTime),
        tables(tableNum + 1, Table{pricePerHour}),
        freeTables(tableNum) {}

  std::size_t getNumTables() const { return tables.size(); }
  time_point getOpenTime() const { return openTime; }
  time_point getCloseTime() const { return closeTime; }
  std::size_t getFreeTables() const { return freeTables; }
  std::size_t getQueueSize() const { return queue.size(); }

  bool isTableBusy(unsigned tableN) const { return tables[tableN].busy(); }

  void addClient(const std::string& client) { clients[client].first = 0; }

  bool containsClient(const std::string& client) const {
    return clients.contains(client);
  }

  bool isClientPlaying(const std::string& client) const {
    return (clients.find(client)->second.first) != 0;
  }

  unsigned removeClient(const std::string& client, time_point time) {
    auto clientIt = clients.find(client)->second;
    if (!isClientPlaying(client)) {
      queue.erase(clientIt.second);
      return 0;
    }

    unsigned tableN = clientIt.first;
    releaseTable(client, tableN, time);
    clients.erase(client);
    return tableN;
  }

  void inviteWaiting(const std::string& client, unsigned tableN,
                     time_point time) {
    std::string waiting = queue.front();
    occupyTable(waiting, tableN, time);
    queue.pop_front();
  }

  void clientGoWait(const std::string& client, time_point time) {
    if (isClientPlaying(client))
      releaseTable(client, clients.find(client)->second.first, time);
    enqueue(clients.find(client)->first);
  }

  void occupyTable(const std::string& client, unsigned tableN,
                   time_point time) {
    if (isClientPlaying(client))
      releaseTable(client, clients.find(client)->second.first, time);

    tables[tableN].occupy(time);
    auto clientIt = clients.find(client);
    clientIt->second.first = tableN;
    --freeTables;
  }

  void releaseTable(const std::string& client, unsigned tableN,
                    time_point time) {
    tables[tableN].release(time);
    clients[client].first = 0;
    ++freeTables;
  }

  void enqueue(const std::string& client) { queue.push_back(client); }

  void printStatistics() {
    int i = 0;
    for (auto it = ++tables.begin(), end = tables.end(); it != end; ++it) {
      std::cout << ++i << " " << it->getProfit() << " "
                << minutesToStr(it->getTotalUsage()) << "\n";
    }
  }

  std::vector<std::pair<std::reference_wrapper<const std::string>, unsigned>>
  getClients() {
    std::vector<std::pair<std::reference_wrapper<const std::string>, unsigned>>
        result;
    result.reserve(clients.size());
    for (auto& [client, info] : clients)
      result.push_back({std::cref(client), info.first});
    return result;
  }

 private:
  time_point openTime, closeTime;

  using client_map = std::unordered_map<
      std::string,
      std::pair<unsigned,
                std::list<std::string>::iterator>>;  // "client name"
                                                     // ->
                                                     // [tableNum,
                                                     // queue::iterator]
  client_map clients;
  std::list<std::string> queue;
  std::vector<Table> tables;
  unsigned freeTables;
};

#endif  // COMPUTERCLUB_HPP_
