#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "query_processor.h"

// ------------------ Create Table ------------------
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

// ------------------ Insert Into Table ------------------
int insertIntoTable(Table **root, const char *query) {
    if (!*root) return ERROR_EMPTY_DATABASE;
    Table *t = *root; // simplified: insert into first table

    Value *row = NULL, *last = NULL;
    char val[64];
    const char *p = strchr(query, '(');
    if (!p) return ERROR_INVALID_QUERY;
    p++;

    while (sscanf(p, "%63[^,)]", val) == 1) {
        Value *v = malloc(sizeof(Value));
        if (!v) return ERROR_MEMORY_ALLOCATION;
        v->value = strdup(val);
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

// ------------------ Select From Table ------------------
int selectFromTable(Table **root, const char *query) {
    if (!*root) return ERROR_EMPTY_DATABASE;
    displayDatabase(*root);
    return SUCCESS;
}

// ------------------ Delete From Table ------------------
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
    } else {
        printf("No rows to delete.\n");
    }
    return SUCCESS;
}

// ------------------ Drop Table ------------------
int dropTable(Table **root, const char *query) {
    if (!*root) return ERROR_EMPTY_DATABASE;
    freeDatabase(root);
    printf("All tables dropped.\n");
    return SUCCESS;
}