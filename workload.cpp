#include "workload.h"

Workload::Workload(long key_size, long value_size)
: key_size(key_size), value_size(value_size) {
	;
}

RandomWorkload::RandomWorkload(long key_size, long value_size, long nr_entry,
			       long nr_op, double read_ratio, unsigned int seed)
: Workload(key_size, value_size), nr_entry(nr_entry), nr_op(nr_op), read_ratio(read_ratio), seed(seed), cur_nr_op(0) {
	sprintf(this->key_format, "%%0%ldld", key_size - 1);
}

bool RandomWorkload::has_next_op() {
	return this->cur_nr_op < this->nr_op;
}

void RandomWorkload::next_op(OperationType *type, char *key_buffer, char *value_buffer) {
	if (!this->has_next_op())
		throw std::invalid_argument("does not have next op");
	bool read = this->generate_random_double() <= this->read_ratio;
	if (read)
		*type = GET;
	else
		*type = SET;
	long key = this->generate_random_long() % this->nr_entry;
	this->generate_key_string(key_buffer, key);
	if (!read)
		this->generate_value_string(value_buffer);
	++this->cur_nr_op;
}

void RandomWorkload::generate_key_string(char *key_buffer, long key) {
	sprintf(key_buffer, this->key_format, key);
}

void RandomWorkload::generate_value_string(char *value_buffer) {
	for (int i = 0; i < this->value_size - 1; ++i) {
		value_buffer[i] = 'a' + (rand_r(&this->seed) % ('z' - 'a' + 1));
	}
	value_buffer[this->value_size - 1] = '\0';
}

long RandomWorkload::generate_random_long() {
	return (((long)rand_r(&this->seed)) << (sizeof(int) * 8)) | rand_r(&this->seed);
}

double RandomWorkload::generate_random_double() {
	return ((double)rand_r(&this->seed)) / RAND_MAX;
}

InitWorkload::InitWorkload(long nr_entry, long key_size, long value_size, unsigned int seed)
: Workload(key_size, value_size), nr_entry(nr_entry), cur_nr_entry(0), seed(seed) {
	sprintf(this->key_format, "%%0%ldld", key_size - 1);
}

bool InitWorkload::has_next_op() {
	return this->cur_nr_entry < this->nr_entry;
}

void InitWorkload::next_op(OperationType *type, char *key_buffer, char *value_buffer) {
	if (!this->has_next_op())
		throw std::invalid_argument("does not have next op");
	*type = SET;
	this->generate_key_string(key_buffer, this->cur_nr_entry++);
	this->generate_value_string(value_buffer);
}

void InitWorkload::generate_key_string(char *key_buffer, long key) {
	sprintf(key_buffer, this->key_format, key);
}

void InitWorkload::generate_value_string(char *value_buffer) {
	for (int i = 0; i < this->value_size - 1; ++i) {
		value_buffer[i] = 'a' + (rand_r(&this->seed) % ('z' - 'a' + 1));
	}
	value_buffer[this->value_size - 1] = '\0';
}
