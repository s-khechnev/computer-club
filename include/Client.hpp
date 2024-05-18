#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>

struct Client {
  std::string name;
  unsigned curTable;

  Client(const std::string name, unsigned curTable = 0)
      : name(name), curTable(curTable) {}

  bool isPlaying() const { return curTable == 0; }

  bool operator==(const Client& lhs) { return name == lhs.name; }
};

namespace std {

template <>
struct hash<Client> {
  std::size_t operator()(const Client& client) const {
    return std::hash<std::string>()(client.name);
  }
};

}  // namespace std

#endif  // #ifndef CLIENT_HPP_
