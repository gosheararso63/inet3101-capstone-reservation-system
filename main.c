#include <stdio.h>
#include "reservation.h"

/*
 * INET 3101 Capstone Project
 * Air/Train/Bus Reservation System
 * Main program loop for the CLI application.
 */
int main(void) {
    int choice;

    do {
        showMenu();
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                createReservation();
                break;
            case 2:
                readReservation();
                break;
            case 3:
                updateReservation();
                break;
            case 4:
                deleteReservation();
                break;
            case 5:
                listReservations();
                break;
            case 6:
                searchByPassenger();
                break;
            case 7:
                generateReport();
                break;
            case 0:
                printf("Exiting reservation system. Goodbye.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
