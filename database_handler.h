#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include "common.h"

// ------------------ Function Declarations ------------------

// Save all tables and rows to DATABASE.txt
int saveDatabase(Table *root);

// Load database from DATABASE.txt (simplified loader)
int loadDatabase(Table **root);

// Display all tables, fields, and rows
void displayDatabase(Table *root);

// Free all allocated memory for tables, fields, and values
void freeDatabase(Table **root);

#endif // DATABASE_HANDLER_H