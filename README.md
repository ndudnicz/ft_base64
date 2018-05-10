## Base64 encode/decode implementation in C

#### Encode:
```C
/*
** Take a data array and its size as parameters, calculate the final base64
** string and convert it into base64.
** Return a malloced char * array.
** Possible improvement: remove switch/case and for loop by +=3
*/

char* base64_encode(void* data, size_t size)
```

#### Decode
```C
/*
** Take a base64 string, a void* address and a size_t address as parameters.
** void **data and size_t *size will be used to store new datas.
** Malloc a new data array, convert the base64_string into base10 data, and
** store it in the void **data given as param.
** Store the new size in the size pointer.
** Return the new void* data array.
*/

void* base64_decode(char *base64_string, void **data, size_t *size)
```
