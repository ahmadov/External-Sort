#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	std::cout << argc << std::endl;
	if (argc != 3) {
		std::cout << "file name and nbytes must be declared" << std::endl;
		return 1;
	}
	std::cout << argv[1] << std::endl;
	std::cout << argv[2] << std::endl;
	int const in { open(argv[1], O_RDONLY) };
	if (in == -1) {
		std::cout << "file doesn't exists" << std::endl;
		return 1;
	}
	int n = atoll(argv[2]) / sizeof(long long);
	long long *rd = new long long[n];
	read(in, rd, atoll(argv[2]));
	close(in);

	for (int i = 0; i < n; ++i) {
		std::cout << rd[i] << std::endl;
	}
	return 0;
}
