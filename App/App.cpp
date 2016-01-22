// App.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include "Enclave_u.h"
#include "sgx_urts.h"

#define ENCLAVE_FILE _T("Enclave.signed.dll")


void print_error(int error, char *msg) {
	printf("error %d: %s\n", error, msg);
}

void wait_for_input() {
	printf("press enter to continue... ");
	getchar();
}

void u_dump_secret(char *secret) {
	printf("Untrusted app received secret: %s\n", secret);
}

// _tmain is a microsoft construct for flexibly enabling unicode support
int _tmain(int argc, char *argv[])
{
	sgx_enclave_id_t enclave_id;
	sgx_launch_token_t launch_token = {0};
	int updated = 0;
	sgx_status_t ret = SGX_SUCCESS;

	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &launch_token, &updated, &enclave_id, NULL);
	if (ret != SGX_SUCCESS) {
		print_error(ret, "could not create enclave");
		return 1;
	}

	// ecall into enclave
	ret = t_request(enclave_id, "dump_secret");
	if (ret != SGX_SUCCESS) {
		print_error(ret, "call into enclave failed");
		return 1;
	}

	ret = sgx_destroy_enclave(enclave_id);
	if (ret != SGX_SUCCESS) {
		print_error(ret, "could not destroy enclave");
		return 1;
	}

	wait_for_input();
	return 0;
}