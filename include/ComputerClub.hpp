#ifndef COMPUTERCLUB_HPP_
#define COMPUTERCLUB_HPP_

#include <chrono>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

#include "Table.hpp"

class ComputerClub {
 public:
  using time_point =
      std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>;

  ComputerClub() = default;

  ComputerClub(unsigned tableNum, time_point openTime, time_point closeTime,
               unsigned pricePerHour)
      : openTime(openTime),
        closeTime(closeTime),
        tables(tableNum + 1, Table{pricePerHour}),
        freeTables(tableNum) {}

  unsigned getTableNum() const { return tables.size(); }
  time_point getOpenTime() const { return openTime; }
  time_point getCloseTime() const { return closeTime; }
  unsigned getFreeTables() const { return freeTables; }
  std::size_t getQueueSize() const { return queue.size(); }

  bool isTableBusy(unsigned tableN) const { return tables[tableN].busy(); }

  void addClient(const std::string& client) { clients[client] = 0; }

  bool containsClient(const std::string& client) const {
    return clients.contains(client);
  }

  bool isClientPlaying(const std::string& client) const {
    return (clients.find(client)->second) != 0;
  }

  void removeClientAndInviteWaiting(const std::string& client,
                                    time_point time) {
    if (!isClientPlaying(client)) {
      // fix me
      queue.remove(client);
      return;
    }

    unsigned tableN = clients.find(client)->second;
    releaseTable(client, tableN, time);
    clients.erase(client);

    if (getQueueSize() == 0) return;

    std::string waiting = queue.front();
    occupyTable(waiting, tableN, time);
    queue.pop_front();
  }

  void clientGoWait(const std::string& client, time_point time) {
    if (isClientPlaying(client))
      releaseTable(client, clients.find(client)->second, time);
    enqueue(clients.find(client)->first);
  }

  void occupyTable(const std::string& client, unsigned tableN,
                   time_point time) {
    if (isClientPlaying(client))
      releaseTable(client, clients.find(client)->second, time);

    tables[tableN].occupy(time);
    auto clientIt = clients.find(client);
    clientIt->second = tableN;
    --freeTables;
  }

  void releaseTable(const std::string& client, unsigned tableN,
                    time_point time) {
    tables[tableN].release(time);
    clients[client] = 0;
    ++freeTables;
  }

  void enqueue(const std::string& client) { queue.push_back(client); }

  void printStat() {
    int i = 1;
    for (auto it = ++tables.begin(), end = tables.end(); it != end; ++it, ++i) {
      std::cout << i << " " << it->getProfit() << " "
                << it->getTotalUsage().count() << "\n";
    }
  }

  void close() {
    for (auto& table : tables) {
      if (table.busy()) table.release(closeTime);
    }
  }

 private:
  time_point openTime, closeTime;

  std::unordered_map<std::string, unsigned> clients;
  std::list<std::string> queue;
  std::vector<Table> tables;
  unsigned freeTables;
};

#endif  // COMPUTERCLUB_HPP_
