#ifndef MICROLINK_H_
#define MICROLINK_H_

/**
 * STX - SYS_ID - MSG_ID - PAYLOAD - CRC-
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define ML_STX 				0xFE
#define ML_BUFF_LEN 		64

/**
 * MESSAGE DEFAULT DEFINITION
 */
#define MSG_ID_DEFAULT 		1
#define MSG_DEFAULT_LEN		12

typedef struct{
	float 		_microlink_val0;
	float 		_microlink_val1;
	float 		_microlink_val2;
}microlink_msg_default_t;
// END MESSAGE DEFAULT DEFINITION

/**
 * Conversion functions
 */
uint16_t a8tou16(const void *buf)
{
    uint16_t temp = 0;
    memcpy(&temp, buf, sizeof(temp));
    return (uint16_t)temp;
}

uint32_t a8tou32(const void *buf)
{
    uint32_t temp = 0;
    memcpy(&temp, buf, sizeof(temp));
    return (uint32_t)temp;
}

float a8tof(const void *buf)
{
    float temp = 0;
    memcpy(&temp, buf, sizeof(temp));
    return (float)temp;
}

void u16toa8(void *buf, uint16_t x)
{
	 memcpy(buf, &x, sizeof(x));
}

void u32toa8(void *buf, uint32_t x)
{
	 memcpy(buf, &x, sizeof(x));
}

void ftoa8(void *buf, float x)
{
	 memcpy(buf, &x, sizeof(x));
}

/**
 * Hash calculator
 */
uint8_t calculate_hash(uint8_t* buf, uint8_t len)
{
    uint8_t hash = 0x12;

    for (uint8_t i = 1; i < len; i++)
    {
    	hash += buf[i];
    	hash += (hash << 3);
    	hash ^= (hash >> 2);
    }
    hash += (hash << 3);
    hash ^= (hash >> 1);
    hash += (hash << 3);

    return hash;
}

/**
 * Message default encoder
 */
void microlink_msg_encode_default(uint8_t* _msg_buff, uint8_t sys_id, float val0, float val1, float val2)
{
	_msg_buff[0] = ML_STX;
	_msg_buff[1] = sys_id;
	_msg_buff[2] = MSG_ID_DEFAULT;
	ftoa8((_msg_buff + 3), val0);
	ftoa8((_msg_buff + 7), val1);
	ftoa8((_msg_buff + 11), val2);
	_msg_buff[15] = calculate_hash((_msg_buff+3), MSG_DEFAULT_LEN);
}

// here can be your encoder

/**
 * Message decoder (parser)
 */
uint8_t microlink_msg_decode(uint8_t* _msg_buff, microlink_msg_default_t &msg)
{
	switch(_msg_buff[2])
	{
	// MESSAGE DEFAULT parser
	case MSG_ID_DEFAULT:
	{
		uint8_t _tmp[sizeof(float)];

		memcpy(_tmp, (_msg_buff + 3), sizeof(float));
		msg._microlink_val0 = a8tof(_tmp);

		memcpy(_tmp, (_msg_buff + 7), sizeof(float));
		msg._microlink_val1 = a8tof(_tmp);

		memcpy(_tmp, (_msg_buff + 11), sizeof(float));
		msg._microlink_val2 = a8tof(_tmp);

		if(calculate_hash((_msg_buff+3), MSG_DEFAULT_LEN) == _msg_buff[15])
		{
			return 1;
		}
		break;
	}

	// here can be your parser
	default:
		break;
	}

	return 0;
}

#endif /* MICROLINK_H_ */
