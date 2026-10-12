#include "project.h"
#include<stdio.h>

int vehicleCount = 0;
int violationCount = 0;
int locationCount = 0;

Vehicle *findVehicle(Vehicle *head, const char regNo[]) {
    Vehicle *result = NULL;
    Vehicle *temp = head;

    while (temp != NULL && result == NULL) {
        if (strcmp(temp->regNo, regNo) == 0) {
            result = temp;
        } else {
            temp = temp->next;
        }
    }

    return result;
}

Location *findLocation(Location *head, int locationID) {
    Location *result = NULL;
    Location *temp = head;

    while (temp != NULL && result == NULL) {
        if (temp->locationID == locationID) {
            result = temp;
        } else {
            temp = temp->next;
        }
    }

   
    return result;
}

Violation *findViolation(Violation *head, int violationID) {
    Violation *result = NULL;
    Violation *temp = head;

    while (temp != NULL && result == NULL) {
        if (temp->violationID == violationID) {
            result = temp;
        } else {
            temp = temp->next;
        }
    }

    return result;
}

float calculateFine(int violationType) {
    float fine;

    switch (violationType) {
        case 1: fine = 500.0; break;
        case 2: fine = 1000.0; break;
        case 3: fine = 1500.0; break;
        case 4: fine = 500.0; break;
        default: fine = 0.0; break;
    }

    return fine;
}

void displayViolationType(int type) {
    switch (type) {
        case 1: printf("Illegal Parking"); break;
        case 2: printf("Over-speeding"); break;
        case 3: printf("Red-light Jump"); break;
        case 4: printf("Helmet/Seatbelt Infraction"); break;
        default: printf("Unknown Violation"); break;
    }
}

int getUnpaidCount(Vehicle *v, Violation *violationHead) {
    int count = 0;
    Violation *vtemp = violationHead;

    while (vtemp != NULL) {
        if (strcmp(v->regNo, vtemp->vehicleRegNo) == 0 && vtemp->isPaid == 0) {
            count++;
        }
        vtemp = vtemp->next;
    }

    return count;
}

int countLocationViolations(int locationID, Violation *violationHead) {
    int count = 0;
    Violation *temp = violationHead;

    while (temp != NULL) {
        if (temp->locationID == locationID) {
            count++;
        }
        temp = temp->next;
    }

    return count;
}

float calculateLocationRevenue(int locationID, Violation *violationHead) {
    float revenue = 0.0;
    Violation *temp = violationHead;

    while (temp != NULL) {
        if (temp->locationID == locationID) {
            revenue += temp->fineAmount;
        }
        temp = temp->next;
    }

    return revenue;
}

