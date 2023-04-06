#ifndef YCSB_LEVELDB_CLIENT_H
#define YCSB_LEVELDB_CLIENT_H

#include <atomic>

#include "client.h"
#include "leveldb/db.h"

struct LevelDBFactory : public ClientFactory {
    std::atomic<int> client_id;
    leveldb::DB *db;

    // TODO: LevelDB constructor, what arguments to take in?
    LevelDBFactory(leveldb::DB *db);
    ~LevelDBFactory();

    Client *create_client() override;
    void destroy_client(Client *client) override;

    void print_stats();

  private:
    leveldb::Options db_opts;
};

struct LevelDBClient : public Client {
    leveldb::DB *db;

    LevelDBClient(LevelDBFactory *factory, int id);
    ~LevelDBClient();

    int do_operation(Operation *op) override;
    int reset() override;
    void close() override;

  private:
    leveldb::Status _leveldb_write(char *key_buffer, char *value_buffer);
    int do_update(char *key_buffer, char *value_buffer);
    int do_insert(char *key_buffer, char *value_buffer);
    int do_read(char *key_buffer, char *value);
    int do_scan(char *key_buffer, long scan_length);
    int do_read_modify_write(char *key_buffer, char *read_reply,
                             char *value_buffer);
};

#endif // YCSB_LEVELDB_CLIENT_H