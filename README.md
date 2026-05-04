# Air/Train/Bus Reservation System (INET 3101 Capstone)

## Problem Statement
This project is a command-line based reservation system designed to simulate how booking systems for air, train, and bus transportation operate. The goal of this application is to allow users to manage reservations efficiently by performing core operations such as creating, viewing, updating, and deleting bookings.

Traditional file-based systems can become inefficient and unstructured when handling multiple records. To address this, this system uses the gdbm database library to provide a more structured and persistent storage solution. The application ensures that reservation data is maintained even after the program is closed and restarted.

---

## Design and Architecture Details
The system is implemented in C and follows a modular and structured design approach. It uses structs and pointers to represent and manage reservation data.

### Key Components:
- **Reservation Struct**  
  Stores all relevant data including passenger name, trip type (air/train/bus), and seat number.

- **gdbm Database Integration**  
  The application uses the GNU dbm (gdbm) library to store records as key-value pairs. This allows efficient storage, retrieval, and updates compared to plain text files.

- **CRUD Operations**
  - Create: Add a new reservation
  - Read: View all reservations
  - Update: Modify an existing reservation
  - Delete: Remove a reservation

- **Menu-Driven CLI Interface**  
  The program uses loops and conditionals to display a user-friendly menu that allows interaction with the system.

- **File Persistence**
  Data is stored in a database file, ensuring that all reservations persist between program executions.

- **Basic Validation**
  The system includes simple validation such as preventing invalid input and handling user selections properly.

---

## Pros and Cons of the Solution

### Pros:
- Efficient data storage using gdbm instead of plain text files
- Persistent storage ensures data is not lost between sessions
- Full CRUD functionality implemented
- Simple and easy-to-use command-line interface
- Modular design using structs and functions improves readability and maintainability

### Cons:
- Limited user interface (CLI only, no GUI)
- Basic validation (could be improved for edge cases like duplicate seat bookings)
- No networking or real-time synchronization between users
- Scalability is limited for very large datasets

---
