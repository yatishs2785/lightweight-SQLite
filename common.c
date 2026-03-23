#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

// ------------------ Error Codes ------------------
typedef enum {
    SUCCESS = 1,
    ERROR_INVALID_QUERY = -1,
    ERROR_MEMORY_ALLOCATION = -2,
    ERROR_FILE_OPEN = -3,
    ERROR_FILE_DELETE = -4,
    ERROR_FIELD_COUNT = -5,
    ERROR_EMPTY_DATABASE = -6,
    ERROR_UNKNOWN_TABLE = -7,
    ERROR_INVALID_DATATYPE = -8
} ErrorCode;

// ------------------ Structures ------------------
typedef struct Field {
    char name[32];
    int datatype; // 1=int, 2=float, 3=string
    struct Field *next;
} Field;

typedef struct Value {
    char *value;
    struct Value *next;
    struct Value *nextRow;
} Value;

typedef struct Table {
    char name[32];
    int rows;
    int cols;
    Field *fields;
    Value *values;
    struct Table *next;
} Table;

// ------------------ Utility ------------------
char *trim(char *s);

#endif // COMMON_H