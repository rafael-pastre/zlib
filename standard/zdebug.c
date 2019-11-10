#include "zdebug.h"
#define DEBUG_MODE

int dprintf(int tag, const char* format_str, ...){
#ifdef DEBUG_MODE
	int ret;
	if(!tag) return 1;
    va_list args;
    va_start(args, format_str);
    ret = vprintf(format_str, args);
    va_end(args);
    return ret;
#else
	return 1;
#endif // DEBUG_MODE
}

int debug_ascii_file(int tag, FILE* fp){
#ifdef DEBUG_MODE
	char c;
	if(!tag)return 1;
	while(fscanf(fp, "%c", &c) != EOF){
		printf("%c", c);
	}
#endif // DEBUG_MODE

    return 1;
}

void exit_if_null(void* p){
	if(p == NULL){
		exit(0);
	}
}

void mexit_file_not_opened(void* p, const char* error_message){
	if(p == NULL){
		printf("%s", error_message);
		exit(0);
	}
}
