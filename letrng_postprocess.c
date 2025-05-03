#include <stdio.h>
#include <stdlib.h>

#define FILE_SOURCE "13million.bin"
#define FILE_DESTINATION "postprocessed.bin"

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

int open_files(char* rname, FILE** read,
		char* wname, FILE** write) {
	*read = fopen(rname, "rb");
	if (!*read) {
		return 1;
	}
	*write = fopen(wname, "wb");
	if (!*write) {
		fclose(*read);
		return 1;
	}
	return 0;
}

int main(void) {
	unsigned long long entropy_pool = 0;	
	int entropy_count = 0;
	FILE *src, *dst;

	int res = open_files(FILE_SOURCE, &src, FILE_DESTINATION, &dst);
	if (res) {
		perror("Failed to open files!");
		return 1;
	}

	// fair coin
	unsigned long long x64, y64;
	while (fread(&x64, sizeof(unsigned long long), 1, src) == 1 &&
		fread(&y64, sizeof(unsigned long long), 1, src) == 1) {
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
			size_t written = fwrite(&entropy_pool, sizeof(unsigned long long), 1, dst);
			if (1 != written) {
				perror("Error writing to file!");
				goto cleanup;
			}
			entropy_count = 0;
		}
	}

cleanup:
	fclose(dst);
	fclose(src);
	return 0;
}
