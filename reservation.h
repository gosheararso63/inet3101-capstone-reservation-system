#ifndef RESERVATION_H
#define RESERVATION_H

#define MAX_ID_LEN 32
#define MAX_NAME_LEN 80
#define MAX_TRIP_LEN 80
#define MAX_STATUS_LEN 20
#define DB_FILE "reservations.gdbm"

/*
 * Reservation record stored in the GDBM database.
 * Each record represents one passenger reservation for an air, train, or bus trip.
 */
typedef struct {
    char id[MAX_ID_LEN];
    char passengerName[MAX_NAME_LEN];
    char tripType[MAX_TRIP_LEN];      /* Air, Train, or Bus */
    char tripNumber[MAX_TRIP_LEN];
    int seatNumber;
    char travelDate[MAX_TRIP_LEN];    /* Example: 2026-05-10 */
    char status[MAX_STATUS_LEN];      /* Confirmed, Cancelled, Waitlisted */
} Reservation;

void showMenu(void);
void createReservation(void);
void readReservation(void);
void updateReservation(void);
void deleteReservation(void);
void listReservations(void);
void searchByPassenger(void);
void generateReport(void);
void trimNewline(char *str);
void readInput(const char *prompt, char *buffer, int size);
int readInt(const char *prompt);
int seatAlreadyBooked(const char *tripNumber, const char *travelDate, int seatNumber, const char *ignoreId);

#endif
