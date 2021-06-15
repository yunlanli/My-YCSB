#ifndef YCSB_CLIENT_H
#define YCSB_CLIENT_H

#include <atomic>
#include <stdexcept>

struct ClientFactory;

struct Client {
	int id;
	ClientFactory *factory;

	Client(int id, ClientFactory *factory);
	virtual int do_update(char *key_buffer, char *value_buffer) {
		throw std::invalid_argument("not implemented");
	};
	virtual int do_insert(char *key_buffer, char *value_buffer) {
		throw std::invalid_argument("not implemented");
	};
	virtual int do_read(char *key_buffer, char **value) {
		throw std::invalid_argument("not implemented");
	};
	virtual int do_scan(char *key_buffer, long scan_length) {
		throw std::invalid_argument("not implemented");
	};
	virtual int do_read_modify_write(char *key_buffer, char *value_buffer) {
		throw std::invalid_argument("not implemented");
	};
	virtual int reset() = 0;
	virtual void close() = 0;
};

struct ClientFactory {
	virtual Client *create_client() = 0;
	virtual void destroy_client(Client *client) = 0;
};

#endif //YCSB_CLIENT_H
