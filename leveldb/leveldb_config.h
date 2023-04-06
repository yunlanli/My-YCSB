#ifndef YCSB_LEVELDB_CONFIG_H
#define YCSB_LEVELDB_CONFIG_H

#include "yaml-cpp/yaml.h"
#include <list>
#include <string>

using std::list;
using std::string;

struct LevelDBConfig {
    string data_dir;
    long cache_size;
    bool print_stats;
};

struct LevelDBExperimentConfig {
    struct {
        long key_size;
        long value_size;
        long nr_entry;
    } database;
    struct {
        long nr_warmup_op;
        long nr_op;
        int nr_thread;
        long next_op_interval_ns;
        struct {
            float read;
            float update;
            float insert;
            float scan;
            float read_modify_write;
        } operation_proportion;
        string request_distribution;
        double zipfian_constant;
        list<string> trace_file_list;
        long scan_length;
    } workload;
    LevelDBConfig leveldb;

    static LevelDBExperimentConfig parse_yaml(YAML::Node &root);
};

#endif // YCSB_LEVELDB_CONFIG_H
