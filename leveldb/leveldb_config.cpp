#include "leveldb_config.h"
#include <cstdio>

LevelDBExperimentConfig LevelDBExperimentConfig::parse_yaml(YAML::Node &root) {
    LevelDBExperimentConfig config;
    YAML::Node database = root["database"];
    config.database.key_size = database["key_size"].as<long>();
    config.database.value_size = database["value_size"].as<long>();
    config.database.nr_entry = database["nr_entry"].as<long>();

    YAML::Node workload = root["workload"];
    config.workload.nr_warmup_op = workload["nr_warmup_op"].as<long>();
    config.workload.nr_op = workload["nr_op"].as<long>();
    config.workload.nr_thread = workload["nr_thread"].as<int>();
    config.workload.next_op_interval_ns =
        workload["next_op_interval_ns"].as<long>();
    YAML::Node operation_proportion = workload["operation_proportion"];
    config.workload.operation_proportion.read =
        operation_proportion["read"].as<float>();
    config.workload.operation_proportion.update =
        operation_proportion["update"].as<float>();
    config.workload.operation_proportion.insert =
        operation_proportion["insert"].as<float>();
    config.workload.operation_proportion.scan =
        operation_proportion["scan"].as<float>();
    config.workload.operation_proportion.read_modify_write =
        operation_proportion["read_modify_write"].as<float>();
    config.workload.request_distribution =
        workload["request_distribution"].as<string>();
    config.workload.zipfian_constant =
        workload["zipfian_constant"].as<double>();
    YAML::Node trace_file_list = workload["trace_file_list"];

    for (YAML::iterator iter = trace_file_list.begin();
         iter != trace_file_list.end(); ++iter)
        config.workload.trace_file_list.push_back((*iter).as<string>());
    config.workload.scan_length = workload["scan_length"].as<long>();
    YAML::Node leveldb = root["leveldb"];
    config.leveldb.data_dir = leveldb["data_dir"].as<string>();
    config.leveldb.cache_size = leveldb["cache_size"].as<long>();
    config.leveldb.print_stats = leveldb["print_stats"].as<bool>();

    return config;
}