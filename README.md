# Movie Ticket Booking System

A console-based C program for a small cinema ticket counter. It reads the customer name, number of tickets and ticket category, then calculates the subtotal, discount and final bill.

## Ticket Categories

| Choice | Category | Price per Ticket |
|--------|----------|------------------|
| 1      | Standard | 150              |
| 2      | Premium  | 250              |
| 3      | Recliner | 400              |

## Discount

A 10% discount is applied to the subtotal when 5 or more tickets are booked. Otherwise the discount is 0.

## Concepts Used

Variables, data types, constants, input/output, arithmetic operators, `switch-case` and `if-else`.

## Run

```
gcc movie_ticket_booking_system.c -o movie_ticket_booking_system
./movie_ticket_booking_system
```

## Input

1. Customer name (single word)
2. Number of tickets (1 to 20)
3. Ticket category (1, 2 or 3)

An error message is shown if the ticket count or category is invalid.

## Sample Output

```
Customer: Asha
Ticket Category: Premium
Tickets: 5
Subtotal: 1250.00
Discount: 125.00
Final Bill: 1125.00
```
