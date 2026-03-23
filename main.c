#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "database_handler.h"
#include "query_processor.h"

int main(void) {
    Table *root = NULL, *current = NULL;
    char query[256];

    printf("Welcome to Personal SQLite-like Database!\n");
    printf("Type commands like: create, insert, select, delete, drop, display, save, quit\n");

    while (true) {
        printf("\nEnter Query: ");
        if (!fgets(query, sizeof(query), stdin)) break;
        query[strcspn(query, "\n")] = '\0'; // remove newline

        if (strcasecmp(query, "display") == 0) {
            displayDatabase(root);
        } else if (strcasecmp(query, "save") == 0) {
            saveDatabase(root);
        } else if (strcasecmp(query, "quit") == 0) {
            freeDatabase(&root);
            printf("Exiting program.\n");
            break;
        } else if (strncasecmp(query, "create", 6) == 0) {
            createTable(&root, &current, query + 7);
        } else if (strncasecmp(query, "insert", 6) == 0) {
            insertIntoTable(&root, query + 7);
        } else if (strncasecmp(query, "drop", 4) == 0) {
            dropTable(&root, query + 5);
        } else if (strncasecmp(query, "delete", 6) == 0) {
            deleteFromTable(&root, query + 7);
        } else if (strncasecmp(query, "select", 6) == 0) {
            selectFromTable(&root, query + 7);
        } else {
            printf("Invalid query.\n");
        }
    }

    return SUCCESS;
}