Vehicle *copyVehicleList(Vehicle *head, Violation *violationHead) {
    Vehicle *copyHead = NULL;
    Vehicle *tail = NULL;
    Vehicle *temp = head;

    while (temp != NULL) {
        Vehicle *newNode = (Vehicle *)malloc(sizeof(Vehicle));
        if (newNode != NULL) {
            *newNode = *temp;
            newNode->violationCount = getUnpaidCount(temp, violationHead);
            newNode->next = NULL;

            if (copyHead == NULL) {
                copyHead = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        temp = temp->next;
    }

    return copyHead;
}

void freeVehicleList(Vehicle *head) {
    Vehicle *temp = head;
    while (temp != NULL) {
        Vehicle *next = temp->next;
        free(temp);
        temp = next;
    }
}

Vehicle *insertVehicle(Vehicle *head, Vehicle *newNode) {
    Vehicle *resultHead = head;

    if (newNode != NULL) {
        if (findVehicle(head, newNode->regNo) != NULL) {
            printf("Error: Vehicle with registration number %s already exists!\n", newNode->regNo);
            free(newNode);
            vehicleCount--;
        } else if (head == NULL || strcmp(newNode->regNo, head->regNo) < 0) {
            newNode->next = head;
            resultHead = newNode;
        } else {
            Vehicle *curr = head;
            while (curr->next != NULL && strcmp(curr->next->regNo, newNode->regNo) < 0) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }
    }

    return resultHead;
}

Vehicle *createVehicleInteractive() {
    Vehicle *newNode = NULL;

    if (vehicleCount >= MAX_VEHICLES) {
        printf("Vehicle limit reached!\n");
    } else {
        newNode = (Vehicle *)malloc(sizeof(Vehicle));
        if (newNode != NULL) {
            printf("Enter Registration Number: ");
             scanf("%s", newNode->regNo);
            printf("Enter Owner Name: ");
             scanf("%s", newNode->ownerName);
            printf("Enter Contact Details: ");
             scanf("%s", newNode->contact);
            printf("Enter Address: "); scanf("%s", newNode->address);

            newNode->totalFineAccrued = 0;
            newNode->totalFinePaid = 0;
            newNode->pendingAmount = 0;
            newNode->violationCount = 0;
            newNode->next = NULL;

            vehicleCount++;
        }
    }

    return newNode;
}

void displayVehicles(Vehicle *head) {
    if (head == NULL) {
        printf("No vehicles registered!\n");
    } else {
        printf("\n========== VEHICLE LIST (SORTED BY REG NO) ==========\n");
        Vehicle *temp = head;
        while (temp != NULL) {
            printf("Reg No: %s | Owner: %s | Contact: %s | Address: %s\n",
                   temp->regNo, temp->ownerName, temp->contact, temp->address);
            printf("Accrued: %f | Paid: %f | Pending: %f | Violations: %d\n",
                   temp->totalFineAccrued, temp->totalFinePaid, temp->pendingAmount, temp->violationCount);
            printf("------------------------------------------------------------\n");
            temp = temp->next;
            printf("\n \n");
        }
        
    }
}

Violation *insertViolation(Violation *head, Violation *newNode) {
    Violation *resultHead = head;

    if (newNode != NULL) {
        newNode->next = head;
        resultHead = newNode;
    }

    return resultHead;
}

Violation *createViolationInteractive(Vehicle *vehicleHead, Location *locationHead) {
    Violation *newNode = NULL;

    if (violationCount >= MAX_VIOLATIONS) {
        printf("Violation limit reached!\n");
    } else {
        char regNo[20];
        printf("Enter Vehicle Registration Number: ");
        scanf("%s", regNo);

        Vehicle *v = findVehicle(vehicleHead, regNo);
        if (v == NULL) {
            printf("Error: Vehicle not found!\n");
        } else {
            newNode = (Violation *)malloc(sizeof(Violation));
            if (newNode != NULL) {
                // printf("Enter Violation ID: "); scanf("%d", &newNode->violationID);
                newNode->violationID = violationCount + 101;
                strcpy(newNode->vehicleRegNo, v->regNo);

                printf("Select Violation Type:\n1. Illegal Parking\n2. Over-speeding\n3. Red-light Jump\n4. Helmet/Seatbelt Infraction\nChoice: ");
                scanf("%d", &newNode->violationType);

                printf("Enter Location ID: "); scanf("%d", &newNode->locationID);

                if (findLocation(locationHead, newNode->locationID) == NULL) {
                    printf("Error: Location ID does not exist!\n");
                    free(newNode);
                    newNode = NULL;
                } else {
                    printf("Enter Date (DD MM YYYY): ");
                    scanf("%d %d %d", &newNode->dateTime.day, &newNode->dateTime.month, &newNode->dateTime.year);
                    printf("Enter Time (HH MM): ");
                    scanf("%d %d", &newNode->dateTime.hour, &newNode->dateTime.minute);

                    newNode->fineAmount = calculateFine(newNode->violationType);

                    if (v->violationCount > 2) {
                        newNode->fineAmount += (newNode->fineAmount * 0.10f);
                    }

                    newNode->isPaid = 0;
                    newNode->next = NULL;

                    v->totalFineAccrued += newNode->fineAmount;
                    v->pendingAmount += newNode->fineAmount;
                    v->violationCount++;

                    violationCount++;
                }
            }
        }
    }

    return newNode;
}

Payment *pushPayment(Payment *top, Payment *newNode) {
    Payment *resultTop = top;

    if (newNode != NULL) {
        newNode->next = top;
        resultTop = newNode;
    }

    return resultTop;
}

void makePayment(Violation *violationHead, Vehicle *vehicleHead, Payment **paymentTop) {
    int vID;
    printf("Enter Violation ID to pay: ");
    scanf("%d", &vID);

    Violation *viol = findViolation(violationHead, vID);

    if (viol == NULL) {
        printf("Violation not found!\n");
    } else if (viol->isPaid == 1) {
        printf("Violation already paid!\n");
    } else {
        Vehicle *veh = findVehicle(vehicleHead, viol->vehicleRegNo);

        if (veh == NULL) {
            printf("Associated vehicle not found!\n");
        } else {
            Payment *newPayment = (Payment *)malloc(sizeof(Payment));

            if (newPayment != NULL) {
                printf("Enter Transaction ID: "); 
                scanf("%d", &newPayment->transactionID);
                newPayment->violationID = vID;
                newPayment->amountPaid = viol->fineAmount;
                printf("Enter Mode of Payment (UPI/Card/Cash): "); scanf("%s", newPayment->modeOfPayment);
                printf("Enter Payment Date (DD MM YYYY): ");
                scanf("%d %d %d", &newPayment->date.day, &newPayment->date.month, &newPayment->date.year);
                printf("Enter Payment Time (HH MM): ");
                scanf("%d %d", &newPayment->date.hour, &newPayment->date.minute);

                viol->isPaid = 1;
                veh->totalFinePaid += newPayment->amountPaid;
                veh->pendingAmount -= newPayment->amountPaid;

                *paymentTop = pushPayment(*paymentTop, newPayment);
                printf("Payment of %.2f successfully processed!\n", newPayment->amountPaid);
            }
        }
    }
}

void displayPayments(Payment *top) {
    if (top == NULL) {
        printf("No payment transactions recorded.\n");
    } else {
        printf("\n========== PAYMENT HISTORY (REVERSE CHRONOLOGICAL - STACK) ==========\n");
        Payment *temp = top;
        while (temp != NULL) {
            printf("Transaction ID : %d | Violation ID: %d\n", temp->transactionID, temp->violationID);
            printf("Amount Paid    : %.2f | Mode: %s\n", temp->amountPaid, temp->modeOfPayment);
            printf("Date & Time    : %02d/%02d/%04d %02d:%02d\n",
                   temp->date.day, temp->date.month, temp->date.year, temp->date.hour, temp->date.minute);
            printf("------------------------------------------------------------\n");
            temp = temp->next;
        }
    }
}

Payment *createInitialPayment(Payment *top, Violation *v)
{
    Payment* ans = top;
    Payment *p = malloc(sizeof(Payment));

    
    if(p != NULL)
    {
    p->transactionID = v->violationID;
    p->violationID = v->violationID;
    p->amountPaid = v->fineAmount;
    strcpy(p->modeOfPayment, "Cash");
    p->date = v->dateTime;
    p->next = NULL;

    ans =  pushPayment(top, p);
    }

    return ans;
}



Vehicle *mergeVehiclesByUnpaid(Vehicle *left, Vehicle *right) {
    Vehicle* dummy = (Vehicle*)(malloc(sizeof(Vehicle)));
    Vehicle *tail = dummy;

    dummy->next = NULL;

    while (left != NULL && right != NULL) {
        if (left->violationCount >= right->violationCount) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }

        tail = tail->next;
    }

    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

     Vehicle* ans = dummy->next;
    free(dummy);

    return ans;
}

Vehicle *mergeSortVehiclesByUnpaid(Vehicle *head) {
    Vehicle *resultHead = head;

    if (head != NULL && head->next != NULL) {
        Vehicle *slow = head;
        Vehicle *fast = head->next;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }

        Vehicle *mid = slow->next;
        slow->next = NULL;

        Vehicle *left = mergeSortVehiclesByUnpaid(head);
        Vehicle *right = mergeSortVehiclesByUnpaid(mid);

        resultHead = mergeVehiclesByUnpaid(left, right);
    }

    return resultHead;
}

