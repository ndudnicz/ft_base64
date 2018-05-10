#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
** Bytes:                A               B               C
**               |_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _|
**               |_ _ _ _ _ _|_ _|_ _ _ _|_ _ _ _|_ _|_ _ _ _ _ _|
** Base64           A_LEFT     ^  B_LEFT     ^   C_LEFT    ^
** masks:                   A_RIGHT       B_RIGHT       C_RIGHT
*/

/* masks 8 bits */
/* 11111100 */
#define A_LEFT 0xFC
/* 00000011 */
#define A_RIGHT 0x03

/* 11110000 */
#define B_LEFT 0xF0
/* 00001111 */
#define B_RIGHT 0x0F

/* 11000000 */
#define C_LEFT 0xC0
/* 00111111 */
#define C_RIGHT 0x3F

/* masks 6 bits */
/* 00110000 */
#define A_LEFT_6BITS 0x30
/* 00001111 */
#define A_RIGHT_6BITS 0x0F

/* 00111100 */
#define B_6BITS 0x3C

/* 00000011 */
#define C_LEFT_6BITS 0x03


/* get the base64 final string length */
#define BASE64_LENGTH(X) (((X - 1) / 3) + X + 1 + (X%3 ? (X%3 == 1 ? 2 : 1) : 0))

#define DATA_LENGTH(X) (X * 3 / 4)

static const char DATA_TO_B64[64] = {
	'A','B','C','D','E','F','G','H','I','J',
	'K','L','M','N','O','P','Q','R','S','T',
	'U','V','W','X','Y','Z','a','b','c','d',
	'e','f','g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v','w','x',
	'y','z','0','1','2','3','4','5','6','7',
	'8','9','+','/'
};

static char B64_TO_DATA[128] = {
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0,62, 0, 0, 0,63,52,53,
	54,55,56,57,58,59,60,61, 0, 0,
	 0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
	 5, 6, 7, 8, 9,10,11,12,13,14,
	15,16,17,18,19,20,21,22,23,24,
	25, 0, 0, 0, 0, 0, 0,26,27,28,
	29,30,31,32,33,34,35,36,37,38,
	39,40,41,42,43,44,45,46,47,48,
	49,50,51, 0, 0, 0, 0, 0
};

/*
** Take a data array and its size as parameters, calculate the final base64
** string and convert it into base64.
** Return a malloced char * array.
** Possible improvement: remove switch/case and for loop by +=3
*/

char*
base64_encode(
	void *data,
	size_t size
)
{
	char		*base64_data;
	char		*data_uint8 = (char*)data;
	uint8_t		remain = 0;
	size_t		i, n;

	if (!(base64_data = (char*)calloc(1, BASE64_LENGTH(size) + 1))) {
		return NULL;
	}
	else {
		for (i = 0, n = 0; i < size; i += 1, n += 1) {
			switch (i % 3) {
				case 0 /* A */ :
				base64_data[n] = DATA_TO_B64[ (data_uint8[i] & A_LEFT) >> 2 ];
				remain = data_uint8[i] & A_RIGHT;
				break;

				case 1 /* B */ :
				base64_data[n] = DATA_TO_B64[ ((data_uint8[i] & B_LEFT) >> 4) | (remain << 4) ];
				remain = data_uint8[i] & B_RIGHT;
				break;

				case 2 /* C */ :
				base64_data[n] = DATA_TO_B64[ ((data_uint8[i] & C_LEFT) >> 6) | (remain << 2) ];
				n++;
				base64_data[n] = DATA_TO_B64[ data_uint8[i] & C_RIGHT ];
				break;
			}
		}
		if (size % 3 == 1) {
			base64_data[n] = DATA_TO_B64[ remain << 4 ];
			base64_data[n+1] = '=';
			base64_data[n+2] = '=';
		} else if (size % 3 == 2) {
			base64_data[n] = DATA_TO_B64[ remain << 2 ];
			base64_data[n+1] = '=';
		}
		return base64_data;
	}
}

/*
** Malloc a new data array, convert the base64_string into base10 data, and
** store it in the void **data given as param.
** Store the new size in the size pointer given as parameter
** Return the new void* data array.
*/

void*
base64_decode(
	char *base64_string,
	void **data,
	size_t *size
)
{
	size_t const			string_len = strlen(base64_string);
	uint8_t const			completion = base64_string[string_len - 1] == '=' ? (base64_string[string_len - 2] == '=' ? 2 : 1): 0;
	size_t const			string_base64_len = string_len - completion;
	*size = DATA_LENGTH(string_base64_len);
	uint8_t *const			data_uint8 = (uint8_t*)calloc(1, sizeof(uint8_t) * *size);
	uint8_t const *const	base64_string_uint8 = (uint8_t*)base64_string;
	size_t					i, n;

	if (!data_uint8) {
		return NULL;
	} else {
		for (i = 0, n = 0; n < string_len; i += 1) {
			if (base64_string_uint8[n] > 127) {free(data_uint8); return NULL;}
			switch (i % 3) {
				case 0 /* A */ :
				data_uint8[i] |= B64_TO_DATA[ base64_string_uint8[n] ] << 2 ;
				n++;
				if (base64_string_uint8[n] > 127) {free(data_uint8); return NULL;}
				data_uint8[i] |= (B64_TO_DATA[ base64_string_uint8[n] ] & A_LEFT_6BITS) >> 4;
				break;

				case 1 /* B */ :
				data_uint8[i] |= (B64_TO_DATA[ base64_string_uint8[n] ] & A_RIGHT_6BITS) << 4;
				n++;
				if (base64_string_uint8[n] > 127) {free(data_uint8); return NULL;}
				data_uint8[i] |= (B64_TO_DATA[ base64_string_uint8[n] ] & B_6BITS) >> 2;
				break;

				case 2 /* C */ :
				data_uint8[i] |= (B64_TO_DATA[ base64_string_uint8[n] ] & C_LEFT_6BITS) << 6;
				n++;
				if (base64_string_uint8[n] > 127) {free(data_uint8); return NULL;}
				data_uint8[i] |= B64_TO_DATA[ base64_string_uint8[n] ];
				n++;
				break;
			}
		}
		*data = data_uint8;
		return *data;
	}
}
