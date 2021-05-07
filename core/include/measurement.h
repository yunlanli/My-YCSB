#ifndef YCSB_MEASUREMENT_H
#define YCSB_MEASUREMENT_H

#include "workload.h"
#include "avl_tree.h"
#include <chrono>
#include <atomic>
#include <unordered_map>
#include <list>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>


struct OpMeasurement {
	std::atomic<long> op_count_arr[NR_OP_TYPE];
	std::chrono::steady_clock::time_point start_time;
	std::chrono::steady_clock::time_point end_time;

	std::atomic<long> rt_op_count_arr[NR_OP_TYPE];
	std::chrono::steady_clock::time_point rt_time;

	long max_progress;
	std::atomic<long> cur_progress;
	std::atomic<bool> finished;

	std::unordered_map<int, std::vector<double>[NR_OP_TYPE]> per_client_latency_vec;
	std::vector<double> final_latency_vec[NR_OP_TYPE];

	OpMeasurement();
	void set_max_progress(long new_max_progress);
	void start_measure();
	void finish_measure();

	void record_op(OperationType type, double latency, int id);
	void record_progress(long progress_delta);

	long get_op_count(OperationType type);
	double get_throughput(OperationType type);
	void get_rt_throughput(double *throughput_arr);
	double get_progress_percent();
	double get_latency_average(OperationType type);
	double get_latency_percentile(OperationType type, float percentile);
};

#endif //YCSB_MEASUREMENT_H