void displayByUnpaid(Vehicle *vehicleHead, Violation *violationHead) {
    Vehicle *copyHead = copyVehicleList(vehicleHead, violationHead);

    if (copyHead == NULL) {
        printf("No vehicles registered!\n");
    } else {
        copyHead = mergeSortVehiclesByUnpaid(copyHead);

        printf("\n===== VEHICLES BY UNPAID VIOLATIONS (DESCENDING) =====\n");
        Vehicle *temp = copyHead;
        while (temp != NULL) {
            printf("Reg No: %s | Owner: %s | Unpaid Violations: %d | Pending Fine: %.2f\n",
                   temp->regNo, temp->ownerName, temp->violationCount, temp->pendingAmount);
            temp = temp->next;
        }

        freeVehicleList(copyHead);
    }
}

Vehicle *mergeVehiclesByPending(Vehicle *left, Vehicle *right) {
    Vehicle* dummy = (Vehicle*)(malloc(sizeof(Vehicle)));
    Vehicle *tail = dummy;

    dummy->next = NULL;

    while (left != NULL && right != NULL) {
        if (left->pendingAmount >= right->pendingAmount) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }

        tail = tail->next;
    }

    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }
    Vehicle* ans = dummy->next;
    free(dummy);

    return ans;
}
Vehicle *mergeSortVehiclesByPending(Vehicle *head) {
    Vehicle *resultHead = head;

    if (head != NULL && head->next != NULL) {
        Vehicle *slow = head;
        Vehicle *fast = head->next;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }

        Vehicle *mid = slow->next;
        slow->next = NULL;

        Vehicle *left = mergeSortVehiclesByPending(head);
        Vehicle *right = mergeSortVehiclesByPending(mid);

        resultHead = mergeVehiclesByPending(left, right);
    }
    
    return resultHead;
}

