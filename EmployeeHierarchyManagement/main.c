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

// Returns the index of the employee with the given ID, or -1 if there is no such employee.
// Used for both checks: "is this ID already taken?" (result != -1) and "does this employee exist?" (result != -1).
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

void updateManagerReport(int employeeId, int managerId, struct employee employee[], int numberOfEmployees)
{
    if (managerId == -1)
    {
        return;
    }

    int managerIndex = getEmployeeIndex(managerId, numberOfEmployees, employee);

    employee[managerIndex].reports[employee[managerIndex].reportsCount] = employeeId;
    employee[managerIndex].reportsCount++;
}

void addEmployee(struct employee employee[], int *noOfEmployees)
{
    static bool ceoExists = false;

    if (*noOfEmployees >= 100)
    {
        printf("\nEmployee limit reached. Cannot add more employees.\n");
        return;
    }

    int numberOfEmployees = *noOfEmployees;
    printf("\nEnter the Unique Employee ID: ");
    scanf("%d", &employee[numberOfEmployees].employeeId);

    while (getEmployeeIndex(employee[numberOfEmployees].employeeId, numberOfEmployees, employee) != -1)
    {
        printf("Employee with this ID already exists. Please enter the Unique Employee ID: ");
        scanf("%d", &employee[numberOfEmployees].employeeId);
    }

    printf("Enter the name of Employee: ");
    scanf("%99s", employee[numberOfEmployees].name);
    printf("Enter the designation of Employee: ");
    scanf("%99s", employee[numberOfEmployees].designation);

    employee[numberOfEmployees].reportsCount = 0;

    printf("Enter the Reporting Manager of the Employee (-1 if no manager): ");
    scanf("%d", &employee[numberOfEmployees].reportingManagerId);

    while ((employee[numberOfEmployees].reportingManagerId == -1 && ceoExists) || employee[numberOfEmployees].reportingManagerId != -1 && getEmployeeIndex(employee[numberOfEmployees].reportingManagerId, numberOfEmployees, employee) == -1)
    {
        printf("Employee with this ID does not exists. Please enter the existing Manager ID: ");
        scanf("%d", &employee[numberOfEmployees].reportingManagerId);
    }

    if (employee[numberOfEmployees].reportingManagerId == -1)
    {
        ceoExists = true;
    }

    updateManagerReport(employee[numberOfEmployees].employeeId, employee[numberOfEmployees].reportingManagerId, employee, numberOfEmployees);

    (*noOfEmployees)++;

    printf("\nEmployee added successfully.\n\n");
}

// When an employee is removed, their direct reports must not be left without a manager.
// This moves every direct report of the removed employee under the new manager
// and updates each report's reportingManagerId.
void moveReportsToNewManager(struct employee employee[], int numberOfEmployees, int employeeIndex, int newManagerIndex)
{
    for (int position = 0; position < employee[employeeIndex].reportsCount; position++)
    {
        int reportId = employee[employeeIndex].reports[position];
        int reportIndex = getEmployeeIndex(reportId, numberOfEmployees, employee);

        employee[reportIndex].reportingManagerId = employee[newManagerIndex].employeeId;
        employee[newManagerIndex].reports[employee[newManagerIndex].reportsCount] = reportId;
        employee[newManagerIndex].reportsCount++;
    }
}

// Finds who takes over the reports of the employee being removed.
// Rule: the sibling (same manager) with the fewest direct reports.
// If the employee has no siblings, the employee's own manager takes over.
// Returns the index of that employee.
int findAlternativeManager(struct employee employee[], int numberOfEmployees, int managerIndex, int employeeId)
{
    int alternativeIndex = managerIndex;
    int fewestReports = MAX_EMPLOYEES + 1;

    for (int position = 0; position < employee[managerIndex].reportsCount; position++)
    {
        int siblingId = employee[managerIndex].reports[position];

        if (siblingId != employeeId)
        {
            int siblingIndex = getEmployeeIndex(siblingId, numberOfEmployees, employee);

            if (employee[siblingIndex].reportsCount < fewestReports)
            {
                fewestReports = employee[siblingIndex].reportsCount;
                alternativeIndex = siblingIndex;
            }
        }
    }
    return alternativeIndex;
}

// Deletes the removed employee's ID from their manager's reports list.
// The entries after it shift one place left, so the list has no gap.
void removeReportFromManager(struct employee employee[], int managerIndex, int employeeId)
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

// Returns the total number of employees under the given employee, at every level (not including the employee).
int getTotalReportsCount(struct employee employee[], int numberOfEmployees, int employeeIndex)
{
    int total = 0;

    for (int position = 0; position < employee[employeeIndex].reportsCount; position++)
    {
        int reportId = employee[employeeIndex].reports[position];
        int reportIndex = getEmployeeIndex(reportId, numberOfEmployees, employee);

        total = total + 1 + getTotalReportsCount(employee, numberOfEmployees, reportIndex);
    }
    return total;
}

// Prints the employee, then each of their reports below them, indented one level deeper.
void printHierarchy(struct employee employee[], int numberOfEmployees, int employeeIndex, int level)
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

        printHierarchy(employee, numberOfEmployees, reportIndex, level + 1);
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
    int newManagerIndex = findAlternativeManager(employee, numberOfEmployees, managerIndex, employeeId);

    moveReportsToNewManager(employee, numberOfEmployees, employeeIndex, newManagerIndex);
    removeReportFromManager(employee, managerIndex, employeeId);

    for (int index = employeeIndex; index < numberOfEmployees - 1; index++)
    {
        employee[index] = employee[index + 1];
    }
    (*noOfEmployees)--;

    printf("\nEmployee removed successfully.\n\n");
}

void displayEmployeeHierarchy(struct employee employee[], int numberOfEmployees)
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
    printHierarchy(employee, numberOfEmployees, employeeIndex, 0);
    printf("\nTotal Employees Under %d: %d\n\n", employeeId, getTotalReportsCount(employee, numberOfEmployees, employeeIndex));
}

void displayCompleteHierarchy(struct employee employee[], int numberOfEmployees)
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
            printHierarchy(employee, numberOfEmployees, index, 0);
            printf("\nTotal Employees: %d\n\n", 1 + getTotalReportsCount(employee, numberOfEmployees, index));
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
                displayEmployeeHierarchy(employee, numberOfEmployees);
                break;

            case 4:
                displayCompleteHierarchy(employee, numberOfEmployees);
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