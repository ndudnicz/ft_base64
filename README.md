## Base64 encode/decode implementation in C

#### Encode:
```C
char* base64_encode(void* data, size_t size)
```

#### Decode
```C
void* base64_decode(char *base64_string, void **data, size_t *size)
```
