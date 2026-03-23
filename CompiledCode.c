#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ------------------ Error Codes ------------------
typedef enum {
    SUCCESS = 1,
    ERROR_INVALID_QUERY = -1,
    ERROR_MEMORY_ALLOCATION = -2,
    ERROR_FILE_OPEN = -3,
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
static char *trim(char *s) {
    while (*s == ' ' || *s == '\t') s++;
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\n')) {
        *end = '\0';
        end--;
    }
    return s;
}

// ------------------ Database Functions ------------------
int saveDatabase(Table *root) {
    FILE *fp = fopen("DATABASE.txt", "w");
    if (!fp) return ERROR_FILE_OPEN;

    while (root) {
        fprintf(fp, "%s %d %d\n", root->name, root->cols, root->rows);
        Field *f = root->fields;
        while (f) {
            fprintf(fp, "%s %d\n", f->name, f->datatype);
            f = f->next;
        }
        Value *v = root->values;
        while (v) {
            Value *row = v;
            while (row) {
                fprintf(fp, "%s\t", row->value);
                row = row->next;
            }
            fprintf(fp, "\n");
            v = v->nextRow;
        }
        root = root->next;
    }
    fclose(fp);
    printf("Database saved.\n");
    return SUCCESS;
}

void displayDatabase(Table *root) {
    if (!root) {
        printf("Database is empty.\n");
        return;
    }
    while (root) {
        printf("\nTable: %s (Cols=%d, Rows=%d)\n", root->name, root->cols, root->rows);
        Field *f = root->fields;
        while (f) {
            printf(" %s", f->name);
            if (f->datatype == 1) printf("(int)");
            else if (f->datatype == 2) printf("(float)");
            else printf("(string)");
            printf("\t");
            f = f->next;
        }
        printf("\n");

        Value *v = root->values;
        while (v) {
            Value *row = v;
            while (row) {
                printf("%s\t", row->value);
                row = row->next;
            }
            printf("\n");
            v = v->nextRow;
        }
        root = root->next;
    }
}

void freeDatabase(Table **root) {
    while (*root) {
        Table *t = *root;
        *root = t->next;

        while (t->fields) {
            Field *f = t->fields;
            t->fields = f->next;
            free(f);
        }
        while (t->values) {
            Value *row = t->values;
            t->values = row->nextRow;
            while (row) {
                Value *tmp = row;
                row = row->next;
                free(tmp->value);
                free(tmp);
            }
        }
        free(t);
    }
}

// ------------------ Query Functions ------------------
int createTable(Table **root, Table **current, const char *query) {
    char tableName[32];
    if (sscanf(query, "table %31s", tableName) != 1) return ERROR_INVALID_QUERY;

    Table *t = malloc(sizeof(Table));
    if (!t) return ERROR_MEMORY_ALLOCATION;
    strcpy(t->name, tableName);
    t->rows = 0;
    t->cols = 0;
    t->fields = NULL;
    t->values = NULL;
    t->next = NULL;

    if (!*root) *root = t;
    else (*current)->next = t;
    *current = t;

    printf("Table %s created.\n", tableName);
    return SUCCESS;
}

int insertIntoTable(Table **root, const char *query) {
    if (!*root) return ERROR_EMPTY_DATABASE;
    Table *t = *root; // Simplified: always insert into first table

    Value *row = NULL, *last = NULL;
    char val[64];
    const char *p = strchr(query, '(');
    if (!p) return ERROR_INVALID_QUERY;
    p++;
    while (sscanf(p, "%63[^,)]", val) == 1) {
        Value *v = malloc(sizeof(Value));
        v->value = strdup(trim(val));
        v->next = NULL;
        v->nextRow = NULL;
        if (!row) row = v;
        else last->next = v;
        last = v;

        p = strchr(p, ',');
        if (!p) break;
        p++;
    }
    if (!t->values) t->values = row;
    else {
        Value *tmp = t->values;
        while (tmp->nextRow) tmp = tmp->nextRow;
        tmp->nextRow = row;
    }
    t->rows++;
    printf("Row inserted into %s.\n", t->name);
    return SUCCESS;
}

int selectFromTable(Table **root, const char *query) {
    if (!*root) return ERROR_EMPTY_DATABASE;
    displayDatabase(*root);
    return SUCCESS;
}

int deleteFromTable(Table **root, const char *query) {
    if (!*root) return ERROR_EMPTY_DATABASE;
    Table *t = *root;
    if (t->values) {
        Value *row = t->values;
        t->values = row->nextRow;
        while (row) {
            Value *tmp = row;
            row = row->next;
            free(tmp->value);
            free(tmp);
        }
        t->rows--;
        printf("Row deleted from %s.\n", t->name);
    }
    return SUCCESS;
}

int dropTable(Table **root, const char *query) {
    if (!*root) return ERROR_EMPTY_DATABASE;
    freeDatabase(root);
    printf("All tables dropped.\n");
    return SUCCESS;
}

// ------------------ Main ------------------
int main(void) {
    Table *root = NULL, *current = NULL;

    char query[256];
    while (true) {
        printf("\nEnter Query: ");
        if (!fgets(query, sizeof(query), stdin)) break;
        query[strcspn(query, "\n")] = '\0';

        if (strcasecmp(query, "display") == 0) displayDatabase(root);
        else if (strcasecmp(query, "save") == 0) saveDatabase(root);
        else if (strcasecmp(query, "quit") == 0) {
            freeDatabase(&root);
            break;
        } else if (strncasecmp(query, "create", 6) == 0) createTable(&root, &current, query + 7);
        else if (strncasecmp(query, "insert", 6) == 0) insertIntoTable(&root, query + 7);
        else if (strncasecmp(query, "drop", 4) == 0) dropTable(&root, query + 5);
        else if (strncasecmp(query, "delete", 6) == 0) deleteFromTable(&root, query + 7);
        else if (strncasecmp(query, "select", 6) == 0) selectFromTable(&root, query + 7);
        else printf("Invalid query.\n");
    }

    return SUCCESS;
}