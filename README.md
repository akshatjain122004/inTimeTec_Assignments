# Calculator

A simple command-line calculator written in C that evaluates a basic arithmetic expression entered as a string.

## Features

- Reads a single-line expression from standard input
- Supports addition (`+`), subtraction (`-`), multiplication (`*`), and division (`/`)
- Evaluates expressions left-to-right (no operator precedence/BEMDAS)
- Supports non-negative integers only
- Accepts the expression with or without surrounding double quotes
- Handles basic error cases: invalid expressions and division by zero

## How It Works

The program reads a line of input, strips optional surrounding quotes and whitespace, then parses the expression character by character, accumulating the result as it processes each number and operator in sequence.

## Building

gcc calculator.c -o calculator

Usage

./calculator

Example:

Enter an expression:
5 + 3 * 2 - 4 / 2

Output:

14

▎ Note: Since operators are evaluated left-to-right without precedence rules, 5 + 3 * 2 is computed as (5 + 3) * 2, not standard math order.

Error Handling

- Invalid characters or malformed expressions print Error: Invalid expression.
- Dividing by zero prints Error: Division by zero.

Files

- calculator.c — source code
- calculator.exe — compiled Windows executable

# CRUD

## CRUD User Management System

A simple command-line CRUD (Create, Read, Update, Delete) application written in C that manages user records stored in a plain text file.

## Features

- Add a new user (with duplicate ID check)
- View all users
- Update an existing user's name and age by ID
- Delete a user by ID
- Persists data in `users.txt` using a comma-separated format

## Data Model

Each user record consists of:

| Field | Type        |
|-------|-------------|
| ID    | integer     |
| Name  | string (up to 49 chars) |
| Age   | integer     |

Records are stored one per line in `users.txt` as:

id,name,age

## Building

gcc crud.c -o crud

Usage

./crud

On launch, a menu is displayed:

MENU
1. Add user
2. Show users
3. Update user
4. Delete user
5. Exit

Follow the prompts to perform the desired operation. The program loops until option 5 (Exit) is selected.

How It Works

- users.txt is created automatically on startup if it doesn't already exist.
- Add/Update/Delete operations read through users.txt, and Update/Delete rewrite the file via a temporary file (temp.txt) that replaces the original.
- Duplicate IDs are rejected when adding a new user.

Files

- crud.c — source code
- crud.exe — compiled Windows executable
- users.txt — data file where user records are stored
