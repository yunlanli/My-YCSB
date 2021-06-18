#include <iostream>
#include "worker.h"
#include "redis_client.h"
#include "redis_config.h"
#include "yaml-cpp/yaml.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <config file>\n", argv[0]);
		return -EINVAL;
	}
	YAML::Node file = YAML::LoadFile(argv[1]);
	RedisConfig config = RedisConfig::parse_yaml(file);

	RedisFactory factory(config.redis.addr.c_str(), config.redis.port);

	run_init_workload_with_op_measurement("Initialization",
	                                      &factory,
	                                      config.database.nr_entry,
	                                      config.database.key_size,
	                                      config.database.value_size,
	                                      config.workload.nr_init_thread);
}
