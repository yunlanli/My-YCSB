#include "leveldb_client.h"
#include "client.h"
#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "leveldb_util.h"
#include <cstring>
#include <stdexcept>

LevelDBFactory::LevelDBFactory(leveldb::DB *db) : client_id{0} {
    if (db == nullptr) {
        throw std::invalid_argument{
            "Can't create a LevelDBFactory with a nullptr."};
    }

    this->db = db;
}

LevelDBFactory::~LevelDBFactory() {}

Client *LevelDBFactory::create_client() {
    return new LevelDBClient(this, this->client_id++);
}

void LevelDBFactory::destroy_client(Client *client) {
    delete (LevelDBClient *)client;
}

// LevelDBClient
LevelDBClient::LevelDBClient(LevelDBFactory *factory, int id)
    : Client(id, factory) {
    this->db = factory->db;
}

LevelDBClient::~LevelDBClient() {}

int LevelDBClient::reset() {
    // TODO
    return 0;
}

void LevelDBClient::close() {
    // TODO
}

int LevelDBClient::do_operation(Operation *op) {
    switch (op->type) {
    case UPDATE:
        return this->do_update(op->key_buffer, op->value_buffer);
    case INSERT:
        return this->do_insert(op->key_buffer, op->value_buffer);
    case READ:
        return this->do_read(op->key_buffer, op->reply_value_buffer);
    case SCAN:
        return this->do_scan(op->key_buffer, op->scan_length);
    case READ_MODIFY_WRITE:
        return this->do_read_modify_write(
            op->key_buffer, op->reply_value_buffer, op->value_buffer);
    default:
        throw std::invalid_argument("invalid op type");
    }
}

leveldb::Status LevelDBClient::_leveldb_write(char *key_buffer,
                                              char *value_buffer) {
    leveldb::WriteOptions writeOpts{};
    leveldb::Slice key{key_buffer}, value{value_buffer};

    return this->db->Put(writeOpts, key, value);
}

int LevelDBClient::do_update(char *key_buffer, char *value_buffer) {
    leveldb::Status s = this->_leveldb_write(key_buffer, value_buffer);
    if (!s.ok()) {
        char errMsg[100];
        sprintf(errMsg, "update key %s failed: leveldb write returned %s",
                key_buffer, s.ToString().c_str());
        throw LevelDBClientError{errMsg};
    }

    dprintln("updated key-value pair: (", key_buffer, ", ", value_buffer, ")");
    return 0;
}

int LevelDBClient::do_insert(char *key_buffer, char *value_buffer) {
    leveldb::Status s = this->_leveldb_write(key_buffer, value_buffer);
    if (!s.ok()) {
        char errMsg[100];
        sprintf(errMsg, "insert key %s failed: leveldb write returned %s",
                key_buffer, s.ToString().c_str());
        throw LevelDBClientError{errMsg};
    }

    dprintln("inserted key-value pair: (", key_buffer, ", ", value_buffer, ")");
    return 0;
}

// what return value to use when the key is not found?
// return value is unchecked.
int LevelDBClient::do_read(char *key_buffer, char *value) {
    leveldb::ReadOptions readOpts{};
    leveldb::Slice key{key_buffer};
    std::string _value;
    leveldb::Status s;

    dprintln("leveldbclient::do_read");

    s = this->db->Get(readOpts, key, &_value);
    if (!s.ok()) {
        if (s.IsNotFound()) {
            dprintln("leveldb read key not found: ", key_buffer);
            return -1;
        } else {
            char errMsg[100];
            sprintf(errMsg, "read key %s failed: leveldb read returned %s",
                    key_buffer, s.ToString().c_str());
            throw LevelDBClientError{errMsg};
        }
    }

    // assume the *value array has the same size as the value entry
    // so that strncpy would always copy exactly n bytes from src into dest
    strncpy(value, _value.c_str(), _value.size() + 1);
    dprintln("leveldb read succeeded: ", key.ToString(), " -> ", _value);

    return 0;
}

int LevelDBClient::do_scan(char *key_buffer, long scan_length) {
    leveldb::ReadOptions readOpts;
    leveldb::Iterator *iter;
    leveldb::Slice key{key_buffer}, value;
    leveldb::Status s;
    long iterCount = 0;

    iter = this->db->NewIterator(readOpts);
    iter->Seek(key);

    // TODO: Operation doesn't have a value buffer for SCAN
    // are we expected to simply iterate through the keys starting
    // from key_buffer, and stop when we has iterated @scan_length
    // keys? What if we reached the last key before hitting @scan_length?
    while (iter->Valid() && iterCount++ < scan_length) {
        key = iter->key();
        value = iter->value();
        dprintln("scan from ", key_buffer, " (", iterCount, "/", scan_length,
                 "): ", key.ToString(), " -> ", value.ToString());

        iter->Next();
    }

    s = iter->status();
    if (!s.ok()) {
        char errMsg[100];
        sprintf(errMsg,
                "scan from key %s failed: during scanning, leveldb returned %s",
                key_buffer, s.ToString().c_str());
        throw LevelDBClientError{errMsg};
    }

    delete iter;
    return 0;
}

int LevelDBClient::do_read_modify_write(char *key_buffer, char *read_reply,
                                        char *value_buffer) {
    this->do_read(key_buffer, read_reply);
    return this->do_update(key_buffer, value_buffer);
}