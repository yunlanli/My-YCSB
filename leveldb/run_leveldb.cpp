#include "leveldb/cache.h"
#include "leveldb_client.h"
#include "leveldb_config.h"
#include "worker.h"
#include "workload.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <config file>\n", argv[0]);
        return -EINVAL;
    }
    YAML::Node file = YAML::LoadFile(argv[1]);
    LevelDBExperimentConfig config = LevelDBExperimentConfig::parse_yaml(file);
    LevelDBConfig dbconfig = config.leveldb;

    OpProportion op_prop;
    op_prop.op[READ] = config.workload.operation_proportion.read;
    op_prop.op[UPDATE] = config.workload.operation_proportion.update;
    op_prop.op[INSERT] = config.workload.operation_proportion.insert;
    op_prop.op[SCAN] = config.workload.operation_proportion.scan;
    op_prop.op[READ_MODIFY_WRITE] =
        config.workload.operation_proportion.read_modify_write;

    leveldb::DB *db;
    leveldb::Options options;
    std::string db_path = dbconfig.data_dir;

    options.block_cache = leveldb::NewLRUCache(dbconfig.cache_size);
    options.create_if_missing = true;

    leveldb::Status s = leveldb::DB::Open(options, db_path, &db);
    if (!s.ok()) {
        std::fprintf(stderr, "open error: %s\n", s.ToString().c_str());
        std::exit(1);
    }

    LevelDBFactory factory{db};

    for (int i = 0; i < 2; ++i) {
        long nr_op;
        if (i == 0) {
            if (config.workload.nr_warmup_op == 0)
                continue;
            nr_op = config.workload.nr_warmup_op;
        } else {
            nr_op = config.workload.nr_op;
        }
        if (config.workload.request_distribution == "uniform") {
            run_uniform_workload_with_op_measurement(
                i == 0 ? "Uniform (Warm-Up)" : "Uniform", &factory,
                config.database.nr_entry, config.database.key_size,
                config.database.value_size, config.workload.scan_length,
                config.workload.nr_thread, op_prop, nr_op,
                config.workload.next_op_interval_ns, nullptr);
        } else if (config.workload.request_distribution == "zipfian") {
            run_zipfian_workload_with_op_measurement(
                i == 0 ? "Zipfian (Warm-Up)" : "Zipfian", &factory,
                config.database.nr_entry, config.database.key_size,
                config.database.value_size, config.workload.scan_length,
                config.workload.nr_thread, op_prop,
                config.workload.zipfian_constant, nr_op,
                config.workload.next_op_interval_ns, nullptr);
        } else if (config.workload.request_distribution == "latest") {
            run_latest_workload_with_op_measurement(
                i == 0 ? "Latest (Warm-Up)" : "Latest", &factory,
                config.database.nr_entry, config.database.key_size,
                config.database.value_size, config.workload.nr_thread,
                op_prop.op[READ], config.workload.zipfian_constant, nr_op,
                config.workload.next_op_interval_ns, nullptr);
        } else if (config.workload.request_distribution == "trace") {
            run_trace_workload_with_op_measurement(
                i == 0 ? "Trace (Warm-Up)" : "Trace", &factory,
                config.database.key_size, config.database.value_size,
                config.workload.nr_thread, config.workload.trace_file_list,
                nr_op, config.workload.next_op_interval_ns, nullptr);
        } else {
            throw std::invalid_argument("unrecognized workload");
        }

        if (dbconfig.print_stats) {
            struct stat {
                std::string property;
                std::string value;
                std::string prefix;
            };
            std::list<stat> stats{
                {.property = "leveldb.stats"},
                {.property = "leveldb.approximate-memory-usage",
                 .prefix = "approximate memory usage (B): "}};

            fprintf(stdout, "=== LevelDB Stats Start ===\n");

            for (auto stat : stats) {
                if (db->GetProperty(leveldb::Slice(stat.property), &stat.value))
                    fprintf(stdout, "%s%s\n", stat.prefix.c_str(),
                            stat.value.c_str());
            }
            fprintf(stdout, "LRU total charge: %zd\n",
                    options.block_cache->TotalCharge());

            fprintf(stdout, "=== LevelDB Stats End ===\n");
        }
    }

    delete db;
    delete options.block_cache;
}