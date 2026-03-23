Personal SQLite‑like Database in C
A lightweight, educational database engine written in modern C (C11).
This project mimics basic SQL functionality — CREATE, INSERT, SELECT, DELETE, DROP, SAVE, and DISPLAY — using custom data structures and file persistence.

✨ Features
- Create tables with names and fields
- Insert rows of values
- Select and display table contents
- Delete rows
- Drop tables
- Save/load database from DATABASE.txt
- Simple command‑line query interface

📂 Project Structure
src/
  main.c                # Entry point, query loop
  database_handler.c    # Load, save, display, free
  query_processor.c     # Create, insert, select, delete, drop

include/
  common.h              # Shared structs, enums, utilities
  database_handler.h    # Database function declarations
  query_processor.h     # Query function declarations

DATABASE.txt            # Saved database file
queries.sql             # Sample query file with 50+ commands
Makefile                # Build instructions
README.md               # Documentation



⚙️ Build & Run
make
./personal-sqlite



📋 Sample Queries
create table Students
insert into Students values (Alice, 21, CS)
insert into Students values (Bob, 22, Math)
display
select * from Students
delete from Students where name=Alice
drop table Students
save
quit


For a larger demo, see queries.sql which contains 50+ queries across multiple tables (Students, Courses, Teachers).

📖 Example Output
Running display after inserting rows into Students:
Table: Students (Cols=0, Rows=2)
 Alice    21    CS
 Bob      22    Math



🚀 Future Enhancements
- Support for field definitions with datatypes (int, float, string)
- WHERE clause filtering in SELECT and DELETE
- Multiple tables with relationships
- Improved error handling and query parsing

🎯 Motivation
This project is for learning purposes — to understand how databases work under the hood, practice modern C programming, and build a portfolio‑ready project for GitHub.
