#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char inputLine[1000];

    printf("Enter an expression: \n");

    if (fgets(inputLine, sizeof(inputLine), stdin) == NULL)
    {
        printf("Error: Invalid expression.\n");
        return 0;
    }
    inputLine[strcspn(inputLine, "\r\n")] = '\0';

    char *expression = inputLine;
    int expressionLength = strlen(expression);

    if (expressionLength >= 2 && expression[0] == '"' && expression[expressionLength - 1] == '"')
    {
        expression[expressionLength - 1] = '\0';
        expression++;
    }

    long totalResult = 0;
    long currentTerm = 0;
    char currentOperator = '+';
    int index = 0;

    while (1)
    {
        while (isspace(expression[index]))
        {
            index++;
        }

        if (!isdigit(expression[index]))
        {
            printf("Error: Invalid expression.\n");
            return 0;
        }

        long currentNumber = 0;
        while (isdigit(expression[index]))
        {
            currentNumber = currentNumber * 10 + (expression[index] - '0');
            index++;
        }

        if (currentOperator == '+')
        {
            totalResult += currentTerm;
            currentTerm = currentNumber;
        }
        else if (currentOperator == '-')
        {
            totalResult += currentTerm;
            currentTerm = -currentNumber;
        }
        else if (currentOperator == '*')
        {
            currentTerm = currentTerm * currentNumber;
        }
        else if (currentOperator == '/')
        {
            if (currentNumber == 0)
            {
                printf("Error: Division by zero.\n");
                return 0;
            }
            currentTerm = currentTerm / currentNumber;
        }

        while (isspace(expression[index]))
        {
            index++;
        }

        if (expression[index] == '\0')
        {
            break;
        }

        if (expression[index] == '+' || expression[index] == '-' || expression[index] == '*' || expression[index] == '/')
        {
            currentOperator = expression[index];
            index++;
        }
        else
        {
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }

    totalResult += currentTerm;
    printf("%ld\n", totalResult);
    return 0;
}