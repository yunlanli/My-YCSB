#ifndef YCSB_REDIS_CLIENT_H
#define YCSB_REDIS_CLIENT_H

#include <cstring>
#include "client.h"
#include <hiredis/hiredis.h>

struct RedisFactory;

struct RedisClient : public Client {
	redisContext *redis_context;
	redisReply *last_reply;

	RedisClient(RedisFactory *factory, int id);
	~RedisClient();
	int do_operation(Operation *op) override;
	int reset() override;
	void close() override;

private:
	int do_update(char *key_buffer, char *value_buffer);
	int do_insert(char *key_buffer, char *value_buffer);
	int do_read(char *key_buffer, char **value);
	void set_last_reply(redisReply *reply);
};

struct RedisFactory : public ClientFactory {
	const char *redis_addr;
	const int redis_port;
	std::atomic<int> client_id;

	RedisFactory(const char *redis_addr, int redis_port);
	RedisClient *create_client() override;
	void destroy_client(Client *client) override;
};

#endif //YCSB_REDIS_CLIENT_H
