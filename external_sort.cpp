#include "external_sort.h"
#include <algorithm>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

ExternalSort::ExternalSort(char* input_filename, char* output_filename, long long size_of_data, long long size_of_memory) {
	this->size_of_data = size_of_data;				// size of data in bytes
	this->size_of_memory = size_of_memory;				// size of memory in bytes

	this->number_count_of_data = this->size_of_data / sizeof(long long);	
	this->number_count_in_chunk = this->size_of_memory / sizeof(long long);  
	this->number_of_chunks = (this->number_count_of_data + this->number_count_in_chunk - 1) / this->number_count_in_chunk;
	this->number_count_in_file = new int[this->number_of_chunks + 1];
	
	this->input_handler = open(input_filename, O_RDONLY);
	this->output_handler = open(output_filename, O_RDWR | O_CREAT, 0666);
}

ExternalSort::~ExternalSort() {
	delete[] number_count_in_file;
	close(this->input_handler);
	close(this->output_handler);
}

void ExternalSort::sort() {
	long long remaining = this->number_count_of_data;
	for (int i = 1; i <= this->number_of_chunks; ++i) {
		this->number_count_in_file[i] = std::min(this->number_count_in_chunk, remaining);
		read_and_sort(i, this->number_count_in_file[i]);
		remaining -= this->number_count_in_chunk;
	}  
	this->merge();
}

void ExternalSort::read_and_sort(int part, int limit) {
	long long* buffer = new long long[limit];
	int data_in_bytes = limit * sizeof(long long);
	read(this->input_handler, buffer, data_in_bytes);

	std::sort(buffer, buffer + limit);

	std::string filename = "tmp_";
	filename += std::to_string(part);
	char* temporary_filename = new char[filename.length() + 1];
	strcpy(temporary_filename, filename.c_str());

	int handler = open(temporary_filename, O_RDWR | O_CREAT, 0666);
	
	write(handler, buffer, data_in_bytes);
	close(handler);

	handler = open(temporary_filename, O_RDONLY);
	this->temporary_file_handlers.push_back(handler);	

	delete[] buffer;
	delete[] temporary_filename;
}

void ExternalSort::merge() {
	int number_count = this->number_count_in_chunk / (this->number_of_chunks + 1);
	int* current_position_in_data = new int[this->number_of_chunks + 1];
	int* number_count_in_data = new int[this->number_of_chunks + 1];
	long long** data = new long long* [this->number_of_chunks + 1];
	for (int i = 1; i <= this->number_of_chunks; ++i) {		
		number_count_in_data[i] = std::min(number_count, this->number_count_in_file[i]);
		data[i] = new long long[number_count_in_data[i]];
		current_position_in_data[i] = 0;
		read(this->temporary_file_handlers[i - 1], data[i], number_count_in_data[i] * sizeof(long long));
		this->pq.push(std::make_pair(data[i][current_position_in_data[i]], i));	
	}	
	long long* buffer = new long long[number_count];
	int count_in_buffer = 0;

	while (!this->pq.empty()) {
		std::pair<long long,int> current = this->pq.top(); 
		this->pq.pop();

		buffer[count_in_buffer++] = current.first;
		if (count_in_buffer == number_count) {
			write(this->output_handler, buffer, count_in_buffer * sizeof(long long));
			count_in_buffer = 0;
		}	
	
		++current_position_in_data[current.second];
		--this->number_count_in_file[current.second];
		
		if (current_position_in_data[current.second] == number_count_in_data[current.second]) {
			if (this->number_count_in_file[current.second] != 0) {
				int nbytes = std::min(number_count, this->number_count_in_file[current.second]);
				read(this->temporary_file_handlers[current.second - 1], data[current.second], nbytes * sizeof(long long));
				current_position_in_data[current.second] = 0;
				number_count_in_data[current.second] = nbytes;
			}
		}	
		if (this->number_count_in_file[current.second] != 0) {
			this->pq.push(std::make_pair(data[current.second][current_position_in_data[current.second]], current.second));
		}
	}
	
	if (count_in_buffer != 0) {
		write(this->output_handler, buffer, count_in_buffer * sizeof(long long));
	}

	for(int i = 1; i <= this->number_of_chunks; ++i) {
		std::string filename = "tmp_";
		filename += std::to_string(i);
		close(this->temporary_file_handlers[i]);
		remove(filename.c_str());
	}
		
	delete[] current_position_in_data;
	delete[] number_count_in_data;
	delete[] data;
}

void ExternalSort::verify() {
	//TODO: implement verify
}


