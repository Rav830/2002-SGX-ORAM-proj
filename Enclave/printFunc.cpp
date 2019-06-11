

///////////////////////////
//copied printf from sample enclave
#include "Enclave_t.h"
#include "string.h"
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
int printf(const char* fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}
int snprintf(char* toStore, size_t len, const char* fmt, ...){
	//char buf[BUFSIZ] = { '\0' };
    //ocall_print_int(BUFSIZ);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(toStore, len, fmt, ap);
    va_end(ap);
	
	return (int)len;
}

///////////////////////////////////////////
