# Employee Hierarchy Management System

A menu-driven C program that manages an organization's employee hierarchy using structures, arrays and recursion.

## Features

1. **Add Employee**: enter ID, name, designation and reporting manager (`-1` for the CEO). The new employee is also added to the manager's list of reports.
2. **Remove Employee**: the removed employee's direct reports move to the sibling with the fewest direct reports. If there are no siblings, they move to the manager. The CEO cannot be removed.
3. **Display Employee Hierarchy**: shows an employee with everyone under them, plus the total count under that employee.
4. **Display Complete Hierarchy**: shows the whole organization from the CEO, plus the total number of employees.
5. **Exit**

## Rules Enforced

- Employee IDs are unique.
- Each employee has at most one manager and cannot report to themselves.
- Only one top-level employee (CEO) is allowed, so there are no circular reports.
- Maximum of 100 employees.

## Recursion

`displayHierarchyRecursive` prints an employee and then calls itself for each direct report. `countSubordinates` returns 1 plus the count of each report's subtree, for every direct report.

## Build and Run

```
gcc employee_hierarchy.c -o employee_hierarchy
./employee_hierarchy
```

## Example

```
===== ORGANIZATION HIERARCHY =====
101 - Rahul - CEO
    102 - Amit - EngManager
        104 - Neha - Developer
        105 - Raj - Developer
    103 - Priya - HRManager
        106 - Karan - HRExecutive

Total Employees: 6
```

Removing 102 moves Neha and Raj under Priya (103), the only sibling.

## Note

Names and designations are read with `%s`, so enter them as a single word (for example `EngManager`).
