#ifndef BASE64_H
#define BASE64_H

char*
base64_encode(
	void *data,
	size_t size
);

void*
base64_decode(
	char *base64_string,
	void **data,
	size_t *size
);

#endif /* end of include guard: BASE64_H */
