#include <stdio.h>
#include <stdlib.h>

#define FILENAME "13million.bin"

unsigned long long fold_bits(unsigned long long B) {
	unsigned long long R = 0; // Initial value of R is not disclosed in the paper
	unsigned long long T;
	for (int n = 1; n <= 64; n++) {
		T = B << (64 - n);
		T = T >> 63;
		R = R ^ T;
	}
	return R;
}

int main(void) {
	unsigned long long entropy_pool = 0;	
	int entropy_count = 0;

	FILE *file = fopen(FILENAME, "rb");	
	if (!file) {
		perror("Failed to open file");
		return 1;
	}

	// fair coin
	unsigned long long x64, y64;
	while (fread(&x64, sizeof(unsigned long long), 1, file) == 1 &&
		fread(&y64, sizeof(unsigned long long), 1, file) == 1) {
		unsigned long long a = fold_bits(x64);
		unsigned long long b = fold_bits(y64);
		if (a == b) {
			continue;
		}
		entropy_pool <<= 1;
		if (a == 1)
			entropy_pool |= 1;
		else
			entropy_pool |= 0;
		entropy_count++;
		if (entropy_count % 64 == 0) {
			printf("%llu\n", entropy_pool);
			break;
		}
	}
	if (ferror(file)) {
		perror("Error reading file");
	}
	fclose(file);
	return 0;
}
