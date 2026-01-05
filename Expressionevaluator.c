/*
Project: Infix to Postfix Converter & Expression Evaluator
Language: C
Data Structure: Stack
Author: Siri Hegde
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAX 100

/* Stack for operators */
typedef struct {
    int top;
    char data[MAX];
} CharStack;

/* Stack for operands */
typedef struct {
    int top;
    int data[MAX];
} IntStack;

/* Function declarations */
int precedence(char);
int isOperator(char);
void pushChar(CharStack *, char);
char popChar(CharStack *);
char peekChar(CharStack *);
void pushInt(IntStack *, int);
int popInt(IntStack *);
void infixToPostfix(char[], char[]);
int evaluatePostfix(char[]);

/* MAIN FUNCTION */
int main() {
    char infix[MAX], postfix[MAX];
    int choice;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Convert Infix to Postfix\n");
        printf("2. Evaluate Postfix Expression (digits only)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        /* FIX FOR INFINITE LOOP */
        if (scanf(" %d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');  // clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter infix expression: ");
                scanf("%s", infix);
                infixToPostfix(infix, postfix);
                printf("Postfix Expression: %s\n", postfix);
                break;

            case 2:
                printf("Enter postfix expression: ");
                scanf("%s", postfix);
                printf("Evaluation Result: %d\n", evaluatePostfix(postfix));
                break;

            case 3:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 3);

    return 0;
}

/* OPERATOR PRECEDENCE */
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

/* CHECK OPERATOR */
int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

/* STACK OPERATIONS (CHAR) */
void pushChar(CharStack *s, char ch) {
    s->data[++s->top] = ch;
}

char popChar(CharStack *s) {
    return s->data[s->top--];
}

char peekChar(CharStack *s) {
    return s->data[s->top];
}

/* STACK OPERATIONS (INT) */
void pushInt(IntStack *s, int val) {
    s->data[++s->top] = val;
}

int popInt(IntStack *s) {
    return s->data[s->top--];
}

/* INFIX TO POSTFIX CONVERSION */
void infixToPostfix(char infix[], char postfix[]) {
    CharStack s;
    s.top = -1;
    int k = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        if (isalnum(ch)) {
            postfix[k++] = ch;
        }
        else if (ch == '(') {
            pushChar(&s, ch);
        }
        else if (ch == ')') {
            while (s.top != -1 && peekChar(&s) != '(')
                postfix[k++] = popChar(&s);
            popChar(&s); // remove '('
        }
        else if (isOperator(ch)) {
            while (s.top != -1 &&
                  (precedence(peekChar(&s)) > precedence(ch) ||
                  (precedence(peekChar(&s)) == precedence(ch) && ch != '^')))
                postfix[k++] = popChar(&s);

            pushChar(&s, ch);
        }
    }

    while (s.top != -1)
        postfix[k++] = popChar(&s);

    postfix[k] = '\0';
}

/* POSTFIX EVALUATION */
int evaluatePostfix(char postfix[]) {
    IntStack s;
    s.top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char ch = postfix[i];

        if (isdigit(ch)) {
            pushInt(&s, ch - '0');
        }
        else if (isOperator(ch)) {
            int b = popInt(&s);
            int a = popInt(&s);
            int res;

            switch (ch) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/': res = a / b; break;
                case '^': res = (int)pow(a, b); break;
            }
            pushInt(&s, res);
        }
    }
    return popInt(&s);
}
