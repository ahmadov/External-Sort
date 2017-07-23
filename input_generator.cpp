#include <iostream>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int const out { open("input.dat", O_WRONLY | O_CREAT, 0666) };
	if (out == -1) {
		std::cout << "error occured" << std::endl;
		return 1;
	}
	long long st = 1LL << 40;
	long long *arr_out = new long long[128];
	for (int i = 1; i < 128; ++i)
		arr_out[i - 1] = st + i;
	std::random_shuffle(arr_out, arr_out + 128);
	write(out, arr_out, sizeof(long long) * 128);
	close(out);
	return 0;
}
