#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define A_LEFT 0xFC
#define A_RIGHT 0x03

#define B_LEFT 0xF0
#define B_RIGHT 0x0F

#define C_LEFT 0xC0
#define C_RIGHT 0x3F

#define LENGTH(X) (X * 15 / 10)

static const char arr[64] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'0','1','2','3','4','5','6','7','8','9','+','/'
};


char*
base64_encode(
	void* data,
	size_t size
)
{
	char		*base64_data = (char*)calloc(1, LENGTH(size) + 1);
	uint8_t		*data_uint8 = (uint8_t*)data;
	uint8_t		remain = 0;
	size_t		i, n;

	for (i = 0, n=0; i < size - 2; i+=3, n+=3) {
		// switch (i % 3) {
		// 	case 0:
		// 	printf("A");
		// 	base64_data[n] = arr[ (data_uint8[i] & A_LEFT) >> 2 ];
		// 	remain = data_uint8[i] & A_RIGHT;
		// 	break;
        //
		// 	case 1:
		// 	printf("B");
		// 	base64_data[n] = arr[ ((data_uint8[i] & B_LEFT) >> 4) | (remain << 4) ];
		// 	remain = data_uint8[i] & B_RIGHT;
		// 	break;
        //
		// 	case 2:
		// 	printf("C");
		// 	base64_data[n] = arr[ ((data_uint8[i] & C_LEFT) >> 6) | (remain << 2) ];
		// 	n++;
		// 	base64_data[n] = arr[ data_uint8[i] & C_RIGHT ];
		// 	break;
        //
		// 	default:
		// 	break;
		// }
		base64_data[n] = arr[ (data_uint8[i] & A_LEFT) >> 2 ];
		remain = data_uint8[i] & A_RIGHT;

		// if (i + 1 < size) {
			base64_data[n+1] = arr[ ((data_uint8[i+1] & B_LEFT) >> 4) | (remain << 4) ];
			remain = data_uint8[i] & B_RIGHT;
		// }

		// if (i + 2 < size) {
			base64_data[n+2] = arr[ ((data_uint8[i+2] & C_LEFT) >> 6) | (remain << 2) ];
			n++;
			base64_data[n+2] = arr[ data_uint8[i+2] & C_RIGHT ];
		// }
		// break;

	}
	printf("i:%d n:%d\n",i, n );
	// i-=3;
	// n-=3;
	if (i + 1 < size) {
		printf("A\n");
		base64_data[n+1] = arr[ ((data_uint8[i+1] & B_LEFT) >> 4) | (remain << 4) ];
		remain = data_uint8[i+1] & B_RIGHT;
	}

	if (i + 2 < size) {
		printf("B\n");
		base64_data[n+2] = arr[ ((data_uint8[i+2] & C_LEFT) >> 6) | (remain << 2) ];
		n++;
		base64_data[n+2] = arr[ data_uint8[i+2] & C_RIGHT ];
	}

	if (size % 3) {
		base64_data[n+2] = arr[ size % 3 == 1 ? remain << 4 : remain << 2 ];
	}
	return base64_data;
}

char*
base64_decode(
	void* data,
	size_t size
)
{
	return NULL;
}

int		main(int ac, char **av)
{
	printf("\n%s\n", base64_encode(av[1], strlen(av[1])));
	return (0);
}
