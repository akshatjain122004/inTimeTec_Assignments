#include <stdio.h>
#include <stdbool.h>
#define MAX_EMPLOYEES 100
#define MAX_NAME_LENGTH 100
#define MAX_DESIGNATION_LENGTH 100

struct employee
{
    int employeeId;
    char name[MAX_NAME_LENGTH];
    char designation[MAX_DESIGNATION_LENGTH];
    int reports[MAX_EMPLOYEES];
    int reportingManagerId;
    int reportsCount;
};

bool checkIdAvailable(int id, int n, struct employee employee[])
{
    for (int j = 0 ; j < n ; j++)
    {
        if (id == employee[j].employeeId)
        {
            return false;
        }
    }
    return true;
}

bool employeeIdPresent(int id, int n, struct employee employee[])
{
    for (int j = 0 ; j < n ; j++)
    {
        if (id == employee[j].employeeId)
        {
            return true;
        }
    }
    return false;
}

void updateManagerReport(int employeeId, int managerId, struct employee employee[], int n)
{
    if (managerId == -1)
    {
        return;
    }
    for (int j = 0; j < n; j++)
    {
        if(managerId == employee[j].employeeId)
        {
            employee[j].reports[employee[j].reportsCount] = employeeId;
            employee[j].reportsCount++;
            return;
        }
    }
}

void addEmployee(struct employee employee[],int *noOfEmployees)
{
    static bool ceoExists = false;

    if (*noOfEmployees >= 100)
    {
        printf("\nEmployee limit reached. Cannot add more employees.\n");
        return;
    }

    int n = *noOfEmployees;
    printf("\nEnter the Unique Employee ID: ");
    scanf("%d", &employee[n].employeeId);

    while (!(checkIdAvailable((employee[n].employeeId),n,employee)))
    {
        printf("Employee with this ID already exists. Please enter the Unique Employee ID: ");
        scanf("%d", &employee[n].employeeId);
    }

    printf("Enter the name of Employee: ");
    scanf("%99s", employee[n].name);
    printf("Enter the designation of Employee: ");
    scanf("%99s", employee[n].designation);

    employee[n].reportsCount = 0;

    printf("Enter the Reporting Manager of the Employee (-1 if no manager): ");
    scanf("%d", &employee[n].reportingManagerId);

    while ((employee[n].reportingManagerId == -1 && ceoExists) || employee[n].reportingManagerId != -1 && !(employeeIdPresent((employee[n].reportingManagerId),n,employee)))
    {
        printf("Employee with this ID does not exists. Please enter the existing Manager ID: ");
        scanf("%d", &employee[n].reportingManagerId);
    }

    if (employee[n].reportingManagerId == -1)
    {
        ceoExists = true;
    }

    updateManagerReport(employee[n].employeeId,employee[n].reportingManagerId,employee,n);

    (*noOfEmployees)++;

    printf("\nEmployee added successfully.\n\n");
}

int getEmployeeIndex(int employeeId, int numberOfEmployees, struct employee employee[])
{
    for (int index = 0; index < numberOfEmployees; index++)
    {
        if (employee[index].employeeId == employeeId)
        {
            return index;
        }
    }
    return -1;
}

void transferReports(struct employee employee[], int numberOfEmployees, int employeeIndex, int targetIndex)
{
    for (int position = 0; position < employee[employeeIndex].reportsCount; position++)
    {
        int reportId = employee[employeeIndex].reports[position];
        int reportIndex = getEmployeeIndex(reportId, numberOfEmployees, employee);

        employee[reportIndex].reportingManagerId = employee[targetIndex].employeeId;
        employee[targetIndex].reports[employee[targetIndex].reportsCount] = reportId;
        employee[targetIndex].reportsCount++;
    }
}

int findSiblingWithFewestReports(struct employee employee[], int numberOfEmployees, int managerIndex, int employeeId)
{
    int siblingIndex = -1;
    int fewestReports = MAX_EMPLOYEES + 1;

    for (int position = 0; position < employee[managerIndex].reportsCount; position++)
    {
        int siblingId = employee[managerIndex].reports[position];

        if (siblingId != employeeId)
        {
            int currentIndex = getEmployeeIndex(siblingId, numberOfEmployees, employee);

            if (employee[currentIndex].reportsCount < fewestReports)
            {
                fewestReports = employee[currentIndex].reportsCount;
                siblingIndex = currentIndex;
            }
        }
    }
    return siblingIndex;
}

void detachFromManager(struct employee employee[], int managerIndex, int employeeId)
{
    for (int position = 0; position < employee[managerIndex].reportsCount; position++)
    {
        if (employee[managerIndex].reports[position] == employeeId)
        {
            for (int next = position; next < employee[managerIndex].reportsCount - 1; next++)
            {
                employee[managerIndex].reports[next] = employee[managerIndex].reports[next + 1];
            }
            employee[managerIndex].reportsCount--;
            return;
        }
    }
}

