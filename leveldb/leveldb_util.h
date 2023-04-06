#ifndef YCSB_LEVELDB_UTIL_H
#define YCSB_LEVELDB_UTIL_H

#include <iostream>
#include <stdexcept>

static int _leveldb_client_debug = 0;

template <typename... Args> void dprintln(Args &&...args) {
    if (_leveldb_client_debug)
        (std::cout << ... << args) << '\n';
}

class LevelDBClientError : public std::runtime_error {
  public:
    LevelDBClientError(std::string errMsg) : runtime_error{errMsg} {}
};

#endif // YCSB_LVELDB_UTIL_H