void displayByPendingAmount(Vehicle *vehicleHead, Violation *violationHead) {
    Vehicle *copyHead = copyVehicleList(vehicleHead, violationHead);

    if (copyHead == NULL) {
        printf("No vehicles registered!\n");
    } else {
        copyHead = mergeSortVehiclesByPending(copyHead);

        printf("\n===== VEHICLES BY PENDING FINE AMOUNT (DESCENDING) =====\n");
        Vehicle *temp = copyHead;
        while (temp != NULL) {
            printf("Reg No: %s | Owner: %s | Pending Amount: %.2f\n",
                   temp->regNo, temp->ownerName, temp->pendingAmount);
            temp = temp->next;
        }

        freeVehicleList(copyHead);
    }
}

Location *mergeLocationsByViolations(Location *left, Location *right, Violation *violationHead) {
    Location* dummy = (Location*)(malloc(sizeof(Location)));
    Location *tail = dummy;

    dummy->next = NULL;

    while (left != NULL && right != NULL) {
        int count1 = countLocationViolations(left->locationID, violationHead);
        int count2 = countLocationViolations(right->locationID, violationHead);

        if (count1 >= count2) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }

        tail = tail->next;
    }

    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    Location* ans = dummy->next;
    free(dummy);

    return ans;
}

Location *mergeSortLocationsByViolations(Location *head, Violation *violationHead) {
    Location *resultHead = head;

    if (head != NULL && head->next != NULL) {
        Location *slow = head;
        Location *fast = head->next;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }

        Location *mid = slow->next;
        slow->next = NULL;

        Location *left = mergeSortLocationsByViolations(head, violationHead);
        Location *right = mergeSortLocationsByViolations(mid, violationHead);

        resultHead = mergeLocationsByViolations(left, right, violationHead);
    }

    return resultHead;
}

