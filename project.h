#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEHICLES 1000
#define MAX_VIOLATIONS 1000
#define MAX_LOCATIONS 50

extern int vehicleCount;
extern int violationCount;
extern int locationCount;

typedef struct DateTime {

    int day, month, year;

    int hour, minute;

} DateTime;



typedef struct Vehicle {

    char regNo[20];

    char ownerName[50];

    char contact[20];

    char address[100];



    float totalFineAccrued;

    float totalFinePaid;

    float pendingAmount;

    int violationCount;



    struct Vehicle *next;

} Vehicle;



typedef struct Violation {

    int violationID;

    char vehicleRegNo[20];

    int violationType; // 1: Illegal Parking, 2: Over-speeding, 3: Red-light Jump, 4: Helmet/Seatbelt

    int locationID;

    DateTime dateTime;

    float fineAmount;

    int isPaid; // 0 = Pending, 1 = Paid



    struct Violation *next;

} Violation;



typedef struct Location {

    int locationID;

    char locationName[100];



    struct Location *next;

} Location;



typedef struct Payment {

    int transactionID;

    int violationID;

    float amountPaid;

    char modeOfPayment[20];

    DateTime date;



    struct Payment *next; // Top pointer for Stack

} Payment;

Vehicle *findVehicle(Vehicle *head, const char regNo[]);
Location *findLocation(Location *head, int locationID);
Violation *findViolation(Violation *head, int violationID);
float calculateFine(int violationType);
void displayViolationType(int type);
int getUnpaidCount(Vehicle *v, Violation *violationHead);
int countLocationViolations(int locationID, Violation *violationHead);
float calculateLocationRevenue(int locationID, Violation *violationHead);
Vehicle *copyVehicleList(Vehicle *head, Violation *violationHead);
void freeVehicleList(Vehicle *head);
Vehicle *insertVehicle(Vehicle *head, Vehicle *newNode);
Vehicle *createVehicleInteractive(void);
void displayVehicles(Vehicle *head);
Violation *insertViolation(Violation *head, Violation *newNode);
Violation *createViolationInteractive(Vehicle *vehicleHead, Location *locationHead);
Payment *pushPayment(Payment *top, Payment *newNode);
void makePayment(Violation *violationHead, Vehicle *vehicleHead, Payment **paymentTop);
void displayPayments(Payment *top);
Payment *createInitialPayment(Payment *top, Violation *v);
Vehicle *mergeVehiclesByUnpaid(Vehicle *left, Vehicle *right);
Vehicle *mergeSortVehiclesByUnpaid(Vehicle *head);
void displayByUnpaid(Vehicle *vehicleHead, Violation *violationHead);
Vehicle *mergeVehiclesByPending(Vehicle *left, Vehicle *right);
Vehicle *mergeSortVehiclesByPending(Vehicle *head);
void displayByPendingAmount(Vehicle *vehicleHead, Violation *violationHead);
Location *mergeLocationsByViolations(Location *left, Location *right, Violation *violationHead);
Location *mergeSortLocationsByViolations(Location *head, Violation *violationHead);
void displayLocationsByViolations(Location *locationHead, Violation *violationHead);
Location *mergeLocationsByRevenue(Location *left, Location *right, Violation *violationHead);
Location *mergeSortLocationsByRevenue(Location *head, Violation *violationHead);
void displayLocationsByRevenue(Location *locationHead, Violation *violationHead);
void displayByOwnerName(Vehicle *vehicleHead, Violation *violationHead);
void displayLocationViolations(Location *locationHead, Violation *violationHead, Vehicle *vehicleHead);
void displayViolationTypesByFrequency(Violation *violationHead);
void displayRepeatOffenderFraction(Vehicle *vehicleHead, Violation *violationHead);
void displayVehiclesByThreshold(Vehicle *head, int threshold);
void displayPendingViolations(Violation *violationHead, const char regNo[]);
void loadInitialData(Vehicle **vHead, Location **lHead, Violation **violHead, Payment **paymentTop);

#endif 
