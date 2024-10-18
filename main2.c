//#include <stdio.h>
//#include <ctype.h>
//
//struct Varibale {
//    char name[100];
//    int var[100];
//};
//
//int operators(char chars[200]) {
//    int one;
//    for (int i = 0; i < sizeof(chars); i++) {
//        if (chars[i + 1] == '*') {
//            one = chars[i];
//        }
//        if (chars[i] == '*') {
//            printf("%d\n", one * chars[i + 1]);
//        }else if (chars[i] == '/') {
//            printf("%d\n", one / chars[i + 1]);
//        }else if (chars[i] == '+') {
//            printf("%d\n", one + chars[i + 1]);
//        }
//    }
//    return 0;
//}
//
//void code_check(char chars[300], int length) {
//
//    struct Varibale loc_var[100];
//    char all_code[300];
//    char tmp_code[100];
//    int x;
//    int var_el = 0;
//    int i = 0;
//    int j = 0;
//    while (i < length) {
//        if (chars[i-3] == 'i' && chars[i-2] == 'n' && chars[i-1] == 't') {
//            j = (int)i + 1;
//        }else if(chars[i-3] == 'f' && chars[i-2] == 'l' && chars[i-1] == 't'){
//            float j = (float)i + 1;
//            printf("%f\n", j);
//        }
//            int tmp_index_char = 0;
//            while (j < length) {
//                char tmp_char_name = chars[j];
//                loc_var[var_el].name[tmp_index_char] = tmp_char_name;
//                tmp_index_char++;
//
//                j++;
//                if (chars[j] == '=') {
//                    int index_var = 0;
//                    int k = j+1;
//                    while (k < length) {
//                        int tmp_var_val = chars[k];
//                        loc_var[var_el].var[index_var] = tmp_var_val;
//                        index_var++;
//                        k++;
//                        if (chars[k] == ';') {
//                            k = length;
//                        }
//                    }
//                    j = length;
//                }
//            }
//            var_el++;
//
//        i++;
//    }
//    printf("%f",loc_var[1].var[1]);
//
//    char charset1[20] = {loc_var[0].var[0], '*', loc_var[1].var[1]};
//    char charset2[20] = {loc_var[0].var[0], '/', loc_var[0].var[0]};
//    char charset3[20] = {loc_var[0].var[0], '+', loc_var[0].var[0]};
//
//    operators(charset1);
//    operators(charset2);
//    operators(charset3);
//}
//
//int main() {
//    char code[] = {
//            'i', 'n', 't', ' ', 'x', '=', 10, ';',
//            'f', 'l', 't', ' ', 'y', '=', 5, ';',
//            'i', 'n', 't', ' ', 'z', '=', 10, ';'
//    };
//    int length = sizeof(code) / sizeof(code[0]);
//
//    code_check(code, length);
//
//    return 0;
//}
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char items[MAX];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

int isFull(Stack *s) {
    return s->top == MAX - 1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, char item) {
    if (!isFull(s)) {
        s->items[++(s->top)] = item;
    }
}

char pop(Stack *s) {
    if (!isEmpty(s)) {
        return s->items[(s->top)--];
    }
    return '\0';
}

char peek(Stack *s) {
    if (!isEmpty(s)) {
        return s->items[s->top];
    }
    return '\0';
}

int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

void infixToPostfix(const char *infix, char *postfix) {
    Stack s;
    initStack(&s);
    int i, j = 0;

    for (i = 0; infix[i] != '\0'; i++) {
        if (isalnum(infix[i])) {
            postfix[j++] = infix[i]; // Если операнд, добавляем в выходную строку
        } else if (infix[i] == '(') {
            push(&s, infix[i]); // Если '(', помещаем в стек
        } else if (infix[i] == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[j++] = pop(&s); // Извлекаем из стека до '('
            }
            pop(&s); // Удаляем '(' из стека
        } else { // Оператор
            while (!isEmpty(&s) && precedence(peek(&s)) >= precedence(infix[i])) {
                postfix[j++] = pop(&s);
            }
            push(&s, infix[i]);
        }
    }

    while (!isEmpty(&s)) {
        postfix[j++] = pop(&s);
    }
    postfix[j] = '\0'; // Завершаем строку
}

int main() {
    char infix[MAX], postfix[MAX];
    printf("Enter : ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = 0; // Удаление символа новой строки
    infixToPostfix(infix, postfix);
    printf("Val: %s\n", postfix);

    return 0;
}