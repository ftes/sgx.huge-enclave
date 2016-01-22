#include "Enclave_t.h"
#include "sgx_trts.h"
#include <stdio.h>
#include <stdlib.h>

// use buffer larger than 100MiB (current HW only supports EPC of ~90MiB size)
#define BUF_SIZE 700 * 1024 * 1024

// HeapMaxSize = 256 MiB (0x10000000 Enclave.config.xml)

void t_request(char *request) {
	char *buf = (char*) malloc(sizeof(char) * BUF_SIZE);
	if (buf == NULL) {
		u_dump_secret("could not get enough memory");
		return;
	}

	for (int i=0; i<BUF_SIZE; i++) {
		buf[i] = 'x';
	}
	
	//snprintf(buf, BUF_SIZE, "received %s: ENCLAVE_SECRET\n", request);

	// check whether entire array was populated
	for (int i=0; i<BUF_SIZE; i+=1024*1024) {
		buf[i+10] = '\0';
		// ocall containing enclave secret (not a good idea for productive use)
		u_dump_secret(buf + i);
	}
}