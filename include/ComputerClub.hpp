#ifndef COMPUTERCLUB_HPP_
#define COMPUTERCLUB_HPP_

#include <functional>
#include <iostream>
#include <list>
#include <optional>
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

  std::size_t getNumTables() const { return tables.size() - 1; }
  time_point getOpenTime() const { return openTime; }
  time_point getCloseTime() const { return closeTime; }
  std::size_t getFreeTables() const { return freeTables; }
  std::size_t getQueueSize() const { return queue.size(); }

  bool isTableBusy(unsigned tableN) const { return tables[tableN].busy(); }

  void addClient(const std::string& client) {
    clients[client] = {{}, queue.end()};
  }

  bool containsClient(const std::string& client) const {
    return clients.contains(client);
  }

  bool isClientPlaying(const std::string& client) const {
    return clients.find(client)->second.first.has_value();
  }

  std::optional<unsigned> getTableNumByName(const std::string& client) const {
    return clients.find(client)->second.first;
  }

  std::optional<unsigned> removeClient(const std::string& client,
                                       time_point time) {
    const auto& [tableN, queueIt] = clients.find(client)->second;
    if (!tableN) {
      if (queueIt != queue.end()) queue.erase(queueIt);
      clients.erase(client);
      return {};
    }

    unsigned tableNum = *tableN;
    releaseTable(client, tableNum, time);
    clients.erase(client);
    return tableNum;
  }

  std::optional<std::string> occupyTableFromQueue(unsigned tableN,
                                                  time_point time) {
    if (!getQueueSize()) return {};

    std::string& waiting = queue.front();
    occupyTable(waiting, tableN, time);
    queue.pop_front();
    return waiting;
  }

  void clientGoWait(const std::string& client, time_point time) {
    if (isClientPlaying(client))
      releaseTable(client, *getTableNumByName(client), time);
    enqueue(client);
  }

  void occupyTable(const std::string& client, unsigned tableN,
                   time_point time) {
    if (isClientPlaying(client))
      releaseTable(client, *getTableNumByName(client), time);

    tables[tableN].occupy(time);
    clients[client].first = tableN;
    --freeTables;
  }

  void releaseTable(const std::string& client, unsigned tableN,
                    time_point time) {
    tables[tableN].release(time);
    clients[client].first = {};
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

  auto getPlayingClients() {
    std::vector<std::pair<std::reference_wrapper<const std::string>,
                          unsigned>>
        result;  // [name, tableN]
    result.reserve(getNumTables() - freeTables);
    for (const auto& [client, info] : clients)
      if (info.first) result.emplace_back(std::cref(client), *info.first);

    return result;
  }

 private:
  time_point openTime, closeTime;

  using client_map = std::unordered_map<
      std::string,
      std::pair<std::optional<unsigned>,
                std::list<std::string>::iterator>>;  // "client name"
                                                     // ->
                                                     // [tableNum,
                                                     // queue::iterator]
  client_map clients{};
  std::list<std::string> queue{};
  std::vector<Table> tables;  // 0's table is dummy
  unsigned freeTables;
};

#endif  // COMPUTERCLUB_HPP_
