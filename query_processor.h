#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include "common.h"

// ------------------ Function Declarations ------------------

// Create a new table
int createTable(Table **root, Table **current, const char *query);

// Insert a row of values into a table
int insertIntoTable(Table **root, const char *query);

// Select and display rows from a table
int selectFromTable(Table **root, const char *query);

// Delete a row from a table
int deleteFromTable(Table **root, const char *query);

// Drop (remove) a table or all tables
int dropTable(Table **root, const char *query);

#endif // QUERY_PROCESSOR_H