#ifndef COMPUTERCLUB_HPP_
#define COMPUTERCLUB_HPP_

#include <chrono>
#include <list>
#include <unordered_set>
#include <vector>

#include "Client.hpp"
#include "Table.hpp"

class ComputerClub {
 public:
  using time_point =
      std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>;

  ComputerClub() = default;

  ComputerClub(unsigned tableNum, time_point openTime, time_point closeTime,
               unsigned pricePerHour)
      : tableNum(tableNum),
        openTime(openTime),
        closeTime(closeTime),
        pricePerHour(pricePerHour),
        tables(tableNum + 1, Table{pricePerHour}),
        freeTables(tableNum) {}

  unsigned getTableNum() const { return tableNum; }
  time_point getOpenTime() const { return openTime; }
  time_point getCloseTime() const { return closeTime; }
  unsigned getPricePerHour() const { return pricePerHour; }

  bool isTableBusy(unsigned tableN) const { return tables[tableN].busy(); }
  std::size_t sizeQueue() const { return queue.size(); }

  bool containsClient(const Client& client) const {
    return clients.contains(client);
  }

  void addClient(const Client& client) { clients.insert(client); }

  void occupyTable(Client& client, unsigned tableN, time_point time) {
    tables[tableN].occupy(time);
    client.curTable = tableN;
    --freeTables;
  }

  void releaseTable(Client& client, unsigned tableN, time_point time) {
    tables[tableN].release(time);
    client.curTable = 0;
    ++freeTables;
  }

  void enqueue(Client& client) { queue.push_back(client); }
  Client& dequeue() { return queue.front(); }
  void pop() { return queue.pop_front(); }

 private:
  unsigned tableNum = 0;
  time_point openTime, closeTime;
  unsigned pricePerHour = 0;

  std::unordered_set<Client> clients;
  std::list<Client> queue;
  std::vector<Table> tables;
  unsigned freeTables;
};

#endif  // COMPUTERCLUB_HPP_
