#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    printf("Enter an expression: \n");
    char line[1000];

    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error: Invalid expression.\n");
        return 0;
    }
    line[strcspn(line, "\r\n")] = '\0';

    // if the input is wrapped in quotes, drop them
    char *s = line;
    int len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        s[len - 1] = '\0';
        s++;
    }

    long total = 0;   // sum of finished terms
    long term = 0;    // term we are currently building (handles * and /)
    char op = '+';    // operator that came before the current number
    int i = 0;

    while (1) {
        while (isspace(s[i])) i++;

        // a number must come here
        if (!isdigit(s[i])) {
            printf("Error: Invalid expression.\n");
            return 0;
        }

        long num = 0;
        while (isdigit(s[i])) {
            num = num * 10 + (s[i] - '0');
            i++;
        }

        if (op == '+') {
            total += term;
            term = num;
        } else if (op == '-') {
            total += term;
            term = -num;
        } else if (op == '*') {
            term = term * num;
        } else if (op == '/') {
            if (num == 0) {
                printf("Error: Division by zero.\n");
                return 0;
            }
            term = term / num;
        }

        while (isspace(s[i])) i++;

        if (s[i] == '\0')
            break;

        if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
            op = s[i];
            i++;
        } else {
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }

    total += term;
    printf("%ld\n", total);
    return 0;
}