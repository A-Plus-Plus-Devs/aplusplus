#define ERROR_LOG(msg, ...) printf("\033[1;31mERROR:\033[0m line %d - " msg "\n", __LINE__, ##__VA_ARGS__)
