#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    char phone[15];
    char password[20];
    int credit;
} Customer;
// Add customer details to file
void addCustomer() {
    FILE *fp;
    Customer c;
    fp = fopen("customers.txt", "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    printf("Enter name: ");
    scanf("%s", c.name);
    printf("Enter phone number: ");
    scanf("%s", c.phone);
    printf("Enter password: ");
    scanf("%s", c.password);
    c.credit = 0;
    fwrite(&c, sizeof(Customer), 1, fp);
    fclose(fp);
    printf("Thank you! Welcome to the family. Your phone number has been registered.\n");
}

// Login customer using phone and password
void login() {
    FILE *fp;
    Customer c;
    char phone[15], password[20];
    int found = 0;
    
    fp = fopen("customers.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    
    printf("Enter phone number: ");
    scanf("%s", phone);
    printf("Enter password: ");
    scanf("%s", password);
    
    rewind(fp); // Reset file pointer to the beginning of the file
    
    while (fread(&c, sizeof(Customer), 1, fp)) {
        if (strcmp(phone, c.phone) == 0 && strcmp(password, c.password) == 0) {
            printf("Hello %s\n", c.name);
            printf("Thank you for logging in, %s.\n", c.name);
            printf("Credit points left: %d\n", c.credit);
            printf("Go to 4 to add credit points or 5 if you want to redeem credit points.\n");
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Invalid login credentials.\n");
    }
    
    fclose(fp);
}

// Modify customer data in file
void modifyCustomer() {
    FILE *fp, *fpTemp;
    Customer c;
    char phone[15];
    int found = 0;
    fp = fopen("customers.txt", "r");
    fpTemp = fopen("temp.txt", "w");
    if (fp == NULL || fpTemp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    printf("Enter phone number of customer to modify: ");
    scanf("%s", phone);
    while (fread(&c, sizeof(Customer), 1, fp)) {
        if (strcmp(phone, c.phone) == 0) {
            printf("Enter new name: ");
            scanf("%s", c.name);
            printf("Enter new password: ");
            scanf("%s", c.password);
            found = 1;
            printf("Successfully updated! New name is %s and new number is %s.\n", c.name, phone);
        }
        fwrite(&c, sizeof(Customer), 1, fpTemp);
    }
    if (!found) {
        printf("No customer found with the given phone number.\n");
    }
    fclose(fp);
    fclose(fpTemp);
    remove("customers.txt");
    rename("temp.txt", "customers.txt");
}


// Add credit points to customer
void addCreditPoints() {
    FILE *fp, *fpTemp;
    Customer c;
    char phone[15];
    int points, found = 0;
    fp = fopen("customers.txt", "r");
    fpTemp = fopen("temp.txt", "w");
    if (fp == NULL || fpTemp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    printf("Enter phone number of customer to add credit points: ");
    scanf("%s", phone);
    printf("Enter credit points to add: ");
    scanf("%d", &points);
    while (fread(&c, sizeof(Customer), 1, fp)) {
        if (strcmp(phone, c.phone) == 0) {
            c.credit += points;
            found = 1;
        }
        fwrite(&c, sizeof(Customer), 1, fpTemp);
    }
    if (found) {
        printf("The amount of %d has been added successfully.\n", points); // Display message if customer is found
    } else {
        printf("No customer found with the given phone number.\n");
    }
    fclose(fp);
    fclose(fpTemp);
    remove("customers.txt");
    rename("temp.txt", "customers.txt");
}

// Redeem credit points from customer
void redeemCreditPoints() {
    FILE *fp, *fpTemp;
    Customer c;
    char phone[15];
    int points, found = 0;
    int redemptionOccurred = 0; // Flag to check if redemption occurred
    fp = fopen("customers.txt", "r");
    fpTemp = fopen("temp.txt", "w");
    if (fp == NULL || fpTemp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    printf("Enter phone number of customer to redeem credit points: ");
    scanf("%s", phone);
    printf("Enter credit points to redeem: ");
    scanf("%d", &points);
    while (fread(&c, sizeof(Customer), 1, fp)) {
        if (strcmp(phone, c.phone) == 0) {
            if (c.credit >= points) {
                c.credit -= points;
                found = 1;
                redemptionOccurred = 1; // Set flag indicating redemption occurred
                printf("The amount of %d has been redeemed from your account.\n", points);
            } else {
                printf("Not enough credit points.\n");
                found = -1;
            }
        }
        fwrite(&c, sizeof(Customer), 1, fpTemp);
    }
    if (found == 0) {
        printf("No customer found with the given phone number.\n");
    }
    fclose(fp);
    fclose(fpTemp);

    // Reopen fpTemp to reset file pointer position
    fpTemp = fopen("temp.txt", "r");
    fp = fopen("customers.txt", "w");
    if (fp == NULL || fpTemp == NULL) {
        printf("Error reopening file.\n");
        return;
    }
    // Copy contents of temp file to customers file
    while (fread(&c, sizeof(Customer), 1, fpTemp)) {
        fwrite(&c, sizeof(Customer), 1, fp);
    }
    fclose(fp);
    fclose(fpTemp);

    remove("temp.txt");
    if (!redemptionOccurred) { // If redemption didn't occur, print the message
        printf("No credit points have been redeemed.\n");
    }
}

// Main menu loop
int main() {
    int choice;
    printf("Welcome to Mero Billing System! Please choose the number below to continue. Thank you\n");
    while (1) {
        printf("1. Add new customer\n");
        printf("2. Login\n");
        printf("3. Modify customer data\n");
        printf("4. Add credit points\n");
        printf("5. Redeem credit points\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                login();
                break;
            case 3:
                modifyCustomer();
                break;
            case 4:
                addCreditPoints();
                break;
            case 5:
                redeemCreditPoints();
                break;
            case 6:
                printf("Have a good day.\n");
                exit(0);
            default:
                printf("Invalid choice.\n");

        }
    }
    return 0;
}