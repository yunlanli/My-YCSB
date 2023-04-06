#include "leveldb/cache.h"
#include "leveldb_client.h"
#include "leveldb_config.h"
#include "worker.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <config file>\n", argv[0]);
        return -EINVAL;
    }

    YAML::Node file = YAML::LoadFile(argv[1]);
    LevelDBExperimentConfig config = LevelDBExperimentConfig::parse_yaml(file);
    LevelDBConfig dbconfig = config.leveldb;

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

    run_init_workload_with_op_measurement(
        "Initialization", &factory, config.database.nr_entry,
        config.database.key_size, config.database.value_size, 1);
    return 0;
}