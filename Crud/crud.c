#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    int id;
    char name[50];
    int age;
};

void createFile() {
    FILE *fp = fopen("users.txt", "a");
    if (fp == NULL) {
        printf("Cannot create file\n");
        exit(1);
    }
    fclose(fp);
}

void addUser() {
    struct User u, temp;
    FILE *fp;

    printf("Enter ID: ");
    scanf("%d", &u.id);

    fp = fopen("users.txt", "r");
    while (fscanf(fp, "%d,%49[^,],%d ", &temp.id, temp.name, &temp.age) == 3) {
        if (temp.id == u.id) {
            printf("ID already exists!\n");
            fclose(fp);
            return;
        }
    }
    fclose(fp);

    printf("Enter name: ");
    scanf(" %49[^\n]", u.name);
    printf("Enter age: ");
    scanf("%d", &u.age);

    fp = fopen("users.txt", "a");
    fprintf(fp, "%d,%s,%d\n", u.id, u.name, u.age);
    fclose(fp);

    printf("User added.\n");
}

void showUsers() {
    struct User u;
    int count = 0;

    FILE *fp = fopen("users.txt", "r");
    if (fp == NULL) {
        printf("File not found\n");
        return;
    }

    printf("\nID\tName\t\tAge\n");
    while (fscanf(fp, "%d,%49[^,],%d ", &u.id, u.name, &u.age) == 3) {
        printf("%d\t%-15s\t%d\n", u.id, u.name, u.age);
        count++;
    }

    if (count == 0)
        printf("No users yet.\n");

    fclose(fp);
}

void updateUser() {
    struct User u;
    int id, found = 0;

    printf("Enter ID to update: ");
    scanf("%d", &id);

    FILE *fp = fopen("users.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d,%49[^,],%d ", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
            found = 1;
            printf("Enter new name: ");
            scanf(" %49[^\n]", u.name);
            printf("Enter new age: ");
            scanf("%d", &u.age);
        }
        fprintf(tmp, "%d,%s,%d\n", u.id, u.name, u.age);
    }

    fclose(fp);
    fclose(tmp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User updated.\n");
    else
        printf("No user with ID %d\n", id);
}

void deleteUser() {
    struct User u;
    int id, found = 0;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    FILE *fp = fopen("users.txt", "r");
    FILE *tmp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d,%49[^,],%d ", &u.id, u.name, &u.age) == 3) {
        if (u.id == id) {
        } else {
            fprintf(tmp, "%d,%s,%d\n", u.id, u.name, u.age);
        }
    }

    fclose(fp);
    fclose(tmp);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        printf("User deleted.\n");
    else
        printf("No user with ID %d\n", id);
}

int main() {
    int choice;

    createFile();

    do {
        printf("\nMENU\n");
        printf("1. Add user\n");
        printf("2. Show users\n");
        printf("3. Update user\n");
        printf("4. Delete user\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addUser(); break;
            case 2: showUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: printf("Bye!\n"); break;
            default: printf("try again.\n");
        }
    } while (choice != 5);

    return 0;
}
