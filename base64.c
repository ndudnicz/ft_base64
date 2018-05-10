#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
** Bytes:                A               B               C
**               |_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _|
**               |_ _ _ _ _ _|_ _|_ _ _ _|_ _ _ _|_ _|_ _ _ _ _ _|
** Base64           A_LEFT     ^  B_LEFT     ^   C_LEFT    ^
**  masks:                  A_RIGHT       B_RIGHT       C_RIGHT
*/

#define A_LEFT 0xFC
#define A_RIGHT 0x03

#define B_LEFT 0xF0
#define B_RIGHT 0x0F

#define C_LEFT 0xC0
#define C_RIGHT 0x3F

/* get the base64 final string length */
#define BASE64_LENGTH(X) (((X - 1) / 3) + X + 1 + (X%3 ? (X%3 == 1 ? 2 : 1) : 0))

static const char arr[64] = {
	'A','B','C','D','E','F','G','H','I','J',
	'K','L','M','N','O','P','Q','R','S','T',
	'U','V','W','X','Y','Z','a','b','c','d',
	'e','f','g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v','w','x',
	'y','z','0','1','2','3','4','5','6','7',
	'8','9','+','/'
};

/*
** Take a data array and its size as parameters, calculate the final base64
** string and convert it into base64.
** Return a malloced char * array.
** Possible improvement: remove switch/case and for loop by +=3
*/
char*
base64_encode(
	void* data,
	size_t size
)
{
	char		*base64_data;
	uint8_t		*data_uint8 = (uint8_t*)data;
	uint8_t		remain = 0;
	size_t		i, n;

	if (!(base64_data = (char*)calloc(1, BASE64_LENGTH(size) + 1))) {
		return NULL;
	}
	else {
		for (i = 0, n = 0; i < size; i += 1, n += 1) {
			switch (i % 3) {
				case 0 /* A */ :
				base64_data[n] = arr[ (data_uint8[i] & A_LEFT) >> 2 ];
				remain = data_uint8[i] & A_RIGHT;
				break;

				case 1 /* B */ :
				base64_data[n] = arr[ ((data_uint8[i] & B_LEFT) >> 4) | (remain << 4) ];
				remain = data_uint8[i] & B_RIGHT;
				break;

				case 2 /* C */ :
				base64_data[n] = arr[ ((data_uint8[i] & C_LEFT) >> 6) | (remain << 2) ];
				n++;
				base64_data[n] = arr[ data_uint8[i] & C_RIGHT ];
				break;

				default:
				break;
			}
		}
		if (size % 3 == 1) {
			base64_data[n] = arr[ remain << 4 ];
			base64_data[n+1] = '=';
			base64_data[n+2] = '=';
		} else if (size % 3 == 2) {
			base64_data[n] = arr[ remain << 2 ];
			base64_data[n+1] = '=';
		}
		return base64_data;
	}
}

void*
base64_decode(
	char* string
)
{
	size_t const	string_len = strlen(string);
	return NULL;
}

int		main(int ac, char **av)
{
	printf("%s\n", base64_encode(av[1], strlen(av[1])));
	return (0);
}
