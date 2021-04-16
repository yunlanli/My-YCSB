#ifndef YCSB_WORKLOAD_H
#define YCSB_WORKLOAD_H

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>

enum OperationType {
	SET = 0,
	GET,
	NR_OP_TYPE,
};

struct Workload {
	long key_size;
	long value_size;

	Workload(long key_size, long value_size);
	virtual void next_op(OperationType *type, char *key_buffer, char *value_buffer) = 0;
	virtual bool has_next_op() = 0;
};

struct RandomWorkload : public Workload {
	/* configuration */
	long nr_entry;
	long nr_op;
	double read_ratio;

	/* constants */
	static constexpr int key_format_len = 64;

	/* states */
	unsigned int seed;
	long cur_nr_op;
	char key_format[key_format_len];

	RandomWorkload(long key_size, long value_size, long nr_entry, long nr_op, double read_ratio, unsigned int seed);
	void next_op(OperationType *type, char *key_buffer, char *value_buffer) override;
	bool has_next_op() override;

private:
	void generate_key_string(char *key_buffer, long key);
	void generate_value_string(char *value_buffer);
	long generate_random_long();
	double generate_random_double();
};

struct InitWorkload : public Workload {
	/* configuration */
	long nr_entry;

	/* constants */
	static constexpr int key_format_len = 64;

	/* states */
	unsigned int seed;
	long cur_nr_entry;
	char key_format[key_format_len];

	InitWorkload(long nr_entry, long key_size, long value_size, unsigned int seed);
	void next_op(OperationType *type, char *key_buffer, char *value_buffer) override;
	bool has_next_op() override;
private:
	void generate_key_string(char *key_buffer, long key);
	void generate_value_string(char *value_buffer);
};

#endif //YCSB_WORKLOAD_H
