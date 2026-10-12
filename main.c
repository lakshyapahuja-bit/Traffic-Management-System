#include "project.h"
#include<stdio.h>

int main() {

    Vehicle *vehicleHead = NULL;

    Violation *violationHead = NULL;

    Location *locationHead = NULL;

    Payment *paymentStackTop = NULL;

    loadInitialData(&vehicleHead, &locationHead, &violationHead,&paymentStackTop);

    int choice;
    do {

        printf("\n================ TRAFFIC SYSTEM MENU ================\n");

        printf("1.  Register Vehicle (Op 1)\n");

        printf("2.  Report Violation (Op 2)\n");

        printf("3.  Process Payment (Op 3)\n");

        printf("4.  Search Owner Details & Violations (Op 4)\n");

        printf("5.  Display Location Violations (Op 5)\n");

        printf("6.  Sort Vehicles by Unpaid Violations (Op 6)\n");

        printf("7.  Sort Locations by Violation Count (Op 7)\n");

        printf("8.  Sort Locations by Revenue (Op 8)\n");

        printf("9.  Sort Violation Types by Frequency (Op 9)\n");

        printf("10. Violation Types by Repeat Offender Fraction (Op 10)\n");

        printf("11. Vehicles Matching Violation Threshold (Op 11)\n");

        printf("12. Display Pending Violations for Vehicle (Op 12)\n");

        printf("13. Sort Vehicles by Pending Fine Amount (Op 13)\n");

        printf("14. Display Payment History - Stack (Op 14)\n");

        printf("15. Display All Registered Vehicles\n");

        printf("0.  Exit\n");

        printf("Enter Choice: ");

        scanf("%d", &choice);



        switch (choice) {

            case 1:

                vehicleHead = insertVehicle(vehicleHead, createVehicleInteractive());

                break;

            case 2:

                violationHead = insertViolation(violationHead, createViolationInteractive(vehicleHead, locationHead));

                break;

            case 3:

                makePayment(violationHead, vehicleHead, &paymentStackTop);

                break;

            case 4:

                displayByOwnerName(vehicleHead, violationHead);

                break;

            case 5:

                displayLocationViolations(locationHead, violationHead, vehicleHead);

                break;

            case 6:

                displayByUnpaid(vehicleHead, violationHead);

                break;

            case 7:

                displayLocationsByViolations(locationHead, violationHead);

                break;

            case 8:

                displayLocationsByRevenue(locationHead, violationHead);

                break;

            case 9:

                displayViolationTypesByFrequency(violationHead);

                break;

            case 10:

                displayRepeatOffenderFraction(vehicleHead, violationHead);

                break;

            case 11: {

                int thresh;

                printf("Enter Threshold: "); scanf("%d", &thresh);

                displayVehiclesByThreshold(vehicleHead, thresh);

                break;

            }

            case 12: {

                char reg[20];

                printf("Enter Reg No: "); scanf("%s", reg);

                displayPendingViolations(violationHead, reg);

                break;

            }

            case 13:

                displayByPendingAmount(vehicleHead, violationHead);

                break;

            case 14:

                displayPayments(paymentStackTop);

                break;

            case 15:

                displayVehicles(vehicleHead);

                break;

            case 0:

                printf("Exiting Program...\n");

                break;

            default:

                printf("Invalid Choice!\n");

                break;

        }

    } while (choice != 0);



    return 0;

}
