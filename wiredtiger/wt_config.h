#ifndef YCSB_WT_CONFIG_H
#define YCSB_WT_CONFIG_H

#include <string>
#include "yaml-cpp/yaml.h"

using std::string;

struct WiredTigerConfig {
	struct {
		long key_size;
		long value_size;
		long nr_entry;
	} database;
	struct {
		long nr_warmup_op;
		long nr_op;
		int nr_thread;
		struct {
			float read;
			float update;
			float insert;
			float scan;
			float read_modify_write;
		} operation_proportion;
		string request_distribution;
		double zipfian_constant;
		long scan_length;
	} workload;
	struct {
		string data_dir;
		string table_name;
		string conn_config;
		string session_config;
		string cursor_config;
		string create_table_config;
	} wiredtiger;

	static WiredTigerConfig parse_yaml(YAML::Node &root);
};


WiredTigerConfig WiredTigerConfig::parse_yaml(YAML::Node &root) {
	WiredTigerConfig config;
	YAML::Node database = root["database"];
	config.database.key_size = database["key_size"].as<long>();
	config.database.value_size = database["value_size"].as<long>();
	config.database.nr_entry = database["nr_entry"].as<long>();

	YAML::Node workload = root["workload"];
	config.workload.nr_warmup_op = workload["nr_warmup_op"].as<long>();
	config.workload.nr_op = workload["nr_op"].as<long>();
	config.workload.nr_thread = workload["nr_thread"].as<int>();
	YAML::Node operation_proportion = workload["operation_proportion"];
	config.workload.operation_proportion.read = operation_proportion["read"].as<float>();
	config.workload.operation_proportion.update = operation_proportion["update"].as<float>();
	config.workload.operation_proportion.insert = operation_proportion["insert"].as<float>();
	config.workload.operation_proportion.scan = operation_proportion["scan"].as<float>();
	config.workload.operation_proportion.read_modify_write = operation_proportion["read_modify_write"].as<float>();
	config.workload.request_distribution = workload["request_distribution"].as<string>();
	config.workload.zipfian_constant = workload["zipfian_constant"].as<double>();
	config.workload.scan_length = workload["scan_length"].as<long>();

	YAML::Node wiredtiger = root["wiredtiger"];
	config.wiredtiger.data_dir = wiredtiger["data_dir"].as<string>();
	config.wiredtiger.table_name = wiredtiger["table_name"].as<string>();
	config.wiredtiger.conn_config = wiredtiger["conn_config"].as<string>();
	config.wiredtiger.session_config = wiredtiger["session_config"].as<string>();
	config.wiredtiger.cursor_config = wiredtiger["cursor_config"].as<string>();
	config.wiredtiger.create_table_config = wiredtiger["create_table_config"].as<string>();

	return config;
}

#endif //YCSB_WT_CONFIG_H
