#include <iostream>

#include "external_sort.cpp"

int main() {
	ExternalSort* instance = new ExternalSort("input.dat","output.dat",1024, 512);
	instance->sort();
	return 0;
}
