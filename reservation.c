#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gdbm.h>
#include "reservation.h"

/* Removes the newline character from input read with fgets. */
void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/* Safely reads string input from the user. */
void readInput(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        trimNewline(buffer);
    }
}

/* Safely reads integer input from the user. */
int readInt(const char *prompt) {
    char input[32];
    int value;

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL && sscanf(input, "%d", &value) == 1) {
            return value;
        }
        printf("Invalid number. Please try again.\n");
    }
}

/* Displays the main menu for the reservation system. */
void showMenu(void) {
    printf("\n========================================\n");
    printf(" Air/Train/Bus Reservation System\n");
    printf("========================================\n");
    printf("1. Create Reservation\n");
    printf("2. View Reservation\n");
    printf("3. Update Reservation\n");
    printf("4. Delete Reservation\n");
    printf("5. List All Reservations\n");
    printf("6. Search by Passenger Name\n");
    printf("7. Generate Simple Report\n");
    printf("0. Exit\n");
    printf("========================================\n");
}

/* Checks whether a seat is already booked for the same trip and date. */
int seatAlreadyBooked(const char *tripNumber, const char *travelDate, int seatNumber, const char *ignoreId) {
    GDBM_FILE db;
    datum key, nextKey, data;
    Reservation reservation;

    db = gdbm_open(DB_FILE, 0, GDBM_READER, 0644, NULL);
    if (db == NULL) {
        return 0; /* No database yet means no seat is booked. */
    }

    key = gdbm_firstkey(db);
    while (key.dptr != NULL) {
        data = gdbm_fetch(db, key);
        if (data.dptr != NULL) {
            memcpy(&reservation, data.dptr, sizeof(Reservation));

            if ((ignoreId == NULL || strcmp(reservation.id, ignoreId) != 0) &&
                strcmp(reservation.tripNumber, tripNumber) == 0 &&
                strcmp(reservation.travelDate, travelDate) == 0 &&
                reservation.seatNumber == seatNumber &&
                strcmp(reservation.status, "Cancelled") != 0) {
                free(data.dptr);
                free(key.dptr);
                gdbm_close(db);
                return 1;
            }
            free(data.dptr);
        }

        nextKey = gdbm_nextkey(db, key);
        free(key.dptr);
        key = nextKey;
    }

    gdbm_close(db);
    return 0;
}

/* CREATE operation: adds a new reservation record to the GDBM database. */
void createReservation(void) {
    GDBM_FILE db;
    Reservation reservation;
    datum key, data;

    db = gdbm_open(DB_FILE, 512, GDBM_WRCREAT, 0644, NULL);
    if (db == NULL) {
        printf("Error: Could not open database.\n");
        return;
    }

    readInput("Enter reservation ID: ", reservation.id, MAX_ID_LEN);

    key.dptr = reservation.id;
    key.dsize = strlen(reservation.id) + 1;

    data = gdbm_fetch(db, key);
    if (data.dptr != NULL) {
        printf("Error: A reservation with this ID already exists.\n");
        free(data.dptr);
        gdbm_close(db);
        return;
    }

    readInput("Enter passenger name: ", reservation.passengerName, MAX_NAME_LEN);
    readInput("Enter trip type Air/Train/Bus: ", reservation.tripType, MAX_TRIP_LEN);
    readInput("Enter trip number: ", reservation.tripNumber, MAX_TRIP_LEN);
    readInput("Enter travel date YYYY-MM-DD: ", reservation.travelDate, MAX_TRIP_LEN);
    reservation.seatNumber = readInt("Enter seat number: ");

    if (seatAlreadyBooked(reservation.tripNumber, reservation.travelDate, reservation.seatNumber, NULL)) {
        strcpy(reservation.status, "Waitlisted");
        printf("Seat is already booked. Reservation will be added as Waitlisted.\n");
    } else {
        strcpy(reservation.status, "Confirmed");
    }

    data.dptr = (char *)&reservation;
    data.dsize = sizeof(Reservation);

    if (gdbm_store(db, key, data, GDBM_INSERT) == 0) {
        printf("Reservation created successfully. Status: %s\n", reservation.status);
    } else {
        printf("Error: Reservation could not be saved.\n");
    }

    gdbm_close(db);
}

