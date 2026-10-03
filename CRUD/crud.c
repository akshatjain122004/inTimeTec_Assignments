#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User
{
    int id;
    char name[50];
    int age;
};

void createFile()
{
    FILE *filePointer = fopen("users.txt", "a");

    if (filePointer == NULL)
    {
        printf("Cannot create file\n");
        exit(1);
    }
    fclose(filePointer);
}

void addUser()
{
    struct User user, existingUser;
    FILE *filePointer;

    printf("Enter ID: ");
    scanf("%d", &user.id);

    filePointer = fopen("users.txt", "r");
    while (fscanf(filePointer, "%d,%49[^,],%d ", &existingUser.id, existingUser.name, &existingUser.age) == 3)
    {
        if (existingUser.id == user.id)
        {
            printf("ID already exists!\n");
            fclose(filePointer);
            return;
        }
    }
    fclose(filePointer);

    printf("Enter name: ");
    scanf(" %49[^\n]", user.name);
    printf("Enter age: ");
    scanf("%d", &user.age);

    filePointer = fopen("users.txt", "a");
    fprintf(filePointer, "%d,%s,%d\n", user.id, user.name, user.age);
    fclose(filePointer);

    printf("User added.\n");
}

void showUsers()
{
    struct User user;
    int userCount = 0;

    FILE *filePointer = fopen("users.txt", "r");

    if (filePointer == NULL)
    {
        printf("File not found\n");
        return;
    }

    printf("\nID\tName\t\tAge\n");
    while (fscanf(filePointer, "%d,%49[^,],%d ", &user.id, user.name, &user.age) == 3)
    {
        printf("%d\t%-15s\t%d\n", user.id, user.name, user.age);
        userCount++;
    }

    if (userCount == 0)
    {
        printf("No users yet.\n");
    }

    fclose(filePointer);
}

void updateUser()
{
    struct User user;
    int userId;
    int isFound = 0;

    printf("Enter ID to update: ");
    scanf("%d", &userId);

    FILE *filePointer = fopen("users.txt", "r");
    FILE *tempFilePointer = fopen("temp.txt", "w");

    while (fscanf(filePointer, "%d,%49[^,],%d ", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == userId)
        {
            isFound = 1;
            printf("Enter new name: ");
            scanf(" %49[^\n]", user.name);
            printf("Enter new age: ");
            scanf("%d", &user.age);
        }
        fprintf(tempFilePointer, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(filePointer);
    fclose(tempFilePointer);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (isFound)
    {
        printf("User updated.\n");
    }
    else
    {
        printf("No user with ID %d\n", userId);
    }
}

void deleteUser()
{
    struct User user;
    int userId;
    int isFound = 0;

    printf("Enter ID to delete: ");
    scanf("%d", &userId);

    FILE *filePointer = fopen("users.txt", "r");
    FILE *tempFilePointer = fopen("temp.txt", "w");

    while (fscanf(filePointer, "%d,%49[^,],%d ", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == userId)
        {
            isFound = 1;
        }
        else
        {
            fprintf(tempFilePointer, "%d,%s,%d\n", user.id, user.name, user.age);
        }
    }

    fclose(filePointer);
    fclose(tempFilePointer);

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (isFound)
    {
        printf("User deleted.\n");
    }
    else
    {
        printf("No user with ID %d\n", userId);
    }
}

int main()
{
    int menuChoice;

    createFile();

    do
    {
        printf("\nMENU\n");
        printf("1. Add user\n");
        printf("2. Show users\n");
        printf("3. Update user\n");
        printf("4. Delete user\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &menuChoice);

        switch (menuChoice)
        {
            case 1:
                addUser();
                break;
            case 2:
                showUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                printf("Bye!\n");
                break;
            default:
                printf("Wrong choice, try again.\n");
        }
    } while (menuChoice != 5);

    return 0;
}