void displayLocationsByViolations(Location *locationHead, Violation *violationHead) {
    if (locationHead == NULL) {
        printf("No locations registered!\n");
    } else {
        locationHead = mergeSortLocationsByViolations(locationHead, violationHead);

        printf("\n===== LOCATIONS BY NUMBER OF VIOLATIONS (DESCENDING) =====\n");
        Location *temp = locationHead;
        while (temp != NULL) {
            int count = countLocationViolations(temp->locationID, violationHead);
            printf("Location ID: %d | Name: %s | Total Violations: %d\n",
                   temp->locationID, temp->locationName, count);
            temp = temp->next;
        }
    }
}

Location *mergeLocationsByRevenue(Location *left, Location *right, Violation *violationHead) {
    Location* dummy = (Location*)(malloc(sizeof(Location)));
    Location *tail = dummy;

    dummy->next = NULL;

    while (left != NULL && right != NULL) {
        float rev1 = calculateLocationRevenue(left->locationID, violationHead);
        float rev2 = calculateLocationRevenue(right->locationID, violationHead);

        if (rev1 >= rev2) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }

        tail = tail->next;
    }

    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }
    Location* ans = dummy->next;
    free(dummy);

    return ans;
}

Location *mergeSortLocationsByRevenue(Location *head, Violation *violationHead) {
    Location *resultHead = head;

    if (head != NULL && head->next != NULL) {
        Location *slow = head;
        Location *fast = head->next;

        while (fast != NULL && fast->next != NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }

        Location *mid = slow->next;
        slow->next = NULL;

        Location *left = mergeSortLocationsByRevenue(head, violationHead);
        Location *right = mergeSortLocationsByRevenue(mid, violationHead);

        resultHead = mergeLocationsByRevenue(left, right, violationHead);
    }

    return resultHead;
}


void displayLocationsByRevenue(Location *locationHead, Violation *violationHead) {
    if (locationHead == NULL) {
        printf("No locations registered!\n");
    } else {
        locationHead = mergeSortLocationsByRevenue(locationHead, violationHead);

        printf("\n===== LOCATIONS BY FINE REVENUE (DESCENDING) =====\n");
        Location *temp = locationHead;
        while (temp != NULL) {
            float rev = calculateLocationRevenue(temp->locationID, violationHead);
            printf("Location ID: %d | Name: %s | Revenue: %.2f\n",
                   temp->locationID, temp->locationName, rev);
            temp = temp->next;
        }
    }
}

void displayByOwnerName(Vehicle *vehicleHead, Violation *violationHead) {
    char name[50];
    printf("Enter Owner Name: ");
    scanf("%s", name);

    Vehicle *v = vehicleHead;
    int foundVehicle = 0;

    while (v != NULL) {
        if (strcmp(v->ownerName, name) == 0) {
            foundVehicle = 1;
            printf("\nVehicle Reg No: %s | Owner: %s | Contact: %s\n", v->regNo, v->ownerName, v->contact);
            printf("------------------------------------------------------------\n");

            Violation *vtemp = violationHead;
            int foundViol = 0;

            while (vtemp != NULL) {
                if (strcmp(vtemp->vehicleRegNo, v->regNo) == 0) {
                    printf("  Violation ID: %d | Type: ", vtemp->violationID);
                    displayViolationType(vtemp->violationType);
                    printf(" | Location ID: %d\n", vtemp->locationID);
                    printf("  Date: %02d/%02d/%04d | Fine: %.2f | Status: %s\n",
                           vtemp->dateTime.day, vtemp->dateTime.month, vtemp->dateTime.year,
                           vtemp->fineAmount, vtemp->isPaid ? "Paid" : "Pending");
                    foundViol = 1;
                }
                vtemp = vtemp->next;
            }

            if (foundViol == 0) {
                printf("  No violations recorded for this vehicle.\n");
            }
        }
        v = v->next;
    }

    if (foundVehicle == 0) {
        printf("No vehicles found for owner: %s\n", name);
    }
}

