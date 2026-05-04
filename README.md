# Air/Train/Bus Reservation System

## Problem Statement

This project is an Air/Train/Bus Reservation System built in C for the INET 3101 Capstone Project. The application solves the problem of managing passenger reservations for different types of transportation, including air, train, and bus trips. A user can create a new reservation, view an existing reservation, update reservation details, delete a reservation, list all reservations, search by passenger name, and generate a basic reservation report.

The system also helps prevent overbooking by checking whether a seat is already reserved for the same trip number and travel date. If the seat is already taken, the new reservation is automatically marked as waitlisted instead of confirmed. This gives the program a simple but realistic reservation workflow.

## Design and Architecture Details

The project is written in C and uses the GNU dbm, or gdbm, library as the database storage system. Instead of storing records in a plain text file, each reservation is saved as a structured record inside a `.gdbm` database file. This meets the capstone requirement to use a database with full CRUD support.

The main data structure is a `Reservation` struct. It stores the reservation ID, passenger name, trip type, trip number, seat number, travel date, and reservation status. The reservation ID is used as the key in the gdbm database, while the full `Reservation` struct is stored as the value.

The program is separated into multiple files for cleaner organization:

- `main.c` contains the main menu loop and calls the correct function based on the user's choice.
- `reservation.h` contains constants, the `Reservation` struct, and function prototypes.
- `reservation.c` contains the main application logic, including create, read, update, delete, list, search, and report functions.
- `Makefile` compiles the project and links the program with the gdbm library.

The application uses a command-line menu with loops and switch statements. It uses structs to model reservation data and uses pointers indirectly through gdbm's `datum` structure when storing and retrieving records. The CRUD operations are implemented through gdbm functions such as `gdbm_store`, `gdbm_fetch`, `gdbm_delete`, `gdbm_firstkey`, and `gdbm_nextkey`.

## Pros and Cons of Your Solution

One strength of this solution is that it fully supports CRUD operations using the gdbm library. This makes the project stronger than a plain text file implementation because records are stored in a database file and accessed by unique reservation ID keys. The program is also menu-driven, so it is easy for a user to understand and test during a demo video. Another strength is the seat-checking feature, which prevents two confirmed reservations from using the same seat on the same trip and date.

Another advantage is that the project is split into multiple files. This makes the code easier to read, maintain, and explain. The use of comments also helps document what each major function does.

A weakness of this solution is that it is still a command-line program, so it does not have a graphical interface. The system also does not include a full network/socket feature for remote availability lookup. Another limitation is that it uses simple string matching and does not support advanced filtering by date range, trip type, or passenger history. In the future, the project could be improved by adding socket-based client/server support, file locking for stronger concurrency protection, better input validation, and CSV report export.

## How to Compile and Run

Install gdbm if it is not already installed:

```bash
sudo apt update
sudo apt install libgdbm-dev
```

Compile the project:

```bash
make
```

Run the program:

```bash
./reservation_system
```

Clean build files and the database:

```bash
make clean
```

## Demo Video Checklist

For the demo video, show the following features:

1. Compile the program using `make`.
2. Run the program using `./reservation_system`.
3. Create a confirmed reservation.
4. Create another reservation using the same trip, date, and seat to show that it becomes waitlisted.
5. View a reservation by ID.
6. Update a reservation.
7. List all reservations.
8. Search by passenger name.
9. Generate the reservation report.
10. Delete a reservation.
11. Exit the program without errors.
=======
# inet3101-capstone-reservation-system
C-based Air/Train/Bus Reservation System with gdbm database integration, supporting full CRUD operations, seat management, and persistent storage.
