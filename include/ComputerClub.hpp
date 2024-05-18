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
    if (getQueueSize() == 0) return;

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

  void printStat() {
    int i = 1;
    for (auto it = ++tables.begin(), end = tables.end(); it != end; ++it, ++i) {
      std::cout << i << " " << it->getProfit() << " "
                << it->getTotalUsage().count() << "\n";
    }
  }

 private:
  time_point openTime, closeTime;

  using clients_map = std::unordered_map<
      std::string,
      std::pair<unsigned,
                std::list<std::string>::iterator>>;  // "client name"
                                                     // ->
                                                     // [queue::iterator,
                                                     // tableNum]
  clients_map clients;
  std::list<std::string> queue;
  std::vector<Table> tables;
  unsigned freeTables;
};

#endif  // COMPUTERCLUB_HPP_