void displayLocationViolations(Location *locationHead, Violation *violationHead, Vehicle *vehicleHead) {
    int locID;
    printf("Enter Location ID: ");
    scanf("%d", &locID);

    Location *loc = findLocation(locationHead, locID);

    if (loc == NULL) {
        printf("Location not found!\n");
    } else {
        printf("\nLocation: %s (ID: %d)\n", loc->locationName, loc->locationID);
        printf("========== VIOLATIONS AT THIS LOCATION ==========\n");

        Violation *v = violationHead;
        int found = 0;

        while (v != NULL) {
            if (v->locationID == locID) {
                Vehicle *veh = findVehicle(vehicleHead, v->vehicleRegNo);
                printf("Violation ID: %d | Vehicle: %s", v->violationID, v->vehicleRegNo);
                if (veh != NULL) {
                    printf(" | Owner: %s | Contact: %s", veh->ownerName, veh->contact);
                }
                printf("\nType: "); displayViolationType(v->violationType);
                printf(" | Fine: %.2f | Status: %s\n", v->fineAmount, v->isPaid ? "Paid" : "Pending");
                printf("--------------------------------------------------\n");
                found = 1;
            }
            v = v->next;
        }

        if (found == 0) {
            printf("No violations recorded at this location.\n");
        }
    }
}


void displayViolationTypesByFrequency(Violation *violationHead) {
    int type[4] = {1, 2, 3, 4};
    int count[4] = {0, 0, 0, 0};

    Violation *v = violationHead;
    while (v != NULL) {
        if (v->violationType >= 1 && v->violationType <= 4) {
            count[v->violationType - 1]++;
        }
        v = v->next;
    }

    
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (count[i] < count[j]) {
                int tempCount = count[i];
                count[i] = count[j];
                count[j] = tempCount;

                int tempType = type[i];
                type[i] = type[j];
                type[j] = tempType;
            }
        }
    }

    printf("\n===== VIOLATION TYPES BY FREQUENCY (DESCENDING) =====\n");
    for (int i = 0; i < 4; i++) {
        printf("Type: "); displayViolationType(type[i]);
        printf(" | Frequency: %d\n", count[i]);
    }
}

