#include <stdio.h>

int main()
{
    const float STANDARD_PRICE = 150;
    const float PREMIUM_PRICE = 250;
    const float RECLINER_PRICE = 400;
    const float DISCOUNT_RATE = 0.10;
    char customerName[50];
    int ticketCount;
    int categoryChoice;
    float ticketPrice;
    float subtotal;
    float discountAmount;
    float finalBill;
    const char *categoryName;
    printf("Enter customer name: ");
    scanf("%49s", customerName);
    printf("Enter number of tickets (1-20): ");
    scanf("%d", &ticketCount);
    printf("Enter ticket category (1 = Standard, 2 = Premium, 3 = Recliner): ");
    scanf("%d", &categoryChoice);
    if (ticketCount < 1 || ticketCount > 20)
    {
        printf("Invalid number of tickets. Please enter a value between 1 and 20.\n");
        return 0;
    }
    switch (categoryChoice)
    {
        case 1:
            categoryName = "Standard";
            ticketPrice = STANDARD_PRICE;
            break;
        case 2:
            categoryName = "Premium";
            ticketPrice = PREMIUM_PRICE;
            break;
        case 3:
            categoryName = "Recliner";
            ticketPrice = RECLINER_PRICE;
            break;
        default:
            printf("Invalid ticket category. Please enter 1, 2 or 3.\n");
            return 0;
    }
    subtotal = ticketPrice * ticketCount;
    if (ticketCount >= 5)
    {
        discountAmount = subtotal * DISCOUNT_RATE;
    }
    else
    {
        discountAmount = 0;
    }
    finalBill = subtotal - discountAmount;
    printf("\nCustomer: %s\n", customerName);
    printf("Ticket Category: %s\n", categoryName);
    printf("Tickets: %d\n", ticketCount);
    printf("Subtotal: %.2f\n", subtotal);
    printf("Discount: %.2f\n", discountAmount);
    printf("Final Bill: %.2f\n", finalBill);
    return 0;
}