#ifndef YCSB_WORKER_H
#define YCSB_WORKER_H

#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include "measurement.h"
#include "client.h"
#include "workload.h"

void worker_thread_fn(Client *client, Workload *workload, OpMeasurement *measurement);
void monitor_thread_fn(const char *task, OpMeasurement *measurement);
void run_init_workload(ClientFactory *factory, long nr_entry, long key_size, long value_size);
void run_random_workload(const char *task, ClientFactory *factory, long nr_entry, long key_size, long value_size, int nr_thread, double read_ratio, long nr_op);

#endif //YCSB_WORKER_H