int countSubordinates(struct employee employee[], int numberOfEmployees, int employeeIndex)
{
    int total = 0;

    for (int position = 0; position < employee[employeeIndex].reportsCount; position++)
    {
        int reportId = employee[employeeIndex].reports[position];
        int reportIndex = getEmployeeIndex(reportId, numberOfEmployees, employee);

        total = total + 1 + countSubordinates(employee, numberOfEmployees, reportIndex);
    }
    return total;
}

void displayHierarchyRecursive(struct employee employee[], int numberOfEmployees, int employeeIndex, int level)
{
    for (int space = 0; space < level; space++)
    {
        printf("    ");
    }
    printf("%d - %s - %s\n", employee[employeeIndex].employeeId, employee[employeeIndex].name, employee[employeeIndex].designation);

    for (int position = 0; position < employee[employeeIndex].reportsCount; position++)
    {
        int reportId = employee[employeeIndex].reports[position];
        int reportIndex = getEmployeeIndex(reportId, numberOfEmployees, employee);

        displayHierarchyRecursive(employee, numberOfEmployees, reportIndex, level + 1);
    }
}

void removeEmployee(struct employee employee[], int *noOfEmployees)
{
    int employeeId;
    int numberOfEmployees = *noOfEmployees;

    printf("\nEnter the Employee ID to remove: ");
    scanf("%d", &employeeId);

    int employeeIndex = getEmployeeIndex(employeeId, numberOfEmployees, employee);

    if (employeeIndex == -1)
    {
        printf("\nEmployee with this ID does not exist.\n\n");
        return;
    }

    if (employee[employeeIndex].reportingManagerId == -1)
    {
        printf("\nThe CEO cannot be removed.\n\n");
        return;
    }

    int managerIndex = getEmployeeIndex(employee[employeeIndex].reportingManagerId, numberOfEmployees, employee);
    int targetIndex = findSiblingWithFewestReports(employee, numberOfEmployees, managerIndex, employeeId);

    if (targetIndex == -1)
    {
        targetIndex = managerIndex;
    }

    transferReports(employee, numberOfEmployees, employeeIndex, targetIndex);
    detachFromManager(employee, managerIndex, employeeId);

    for (int index = employeeIndex; index < numberOfEmployees - 1; index++)
    {
        employee[index] = employee[index + 1];
    }
    (*noOfEmployees)--;

    printf("\nEmployee removed successfully.\n\n");
}

void showEmployeeTree(struct employee employee[], int numberOfEmployees)
{
    int employeeId;

    if (numberOfEmployees == 0)
    {
        printf("\nNo employees available.\n\n");
        return;
    }

    printf("\nEnter the Employee ID to display their hierarchy: ");
    scanf("%d", &employeeId);

    int employeeIndex = getEmployeeIndex(employeeId, numberOfEmployees, employee);

    if (employeeIndex == -1)
    {
        printf("\nEmployee not found.\n\n");
        return;
    }

    printf("\n");
    displayHierarchyRecursive(employee, numberOfEmployees, employeeIndex, 0);
    printf("\nTotal Employees Under %d: %d\n\n", employeeId, countSubordinates(employee, numberOfEmployees, employeeIndex));
}

void showOrganizationTree(struct employee employee[], int numberOfEmployees)
{
    if (numberOfEmployees == 0)
    {
        printf("\nNo employees available.\n\n");
        return;
    }

    for (int index = 0; index < numberOfEmployees; index++)
    {
        if (employee[index].reportingManagerId == -1)
        {
            printf("\n===== ORGANIZATION HIERARCHY =====\n");
            displayHierarchyRecursive(employee, numberOfEmployees, index, 0);
            printf("\nTotal Employees: %d\n\n", 1 + countSubordinates(employee, numberOfEmployees, index));
            return;
        }
    }
}

int main()
{
    struct employee employee[MAX_EMPLOYEES];
    int numberOfEmployees = 0;
    int choice = 0;

    while (choice != 5)
    {
        printf("===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addEmployee(employee, &numberOfEmployees);
                break;

            case 2:
                removeEmployee(employee, &numberOfEmployees);
                break;

            case 3:
                showEmployeeTree(employee, numberOfEmployees);
                break;

            case 4:
                showOrganizationTree(employee, numberOfEmployees);
                break;

            case 5:
                printf("\nExiting EMPLOYEE HIERARCHY MANAGEMENT SYSTEM\n");
                break;

            default:
                printf("Invalid choice\n\n");
        }
    }
    return 0;
}