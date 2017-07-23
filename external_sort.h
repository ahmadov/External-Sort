


#include <vector>
#include <queue>

class ExternalSort {
public:
	ExternalSort(char* input_filename, char* output_filename, long long size_of_data, long long size_of_memory);
	
	~ExternalSort();		
	
	void sort();
	
private:
	long long size_of_memory;
	long long size_of_data;
	long long number_count_of_data;
	long long number_count_in_chunk;	
	int* number_count_in_file;	
	int number_of_chunks;
	int input_handler;
	int output_handler;
	std::vector<int> temporary_file_handlers;	

	void read_and_sort(int part, int limit);
		
	void merge();
	
	void verify();	
		
	std::priority_queue< std::pair<long long, int>, std::vector<std::pair<long long,int> >, std::greater<std::pair<long long,int> > > pq;
};


