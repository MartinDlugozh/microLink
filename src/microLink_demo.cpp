#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "microLink.h"

int main(int argc, char* argv[])
{
	uint8_t ml_msg[ML_BUFF_LEN];
	microlink_msg_default_t test_in_msg;

	microlink_msg_encode_default(ml_msg, 0, 333.66, 12.2, 36.665);
	fprintf(stdout, "Decode & check: %i \r\n", microlink_msg_decode(ml_msg, test_in_msg));

	fprintf(stdout, "%g \r\n", test_in_msg._microlink_val0);
	fprintf(stdout, "%g \r\n", test_in_msg._microlink_val1);
	fprintf(stdout, "%g \r\n", test_in_msg._microlink_val2);

	return 0;
}
