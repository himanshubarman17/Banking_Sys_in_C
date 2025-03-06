#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PASS_LEN 15
#define MAX_ACC_LEN 10

struct Account {
    char accNumber[MAX_ACC_LEN + 1];
    char password[MAX_PASS_LEN + 1];
    float balance;
};

int isLoggedIn = 0; // Initialize isLoggedIn to 0 (false)
struct Account currentUser;

// Function to generate a random account number
void generateAccNumber(char accNumber[]) {
    srand(time(0));
    for (int i = 0; i < MAX_ACC_LEN; i++) {
        accNumber[i] = '0' + (rand() % 10);
    }
    accNumber[MAX_ACC_LEN] = '\0'; // Null-terminate the string
}

// Function to create a new account
void createAccount() {
    struct Account acc;
    FILE *fp;

    generateAccNumber(acc.accNumber);

    printf("Enter a password (max length = %d): ", MAX_PASS_LEN);
    scanf("%s", acc.password); // No & needed for strings

    acc.balance = 0.0;
    fp = fopen("accounts.txt", "a");
    if (fp == NULL) {
        printf("Error opening the file!\n");
        return;
    }
    fprintf(fp, "%s %s %.2f\n", acc.accNumber, acc.password, acc.balance);
    fclose(fp);

    printf("\nAccount created successfully!\n");
    printf("Your account number is: {%s}\n", acc.accNumber);
}

// Function to log in to an account
int loginAccount() {
    FILE *fp = fopen("accounts.txt", "r");
    char acc_num[MAX_ACC_LEN + 1], acc_pass[MAX_PASS_LEN + 1];
    if (fp == NULL) {
        printf("Error opening the file!\n");
        return 0;
    }
    printf("Enter your account number: ");
    scanf("%s", acc_num);
    printf("Enter your password: ");
    scanf("%s", acc_pass);

    while (fscanf(fp, "%s %s %f", currentUser.accNumber, currentUser.password, &currentUser.balance) != EOF) {
        if (strcmp(currentUser.accNumber, acc_num) == 0 && strcmp(currentUser.password, acc_pass) == 0) {
            isLoggedIn = 1;
            printf("\nLogin successful!\n");
            fclose(fp);
            return 1;
        }
    }
    printf("\nInvalid account number or password!\n");
    fclose(fp);
    return 0;
}

// Function to check the account balance
void checkBalance() {
    if (!isLoggedIn) {
        printf("Please log in first!\n");
        return;
    }
    printf("Your current balance is $%.2f\n", currentUser.balance); // Corrected format specifier
}

// Function to update the account details in the file
void updateAccount() {
    struct Account acc;
    FILE *fp = fopen("accounts.txt", "r");
    FILE *tempfp = fopen("temp.txt", "w");

    if (fp == NULL || tempfp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(fp, "%s %s %f", acc.accNumber, acc.password, &acc.balance) != EOF) {
        if (strcmp(acc.accNumber, currentUser.accNumber) == 0) {
            fprintf(tempfp, "%s %s %.2f\n", currentUser.accNumber, currentUser.password, currentUser.balance);
        } else {
            fprintf(tempfp, "%s %s %.2f\n", acc.accNumber, acc.password, acc.balance);
        }
    }
    fclose(fp);
    fclose(tempfp);

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

// Function to credit an amount to the account
void creditAmount() {
    if (!isLoggedIn) {
        printf("Please log in first!\n");
        return;
    }
    float amount;
    printf("Enter the amount you want to credit: ");
    scanf("%f", &amount); // Corrected format specifier

    if (amount <= 0) {
        printf("Please enter a valid amount!\n");
        return;
    }
    currentUser.balance += amount;
    printf("Your account #%s has been credited by $%.2f\n", currentUser.accNumber, amount);
    updateAccount();
    checkBalance();
}

// Function to debit an amount from the account
void debitAmount() {
    if (!isLoggedIn) {
        printf("Please log in first!\n");
        return;
    }
    float amount;
    printf("Enter the amount you want to debit: ");
    scanf("%f", &amount); // Corrected format specifier

    if (amount <= 0) {
        printf("Please enter a valid amount!\n");
        return;
    } else if (amount > currentUser.balance) {
        printf("Insufficient balance!\n");
        return;
    }
    currentUser.balance -= amount;
    printf("Your account #%s has been debited by $%.2f\n", currentUser.accNumber, amount);
    updateAccount();
    checkBalance();
}

// Function to log out
void logout() {
    if (!isLoggedIn) {
        printf("You are not logged in!\n");
        return;
    }
    memset(&currentUser, 0, sizeof(struct Account));
    isLoggedIn = 0;
    printf("\nLogout successful!\n");
}

int main() {
    int choice;

    while (1) {
        printf("\n---------Welcome to the Bank------------\n");
        printf("Choose what you want to do:\n");
        printf("1) Create Account\n2) Login\n3) Check Balance\n4) Credit Amount\n5) Debit Amount\n6) Log Out\n7) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                loginAccount();
                break;
            case 3:
                checkBalance();
                break;
            case 4:
                creditAmount();
                break;
            case 5:
                debitAmount();
                break;
            case 6:
                logout();
                break;
            case 7:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}