void displayRepeatOffenderFraction(Vehicle *vehicleHead, Violation *violationHead) {
    int type[4] = {1, 2, 3, 4};
    int totalCount[4] = {0, 0, 0, 0};
    int repeatCount[4] = {0, 0, 0, 0};
    float fraction[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    Violation *v = violationHead;
    while (v != NULL) {
        if (v->violationType >= 1 && v->violationType <= 4) {
            int idx = v->violationType - 1;
            totalCount[idx]++;

            Vehicle *veh = findVehicle(vehicleHead, v->vehicleRegNo);
            if (veh != NULL && veh->violationCount > 2) {
                repeatCount[idx]++;
            }
        }
        v = v->next;
    }

    for (int i = 0; i < 4; i++) {
        if (totalCount[i] > 0) {
            fraction[i] = (float)repeatCount[i] / totalCount[i];
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (fraction[i] < fraction[j]) {
                float tempFrac = fraction[i];
                fraction[i] = fraction[j];
                fraction[j] = tempFrac;

                int tempType = type[i];
                type[i] = type[j];
                type[j] = tempType;

                int tempTot = totalCount[i];
                totalCount[i] = totalCount[j];
                totalCount[j] = tempTot;

                int tempRep = repeatCount[i];
                repeatCount[i] = repeatCount[j];
                repeatCount[j] = tempRep;
            }
        }
    }

    printf("\n===== VIOLATION TYPES BY REPEAT OFFENDER FRACTION =====\n");
    for (int i = 0; i < 4; i++) {
        printf("Type: "); displayViolationType(type[i]);
        printf(" | Total: %d | Repeat: %d | Fraction: %f\n",
               totalCount[i], repeatCount[i], fraction[i]);
    }
}

void displayVehiclesByThreshold(Vehicle *head, int threshold) {
    Vehicle *temp = head;
    printf("\n===== VEHICLES WITH VIOLATIONS >= %d =====\n", threshold);
    int found = 0;

    while (temp != NULL) {
        if (temp->violationCount >= threshold) {
            printf("Reg No: %s | Owner: %s | Violations: %d | Contact: %s\n",
                   temp->regNo, temp->ownerName, temp->violationCount, temp->contact);
            found = 1;
        }
        temp = temp->next;
    }

    if (found == 0) {
        printf("No vehicles match the threshold criterion.\n");
    }
}

void displayPendingViolations(Violation *violationHead, const char regNo[]) {
    Violation *temp = violationHead;
    int found = 0;

    printf("\n===== PENDING VIOLATIONS FOR %s =====\n", regNo);
    while (temp != NULL) {
        if (strcmp(temp->vehicleRegNo, regNo) == 0 && temp->isPaid == 0) {
            printf("Violation ID: %d | Fine: %.2f | Type: ", temp->violationID, temp->fineAmount);
            displayViolationType(temp->violationType);
            printf(" | Date: %02d/%02d/%04d\n", temp->dateTime.day, temp->dateTime.month, temp->dateTime.year);
            found = 1;
        }
        temp = temp->next;
    }

    if (found == 0) {
        printf("No pending violations found.\n");
    }
}

void loadInitialData(Vehicle **vHead, Location **lHead, Violation **violHead,Payment **paymentTop)
{
    FILE *fVeh = fopen("vehicles.txt", "r");

    if (fVeh != NULL) {
        Vehicle *v;

        while ((v = (Vehicle *)malloc(sizeof(Vehicle))) != NULL) {
            if (fscanf(fVeh, "%s %s %s %s",
                       v->regNo, v->ownerName,
                       v->contact, v->address) != 4) {
                free(v);
                break;
            }

            v->totalFineAccrued = 0;
            v->totalFinePaid = 0;
            v->pendingAmount = 0;
            v->violationCount = 0;
            v->next = NULL;

            vehicleCount++;
            *vHead = insertVehicle(*vHead, v);
        }

        fclose(fVeh);
        printf("Vehicles successfully loaded from file.\n");
    }

    FILE *fLoc = fopen("locations.txt", "r");

    if (fLoc != NULL) {
        Location *loc;

        while ((loc = (Location *)malloc(sizeof(Location))) != NULL) {
            if (fscanf(fLoc, "%d %s",
                       &loc->locationID, loc->locationName) != 2) {
                free(loc);
                break;
            }

            loc->next = *lHead;
            *lHead = loc;
            locationCount++;
        }

        fclose(fLoc);
        printf("Locations successfully loaded from file.\n");
    }

    FILE *fViol = fopen("violations.txt", "r");

    if (fViol != NULL) {
        Violation *v;

        while ((v = (Violation *)malloc(sizeof(Violation))) != NULL) {
            if (fscanf(fViol, "%d %s %d %d %d %d %d %d %d %d",
                       &v->violationID, v->vehicleRegNo,
                       &v->violationType, &v->locationID,
                       &v->dateTime.day, &v->dateTime.month,
                       &v->dateTime.year, &v->dateTime.hour,
                       &v->dateTime.minute, &v->isPaid) != 10) {
                free(v);
                break;
            }

            Vehicle *veh = findVehicle(*vHead, v->vehicleRegNo);

            if (veh != NULL) {
                v->fineAmount = calculateFine(v->violationType);

                if (veh->violationCount > 2) {
                    v->fineAmount += v->fineAmount * 0.10f;
                }

                veh->totalFineAccrued += v->fineAmount;

                if (v->isPaid == 1) {
                    veh->totalFinePaid += v->fineAmount;
                    *paymentTop = createInitialPayment(*paymentTop, v);
                } else {
                    veh->pendingAmount += v->fineAmount;
                }

                veh->violationCount++;

                v->next = *violHead;
                *violHead = v;
                violationCount++;
            } else {
                free(v);
            }
        }

        fclose(fViol);
        printf("Violations successfully loaded from file.\n");
    }
}