/* READ operation: finds and displays one reservation by reservation ID. */
void readReservation(void) {
    GDBM_FILE db;
    char id[MAX_ID_LEN];
    datum key, data;
    Reservation reservation;

    db = gdbm_open(DB_FILE, 0, GDBM_READER, 0644, NULL);
    if (db == NULL) {
        printf("Error: Database does not exist yet.\n");
        return;
    }

    readInput("Enter reservation ID to view: ", id, MAX_ID_LEN);
    key.dptr = id;
    key.dsize = strlen(id) + 1;

    data = gdbm_fetch(db, key);
    if (data.dptr == NULL) {
        printf("Reservation not found.\n");
    } else {
        memcpy(&reservation, data.dptr, sizeof(Reservation));
        printf("\nReservation ID: %s\n", reservation.id);
        printf("Passenger Name: %s\n", reservation.passengerName);
        printf("Trip Type: %s\n", reservation.tripType);
        printf("Trip Number: %s\n", reservation.tripNumber);
        printf("Travel Date: %s\n", reservation.travelDate);
        printf("Seat Number: %d\n", reservation.seatNumber);
        printf("Status: %s\n", reservation.status);
        free(data.dptr);
    }

    gdbm_close(db);
}

/* UPDATE operation: updates an existing reservation record. */
void updateReservation(void) {
    GDBM_FILE db;
    char id[MAX_ID_LEN];
    datum key, data;
    Reservation reservation;

    db = gdbm_open(DB_FILE, 512, GDBM_WRCREAT, 0644, NULL);
    if (db == NULL) {
        printf("Error: Could not open database.\n");
        return;
    }

    readInput("Enter reservation ID to update: ", id, MAX_ID_LEN);
    key.dptr = id;
    key.dsize = strlen(id) + 1;

    data = gdbm_fetch(db, key);
    if (data.dptr == NULL) {
        printf("Reservation not found.\n");
        gdbm_close(db);
        return;
    }

    memcpy(&reservation, data.dptr, sizeof(Reservation));
    free(data.dptr);

    printf("Updating reservation for %s. Press Enter after each new value.\n", reservation.passengerName);
    readInput("Enter new passenger name: ", reservation.passengerName, MAX_NAME_LEN);
    readInput("Enter new trip type Air/Train/Bus: ", reservation.tripType, MAX_TRIP_LEN);
    readInput("Enter new trip number: ", reservation.tripNumber, MAX_TRIP_LEN);
    readInput("Enter new travel date YYYY-MM-DD: ", reservation.travelDate, MAX_TRIP_LEN);
    reservation.seatNumber = readInt("Enter new seat number: ");

    if (seatAlreadyBooked(reservation.tripNumber, reservation.travelDate, reservation.seatNumber, reservation.id)) {
        strcpy(reservation.status, "Waitlisted");
        printf("Seat is already booked. Updated reservation status is Waitlisted.\n");
    } else {
        readInput("Enter status Confirmed/Cancelled/Waitlisted: ", reservation.status, MAX_STATUS_LEN);
    }

    data.dptr = (char *)&reservation;
    data.dsize = sizeof(Reservation);

    if (gdbm_store(db, key, data, GDBM_REPLACE) == 0) {
        printf("Reservation updated successfully.\n");
    } else {
        printf("Error: Reservation could not be updated.\n");
    }

    gdbm_close(db);
}

/* DELETE operation: removes a reservation from the GDBM database. */
void deleteReservation(void) {
    GDBM_FILE db;
    char id[MAX_ID_LEN];
    datum key;

    db = gdbm_open(DB_FILE, 512, GDBM_WRCREAT, 0644, NULL);
    if (db == NULL) {
        printf("Error: Could not open database.\n");
        return;
    }

    readInput("Enter reservation ID to delete: ", id, MAX_ID_LEN);
    key.dptr = id;
    key.dsize = strlen(id) + 1;

    if (gdbm_delete(db, key) == 0) {
        printf("Reservation deleted successfully.\n");
    } else {
        printf("Reservation not found or could not be deleted.\n");
    }

    gdbm_close(db);
}

