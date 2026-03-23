#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "database_handler.h"

// ------------------ Save Database ------------------
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

// ------------------ Load Database ------------------
// Simplified loader: just checks file existence
int loadDatabase(Table **root) {
    FILE *fp = fopen("DATABASE.txt", "r");
    if (!fp) return ERROR_FILE_OPEN;

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fclose(fp);
        return ERROR_EMPTY_DATABASE;
    }
    fclose(fp);
    printf("Database loaded (simplified).\n");
    return SUCCESS;
}

// ------------------ Display Database ------------------
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

// ------------------ Free Database ------------------
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