/* Lists all reservations stored in the database. */
void listReservations(void) {
    GDBM_FILE db;
    datum key, nextKey, data;
    Reservation reservation;
    int count = 0;

    db = gdbm_open(DB_FILE, 0, GDBM_READER, 0644, NULL);
    if (db == NULL) {
        printf("Error: Database does not exist yet.\n");
        return;
    }

    printf("\n%-12s %-20s %-10s %-12s %-12s %-8s %-12s\n",
           "ID", "Passenger", "Type", "Trip", "Date", "Seat", "Status");
    printf("--------------------------------------------------------------------------------\n");

    key = gdbm_firstkey(db);
    while (key.dptr != NULL) {
        data = gdbm_fetch(db, key);
        if (data.dptr != NULL) {
            memcpy(&reservation, data.dptr, sizeof(Reservation));
            printf("%-12s %-20s %-10s %-12s %-12s %-8d %-12s\n",
                   reservation.id,
                   reservation.passengerName,
                   reservation.tripType,
                   reservation.tripNumber,
                   reservation.travelDate,
                   reservation.seatNumber,
                   reservation.status);
            count++;
            free(data.dptr);
        }
        nextKey = gdbm_nextkey(db, key);
        free(key.dptr);
        key = nextKey;
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("Total reservations: %d\n", count);
    gdbm_close(db);
}

/* Searches reservations by passenger name using substring matching. */
void searchByPassenger(void) {
    GDBM_FILE db;
    datum key, nextKey, data;
    Reservation reservation;
    char searchName[MAX_NAME_LEN];
    int found = 0;

    db = gdbm_open(DB_FILE, 0, GDBM_READER, 0644, NULL);
    if (db == NULL) {
        printf("Error: Database does not exist yet.\n");
        return;
    }

    readInput("Enter passenger name to search: ", searchName, MAX_NAME_LEN);

    key = gdbm_firstkey(db);
    while (key.dptr != NULL) {
        data = gdbm_fetch(db, key);
        if (data.dptr != NULL) {
            memcpy(&reservation, data.dptr, sizeof(Reservation));
            if (strstr(reservation.passengerName, searchName) != NULL) {
                printf("\nReservation ID: %s | Passenger: %s | Trip: %s %s | Date: %s | Seat: %d | Status: %s\n",
                       reservation.id,
                       reservation.passengerName,
                       reservation.tripType,
                       reservation.tripNumber,
                       reservation.travelDate,
                       reservation.seatNumber,
                       reservation.status);
                found = 1;
            }
            free(data.dptr);
        }
        nextKey = gdbm_nextkey(db, key);
        free(key.dptr);
        key = nextKey;
    }

    if (!found) {
        printf("No matching passengers found.\n");
    }

    gdbm_close(db);
}

/* Generates a simple report by counting confirmed, waitlisted, and cancelled reservations. */
void generateReport(void) {
    GDBM_FILE db;
    datum key, nextKey, data;
    Reservation reservation;
    int total = 0, confirmed = 0, waitlisted = 0, cancelled = 0;

    db = gdbm_open(DB_FILE, 0, GDBM_READER, 0644, NULL);
    if (db == NULL) {
        printf("Error: Database does not exist yet.\n");
        return;
    }

    key = gdbm_firstkey(db);
    while (key.dptr != NULL) {
        data = gdbm_fetch(db, key);
        if (data.dptr != NULL) {
            memcpy(&reservation, data.dptr, sizeof(Reservation));
            total++;
            if (strcmp(reservation.status, "Confirmed") == 0) {
                confirmed++;
            } else if (strcmp(reservation.status, "Waitlisted") == 0) {
                waitlisted++;
            } else if (strcmp(reservation.status, "Cancelled") == 0) {
                cancelled++;
            }
            free(data.dptr);
        }
        nextKey = gdbm_nextkey(db, key);
        free(key.dptr);
        key = nextKey;
    }

    printf("\nReservation Report\n");
    printf("------------------\n");
    printf("Total reservations: %d\n", total);
    printf("Confirmed: %d\n", confirmed);
    printf("Waitlisted: %d\n", waitlisted);
    printf("Cancelled: %d\n", cancelled);

    gdbm_close(db);
}
