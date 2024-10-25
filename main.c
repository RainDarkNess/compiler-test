#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>

struct var {
    char name[1024];
    char value[1024];
    char type[1024];
    bool isNull;
};

int vars = 0;
char words[4][1024][1024] = {{ // types
                                     {"char"},
                                     {"int"},
                                     {"float"},
                                     {"bool"},
                             },
                             { // delimiters
                                     {"assign"}, // 0
                                     {";"}, // 1
                                     {"GRT"}, // 2
                                     {' '}, // 3
                                     {"LOWT"}, // 4
                                     {"begin"}, // 5
                                     {"if"}, // 6
                                     {'('}, // 7
                                     {')'}, // 8
                                     {'-', '-'}, // 9
                                     {"-"}, // 10 "-"
                                     {'+', '+'}, // 11
                                     {"|"}, // 12 "+"
                                     {"*"}, // 13 "*"
                                     {"/"}, // 14 "/"
                                     {"then"}, // 15
                                     {"end."}, // 16
                                     {"print"}, // 17
                                     {"for"}, // 18
                                     {"val"}, // 19
                                     {"do"}, // 20
                                     {"while"}, // 21
                                     {"next"}, // 22
                                     {":"}, // 23
                                     {"program"}, // 24
                                     {"var"}, // 25
                                     {"NEQ"}, // 26
                                     {"EQV"}, // 27
                                     {"LOWE"}, // 28
                                     {"GRE"}, // 29
                                     {"end"}, // 30
                                     {","}, // 31
                                     {"["}, // 32
                                     {"]"}, // 33
                                     {"displ"}, // 34
                             },
                             { // vars values
                                     {112},

                             },
                             { // vars names
                                     {'a', 'a'},

                             }
};

char map[1024];
// TEMPORARY
char *map_2[1024];

char prohibited[] = {';', '=', '>', '<', '?', ':', '\0'};

char file_view[] = {'%', 'd', ',', '%', 'd', ';'};

struct var def_vars[1024];
int vars_count = 0;

// NOT MINE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void file_write(const char *filepath, const char *data) {
    FILE *fp = fopen(filepath, "ab");
    if (fp == NULL) {
        perror("РћС€РёР±РєР° РїСЂРё РѕС‚РєСЂС‹С‚РёРё С„Р°Р№Р»Р°");
        return;
    }

    if (data != NULL) {
        if (fputs(data, fp) == EOF) {
            perror("РћС€РёР±РєР° РїСЂРё Р·Р°РїРёСЃРё РІ С„Р°Р№Р»");
        }
    } else {
        fprintf(stderr, "РћС€РёР±РєР°: РґР°РЅРЅС‹Рµ РґР»СЏ Р·Р°РїРёСЃРё СЂР°РІРЅС‹ NULL\n");
    }

    fclose(fp);
}


char *hexToBinary(char hex[]) {
    int len = strlen(hex);
    char *binary = (char *) malloc((len * 4 + 1) * sizeof(char));
    binary[0] = '\0';
    for (int i = 0; i < len; i++) {
        switch (hex[i]) {
            case '0':
                strcat(binary, "0000");
                break;
            case '1':
                strcat(binary, "0001");
                break;
            case '2':
                strcat(binary, "0010");
                break;
            case '3':
                strcat(binary, "0011");
                break;
            case '4':
                strcat(binary, "0100");
                break;
            case '5':
                strcat(binary, "0101");
                break;
            case '6':
                strcat(binary, "0110");
                break;
            case '7':
                strcat(binary, "0111");
                break;
            case '8':
                strcat(binary, "1000");
                break;
            case '9':
                strcat(binary, "1001");
                break;
            case 'A':
            case 'a':
                strcat(binary, "1010");
                break;
            case 'B':
            case 'b':
                strcat(binary, "1011");
                break;
            case 'C':
            case 'c':
                strcat(binary, "1100");
                break;
            case 'D':
            case 'd':
                strcat(binary, "1101");
                break;
            case 'E':
            case 'e':
                strcat(binary, "1110");
                break;
            case 'F':
            case 'f':
                strcat(binary, "1111");
                break;
            default:
                printf("Invalid hexadecimal digit\n");
                return NULL;
        }
    }
    return binary;
}

//char *decToBinary(char dec[]) {
char* decToBinary(void *answer, size_t size) {

        char* byte_ptr = (char *) malloc(size);
        strcpy(byte_ptr, answer);
        memset(answer, '\0', sizeof(size));
//        for (size_t i = 0; i < size; i++) {
            char temp[300];
            memset(temp, '\0', sizeof(temp));
            sprintf(temp + strlen(temp), "%02hhx", byte_ptr);
            strcat(answer, temp);
//        }
        return answer;
}

char *octToBinary(char oct[]) {
    int len = strlen(oct);
    char *binary = (char *) malloc((len * 3 + 1) * sizeof(char));
    binary[0] = '\0';
    for (int i = 0; i < len; i++) {
        switch (oct[i]) {
            case '0':
                strcat(binary, "000");
                break;
            case '1':
                strcat(binary, "001");
                break;
            case '2':
                strcat(binary, "010");
                break;
            case '3':
                strcat(binary, "011");
                break;
            case '4':
                strcat(binary, "100");
                break;
            case '5':
                strcat(binary, "101");
                break;
            case '6':
                strcat(binary, "110");
                break;
            case '7':
                strcat(binary, "111");
                break;
            default:
                printf("Invalid octal digit\n");
                return NULL;
        }
    }
    return binary;
}


char *number_validation(char *value) {

    char latin_hex_alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f', 'x'};

    bool exponential_write_plus = false;
    bool exponential_write_exp = false;
    bool is_hex = false;
    bool valid = true;
    bool is_real = false;

    char *answer;
    int i = 0;
    char tmp_value[1024];
    memset(tmp_value, '\0', sizeof(tmp_value));
    unsigned long long str_len_value =
            !isdigit(value[strlen(value) - 1]) && strlen(value) > 1 ? strlen(value) - 1 : strlen(value);
    for (; i < str_len_value; i++) {
        tmp_value[i] = value[i];

        if (value[i] == 'E' || value[i] == 'e') {
            printf("Exponential read?\n");
            exponential_write_exp = true;
        }
        if (value[i] == '+' || value[i] == '-') {
            printf("Plus find...Exponential read.\n");
            if (exponential_write_exp)
                exponential_write_plus = true;
            else
                printf("Error. In exp writing not found 'E'\n");
        }
        if (!exponential_write_plus && !exponential_write_exp) {
            if (!isdigit(value[i])) {
                bool for_real_hex_check = false;
                for (int j = 0; j < sizeof(latin_hex_alphabet) + 1; j++) {
                    if (value[i] == latin_hex_alphabet[j]) {
                        is_hex = true;
                        for_real_hex_check = true;
                        break;
                    }
                }
                if (value[i] == '.') {
                    if(is_real){
                        printf("Error. Find double dot\n");
                        return "false_verification";
                    }
                    is_real = true;
                    continue;
                } else if (!is_hex) {
                    printf("Error. Find letter but it not in hex table\n");
                    valid = false;
                    break;
                }
                if (is_real && !for_real_hex_check) {
                    printf("Error. Real format number not valid\n");
                    valid = false;
                    break;
                }
            }
        }else{
            if (value[i] == '.') {
                printf("Error. Find dot after exp\n");
                return "false_verification";
            }
        }
    }

    if (exponential_write_exp && !exponential_write_plus) {
        printf("In exp writing not found '+'\n");
    }

    if (valid) {

        double answer_double = 0;
        float answer_float = 0;
        int answer_int = 0;

        char last_char = value[i];

        if (last_char == 'H' || last_char == 'h') {
            printf("Digit is hex \n");
            answer = hexToBinary(tmp_value);
        } else if (last_char == 'B' || last_char == 'b') {
            printf("Digit is binary \n");
        } else if (last_char == 'O' || last_char == 'o') {
            printf("Digit is oct \n");
            answer = octToBinary(tmp_value);
        } else if (last_char == '.') {
            printf("Error. Real format number not valid\n");
            valid = false;
        } else {
            char *endptr_l;

            if (exponential_write_exp) {
                if(is_hex){
                    answer_double = atof(tmp_value);
                    snprintf(tmp_value, 10, "%a", answer_double);
                }else{
                    answer_int = atoi(tmp_value);
                    snprintf(tmp_value, 10, "%x", answer_int);
                }
            } else if (is_real) {
                answer_float = strtof(tmp_value, &endptr_l);
                snprintf(tmp_value, 10, "%a", answer_float);
            } else {
                answer_int = atoi(tmp_value);
                snprintf(tmp_value, 10, "%b", answer_int);
            }
//            printf("Digit is decimal \n");
//            answer = decToBinary(tmp_value);
//            answer = decToBinary(tmp_value, strlen(tmp_value));
            answer = tmp_value;
        }
    }
    if (answer == NULL)
        valid = false;

    if (valid)
        return answer;
    else
        return "false_verification";
}

char **str_split(const char *str, char delimiter) {
    int i, j, k;
    size_t len = strlen(str);
    int substring_count = 1;

    for (i = 0; i < len; i++) {
        if (str[i] == delimiter) {
            substring_count++;
        }
    }
    char **result = (char **) malloc(substring_count * sizeof(char *));
    if (!result) {
        return NULL;
    }

    i = 0;
    j = 0;
    for (k = 0; k < substring_count; k++) {
        int start = i;
        while (i < len && str[i] != delimiter) {
            i++;
        }
        int substr_len = i - start;
        result[k] = (char *) malloc((substr_len + 1) * sizeof(char));
        if (!result[k]) {
            for (int m = 0; m < k; m++) {
                free(result[m]);
            }
            free(result);
            return NULL;
        }
        strncpy(result[k], &str[start], substr_len);
        result[k][substr_len] = '\0'; // Добавляем завершающий нулевой символ
        i++; // Пропускаем разделитель
    }

    return result;
}

bool isNumber(char const *const text) {
    if (text == NULL || text[0] == '\0') {
        return false;
    }

    int dot_counter = 0;
    size_t length = 1;
    for (char character = text[1]; character != '\0';
         ++length, character = text[length]) {
        bool const is_valid_character =
                (character >= '0' && character <= '9') ||
                (character == '.' && ++dot_counter == 1);

        if (!is_valid_character) {
            return false;
        }
    }

    char const first_character = text[0];
    bool is_character_sign = (first_character == '-' || first_character == '+');
    // verifications for first left character
    if ((is_character_sign || first_character == '.') && length == 1) {
        return false;
    }
    if (length == 2 && is_character_sign && text[1] == '.') {
        return false;
    }
    return (is_character_sign || first_character == '.') ||
           (first_character >= '0' && first_character <= '9');
}

void removeSpacesAndNewlines(char *str) {
    char *result = str;
    bool comment_found = false;

    for (int i = 0; str[i] != '\0'; i++) {

        if (str[i] == '#') {
            if (comment_found) {
                i++;
            }
            comment_found = !comment_found;
        }

        if (!isspace((unsigned char) str[i]) && !comment_found) {
            *result = str[i];
            result++;
        }
    }
    *result = '\0';
}

void hexToAscii(char *hex, char *ascii) {


    char *endptr;

    long decimalValue = strtol(hex, &endptr, 16);

    sprintf(ascii, "%f", atof(hex));

}

void reverseBytes(const char *input, char *output, int len) {
    for (int i = 0; i < len; i++) {
        output[i] = input[len - 1 - i];
    }
    output[len] = '\0';
}


const char *check_type_data(const char *value) {
    const char *type_data = "int";
    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        type_data = "bool";
        return type_data;

    }
//
//    int len = strlen(value);
//    char ascii[len / 2 + 1];
//    char littleEndian[len + 1];
//
//    // Преобразование в ASCII
//
//    // TEMPORARY
//    hexToAscii(value, ascii);
//    printf("ASCII: %s\n", ascii);
//
//    value = ascii;


    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        type_data = "bool";
    }

    for (int i = 0; *(value + i); i++) {
        if (value[i] == '.') {
            type_data = "float";
            break;
        }
    }
    return type_data;
}

int putVarValToVar(int number_dist, int number_source) {
    int answer = 0;
    if (strcmp(def_vars[number_source].type, def_vars[number_dist].type) == 0) {
        printf("Var %s with old value: ", def_vars[number_dist].name);
        printf("%s ", def_vars[number_dist].value);
        strcpy(def_vars[number_dist].value, def_vars[number_source].value);
        printf("has new value: %s type: %s\n", def_vars[number_dist].value, def_vars[number_source].type);
        printf(" from %s\n", def_vars[number_source].name);
        def_vars[number_dist].isNull = false;

    } else {
        printf("Type of var: %s is %s, given %s, from var %s\n", def_vars[number_dist].name, def_vars[number_dist].type,
               def_vars[number_source].type, def_vars[number_source].name);
        answer = 1;
    }

    return answer;
}

int putValToVar(int number_dist, char value[]) {
    int answer = 0;

    const char *type;
    type = check_type_data(value);

    if (strcmp(def_vars[number_dist].type, type) == 0) {
        printf("Var %s with old value: ", def_vars[number_dist].name);
        printf("%s ", def_vars[number_dist].value);
        printf(" from %s\n", value);

        strcpy(def_vars[number_dist].value, value);
        printf("has new value: %s type: %s\n", value, def_vars[number_dist].type);

    } else {
        printf("Type of var: %s is %s, given %s\n", def_vars[number_dist].name, def_vars[number_dist].type, type);
        answer = 1;
    }
    def_vars[number_dist].isNull = false;

    return answer;
}


char *readFile(const char *filePath) {
    FILE *file_ptr;
    char *string = NULL;
    size_t size = 0;
    size_t index = 0;
    char ch;

    file_ptr = fopen(filePath, "r");
    if (NULL == file_ptr) {
        printf("file not found‚\n");
        return NULL;
    }

    string = malloc(1024);
    if (string == NULL) {
        printf("er\n");
        fclose(file_ptr);
        return NULL;
    }

    while ((ch = fgetc(file_ptr)) != EOF) {
        if (index >= size - 1) {
            size += 1024; // РЈРІРµР»РёС‡РёРІР°РµРј СЂР°Р·РјРµСЂ РЅР° 1024
            string = realloc(string, size);
            if (string == NULL) {
                printf("er\n");
                fclose(file_ptr);
                return NULL;
            }
        }
        string[index++] = ch;
    }
    string[index] = '\0';


    fclose(file_ptr);
    return string;
}


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
        case '|':
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

        if (isalnum(infix[i]) || infix[i] == '.') {
            postfix[j++] = infix[i]; // Если операнд, добавляем в выходную строку
        } else if (infix[i] == '(') {
            push(&s, infix[i]); // Если '(', помещаем в стек
        } else if (infix[i] == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[j++] = pop(&s); // Извлекаем из стека до '('
            }
            pop(&s); // Удаляем '(' из стека
        }else { // Оператор
            postfix[j++] = ' ';
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

void get_buffer_from_token(const char *tokens, char* _buffer, char* _buffer2){
    char buffer[2][100];
    memset(buffer, 0, sizeof(buffer));
    bool first = true;
    int j = 0;
    for (int i = 0; tokens[i] != '\0'; i++) {
        if (tokens[i] == ',') {
            first = false;
            j = 0;
            continue;
        }
        if (first)buffer[0][j] = tokens[i];
        else buffer[1][j] = tokens[i];
        j++;
    }
    j = 0;
    strcpy(_buffer, buffer[0]);
    strcpy(_buffer2, buffer[1]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool code_work() {

    char *result = (char *) malloc(1024+sizeof(char*));
    char template[1024];

    memset(template, '\0', sizeof(template));
    memset(result, '\0', 1024+sizeof(char*));

    strcat(result, ".text\n"
                                ".globl\tmain\n");
    strcat(result, ".data\n");

    // init variables
    for (int i = 0; i < vars_count; i++) {
        sprintf(template, "\t%s: .%s %s\n", def_vars[i].name, "quad", def_vars[i].value);
        strcat(result, template);
        memset(template, '\0', sizeof(template));

    }

    // Init main function program
    strcat(result, "main:\n"
                   "\tpushq\t%rbp\n"
                   "\tmovq\t%rsp, %rbp\n"
                   "\tsubq\t$48, %rsp\n"
                   "\tmov $0, %rax\n");

    // IMPORTANT COMMENTS

//    char* string = readFile("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt");

    char *tokens = strtok(map_2, ";");

    bool is_condition_start = false;
    bool begin_find = false;
    bool has_oper = false;
    bool has_then = false;
    char stack[1024];
    memset(stack, '\0', sizeof(stack));

    char infix[MAX], postfix[MAX];
    memset(postfix, '\0', sizeof(postfix));
    memset(infix, '\0', sizeof(infix));


    char variable[1024];
    memset(variable, '\0', sizeof(variable));

    while (tokens != NULL) {
        if(strcmp(tokens, "1,5") == 0) {
            begin_find = true;
            tokens = strtok(NULL, ";");
            tokens = strtok(NULL, ";");
        }
        if(begin_find) {
            char buffer[2][100];
            memset(buffer[0], '\0', sizeof(buffer[0]));
            memset(buffer[1], '\0', sizeof(buffer[1]));


            if (is_condition_start) {

                if (strcmp(tokens, "1,1") == 0) {
                    is_condition_start = false;
                    infix[strcspn(infix, "\n")] = 0;
                    infixToPostfix(infix, postfix);
                    printf("Val:%s\n", postfix);
                    char stack_asm[100][100];
                    memset(stack_asm, '\0', sizeof(stack_asm));

                    int index_stack = 0;
                    int v_index = 0;

                    memset(template, '\0', sizeof(template));
                    bool was_val = false;
                    bool was_del = false;
                    bool has_condition = false;
                    for (int i = 0; *(postfix + i); ++i) {
                        if(postfix[i] == '|' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/'){
                            has_condition = true;
                        }
                    }
                    if(has_condition){
                        for (int j = 0; *(postfix + j); j++) {
                            if (isdigit(postfix[j])) {
                                if (was_del) {
                                    for(int i = 0; i< strlen(stack_asm[index_stack]); i++){
                                        if(!isdigit(stack_asm[index_stack][i])){
                                            strcat(stack_asm[index_stack], "(%rip)");
                                            break;
                                        }
                                    }
                                    index_stack++;
                                    was_del = false;
                                    stack_asm[index_stack][v_index++] = '$';
                                }
                                stack_asm[index_stack][v_index] = postfix[j];
                                was_val = true;
                                v_index++;

                            } else {
                                bool skip = true;
                                if (postfix[j] == '|') {
                                    if (was_val) {
                                        if (strcmp(stack_asm[index_stack - 1], "") != 0) {
                                            sprintf(template, "\tadd %s, %crax\n", stack_asm[index_stack - 1], '%');
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));
                                        }
                                        if (strcmp(stack_asm[index_stack - 0], "") != 0) {
                                            sprintf(template, "\tadd %s, %crax\n", stack_asm[index_stack - 0], '%');
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));

                                            sprintf(template, "\tadd %rax, %rbx\n"
                                                              "\tmov $0, %rax\n");
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));

                                        }
                                        memset(stack_asm[index_stack - 0], '\0', sizeof(stack_asm[index_stack - 0]));
                                        memset(stack_asm[index_stack - 1], '\0', sizeof(stack_asm[index_stack - 1]));
                                        index_stack -= 2;
                                        was_val = false;
                                    } else {
                                        if (strcmp(stack_asm[index_stack], "") != 0) {
                                            sprintf(template, "\tadd %s, %rbx\n"
                                                              "\tmov $0, %rax\n", stack_asm[index_stack]);
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));
                                        }
                                        sprintf(template, "\tadd %rax, %rbx\n", stack_asm[index_stack]);
                                        strcat(result, template);
                                        memset(template, '\0', sizeof(template));
                                    }
//                                memset(stack_asm, '\0', sizeof(stack_asm));
                                } else if (postfix[j] == '*') {
                                    if (was_val) {
                                        if (strcmp(stack_asm[index_stack - 0], "") != 0 &&
                                            strcmp(stack_asm[index_stack - 1], "") == 0) {
                                            sprintf(template, "\tmov %s, %rax\n", stack_asm[index_stack - 1]);
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));

                                            sprintf(template, "\tmul %rcx\n"
                                                              "\tadd %rax, %rbx\n"
                                                              "\tmov $0, %rax\n");
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));
                                            memset(stack_asm[index_stack - 0], '\0',
                                                   sizeof(stack_asm[index_stack - 0]));
                                            index_stack -= 1;
                                        } else {
                                            if (strcmp(stack_asm[index_stack - 1], "") != 0) {
                                                sprintf(template, "\tadd %s, %rax\n", stack_asm[index_stack - 1]);
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                            }
                                            if (strcmp(stack_asm[index_stack - 0], "") != 0) {
                                                sprintf(template, "\tmov %s, %rcx\n", stack_asm[index_stack - 0]);
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                                sprintf(template, "\tmul %rcx\n"
                                                                  "\tadd %rax, %rbx\n"
                                                                  "\tmov $0, %rax\n");
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                            }
                                            memset(stack_asm[index_stack - 0], '\0',
                                                   sizeof(stack_asm[index_stack - 0]));
                                            memset(stack_asm[index_stack - 1], '\0',
                                                   sizeof(stack_asm[index_stack - 1]));
                                            index_stack -= 2;
                                        }
                                        was_val = false;
                                    } else {
                                        sprintf(template, "\tadd %rax, %rbx\n"
                                                          "\tmov $0, %rax\n");
                                        strcat(result, template);
                                        memset(template, '\0', sizeof(template));
                                    }
                                    //memset(stack_asm, '\0', sizeof(stack_asm));
                                    //index_stack -= 2;
                                } else if (postfix[j] == '/') {
                                    if (was_val) {
                                        if (strcmp(stack_asm[index_stack - 0], "") != 0 &&
                                            strcmp(stack_asm[index_stack - 1], "") == 0) {
                                            sprintf(template, "\tmov %s, %crax\n", stack_asm[index_stack - 0], '%');
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));

                                            sprintf(template, "\tmov %rbx, %rcx\n"
                                                              "\tcmp $0, %rcx\n"
                                                              "\tje division_by_zero\n"
                                                              "\tdiv %rcx\n"
                                                              "\txor %rdx, %rdx\n"
                                                              "\tmov $0, %rcx\n"
                                                              "\tadd %rax, %rbx\n"
                                                              "\tmov $0, %rax\n");
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));
                                            memset(stack_asm[index_stack - 0], '\0',
                                                   sizeof(stack_asm[index_stack - 0]));
                                            index_stack -= 1;
                                        } else {
                                            if (strcmp(stack_asm[index_stack - 1], "") != 0) {
                                                sprintf(template, "\tadd %s, %rax\n", stack_asm[index_stack - 1]);
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                            }
                                            if (strcmp(stack_asm[index_stack - 0], "") != 0) {
                                                sprintf(template, "\tmov %s, %rcx\n", stack_asm[index_stack - 0]);
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                                sprintf(template, "\tcmp $0, %rcx\n"
                                                                  "\tje division_by_zero\n"
                                                                  "\txor %rdx, %rdx\n"
                                                                  "\tdiv %rcx\n"
//                                                                           "\tadd %rax, %rbx\n "
//                                                                           "\tmov $0, %rax\n   "
                                                );
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                            }
                                            memset(stack_asm[index_stack - 0], '\0',
                                                   sizeof(stack_asm[index_stack - 0]));
                                            memset(stack_asm[index_stack - 1], '\0',
                                                   sizeof(stack_asm[index_stack - 1]));
                                            index_stack -= 2;
                                        }
                                        was_val = false;
                                    } else {
                                        if (strcmp(stack_asm[index_stack], "") != 0) {
                                            sprintf(template, "mov %rbx, %rax"
                                                              "\tmov %s, %rcx\n"
                                                              "\tcmp $0, %rcx\n"
                                                              "\tje division_by_zero\n"
                                                              "\txor %rdx, %rdx\n"
                                                              "\tmov %rax, %rbx\n"
                                                              "\tmov $0, %rax\n", stack_asm[index_stack]);
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));
                                        }
//                                    sprintf(template, "\tadd %rax, %rbx\n", stack_asm[index_stack]);
//                                    strcat(result, template);
//                                    memset(template, '\0', sizeof(template));

                                        sprintf(template, "\tadd %crax, %crbx\n"
                                                          "\tmov $0, %rax\n");
                                        strcat(result, template);
                                        memset(template, '\0', sizeof(template));
                                    }
                                    //memset(stack_asm, '\0', sizeof(stack_asm));
                                    memset(stack_asm[index_stack - 0], '\0', sizeof(stack_asm[index_stack - 0]));
                                    memset(stack_asm[index_stack - 1], '\0', sizeof(stack_asm[index_stack - 1]));
                                    index_stack -= 2;
                                } else if (postfix[j] == '-') {

                                    if (was_val) {
                                        if (strcmp(stack_asm[index_stack - 0], "") != 0 &&
                                            strcmp(stack_asm[index_stack - 1], "") == 0) {
                                            sprintf(template, "\tmov %s, %crax\n"
                                                              "\tsub %rax, %rbx\n"
                                                              "\tmov $0, %rax\n", stack_asm[index_stack - 0], '%');
                                            strcat(result, template);
                                            memset(template, '\0', sizeof(template));
                                            memset(stack_asm[index_stack - 0], '\0',
                                                   sizeof(stack_asm[index_stack - 0]));
                                            index_stack -= 1;

                                        } else {
                                            if (strcmp(stack_asm[index_stack - 1], "") != 0) {
                                                sprintf(template, "\tmov %s, %rax\n", stack_asm[index_stack - 1]);
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                            }
                                            if (strcmp(stack_asm[index_stack - 0], "") != 0) {
                                                sprintf(template, "\tmov %s, %crbx\n"
                                                                  "\tsub %rax, %rbx\n"
                                                                  "\tmov $0, %rax\n", stack_asm[index_stack - 0], '%');
                                                strcat(result, template);
                                                memset(template, '\0', sizeof(template));
                                            }
                                            memset(stack_asm[index_stack - 0], '\0',
                                                   sizeof(stack_asm[index_stack - 0]));
                                            memset(stack_asm[index_stack - 1], '\0',
                                                   sizeof(stack_asm[index_stack - 1]));
                                            index_stack -= 2;
                                        }
                                        was_val = false;
                                    } else {
                                        sprintf(template, "\tsub %rax, %rbx\n"
                                                          "\tmov $0, %rax\n");
                                        strcat(result, template);
                                        memset(template, '\0', sizeof(template));
                                    }
                                    //memset(stack_asm, '\0', sizeof(stack_asm));
                                    index_stack -= 2;
                                } else if (postfix[j] == ' ') {
//                                index_stack++;
                                } else {
                                    stack_asm[index_stack][v_index] = postfix[j];
                                    was_val = true;
                                    v_index++;
                                    skip = false;
                                }
                                if (skip) {
                                    v_index = 0;
                                    was_del = true;
                                }
                            }
                        }
                        if(!has_oper) {
                            sprintf(template, "\tmov %crbx, %s(%rip)\n"
                                              "\tmov $0, %rbx\n", '%', variable);
                            strcat(result, template);
                            memset(template, '\0', sizeof(template));
                        }else{
                            sprintf(template, "\tmov %rbx, %rax\n"
                                              "\tmov $0, %rbx\n");
                            strcat(result, template);
                            memset(template, '\0', sizeof(template));
                        }
    //                    memset(variable, '\0', sizeof(variable));

                        memset(stack, '\0', sizeof(stack));
                        memset(infix, '\0', sizeof(infix));
                        memset(postfix, '\0', sizeof(postfix));
                        tokens = strtok(NULL, ";");
                    }else{
                        char* str_ch;
                        if(isdigit(*postfix)){
                            sprintf(str_ch, "\tmov $%s, %rax\n", postfix);
                        }else{
                            sprintf(str_ch, "\tmov %s(%rip), %rax\n", postfix);
                        }
                        sprintf(template, "%s\tmov %crax, %s(%rip)\n"
                                                        "\tmov $0, %rax\n", str_ch, '%', variable);
                        strcat(result, template);
                        memset(template, '\0', sizeof(template));
                    }
                }
                get_buffer_from_token(tokens, buffer[0], buffer[1]);
                strcat(infix, words[atoi(buffer[0])][atoi(buffer[1])]);
            }else{
                if(strcmp(tokens, "1,0")!=0) {
                    get_buffer_from_token(tokens, buffer[0], buffer[1]);
                    strcpy(variable, words[atoi(buffer[0])][atoi(buffer[1])]);
                }
            }

            if (strcmp(tokens, "1,0") == 0) {
                is_condition_start = true;
                memset(template, '\0', sizeof((char *) malloc(13 * sizeof(char))));
                memset(infix, '\0', sizeof(infix));
                memset(postfix, '\0', sizeof(postfix));
            }
            if (strcmp(tokens, "1,6") == 0) {
                has_oper = true;
                is_condition_start = true;
            }
//            sprintf(template, "\tmovq a(%rip), %rax\n"
//                              "\tmovq\t%rax, -8(%rbp)\n"
//                              "\tmovq\t-8(%rbp), %rax", );
//        }
            get_buffer_from_token(tokens, buffer[0], buffer[1]);

//            strcat(stack, words[atoi(buffer[0])][atoi(buffer[1])]);
            strcat(stack, tokens);
            strcat(stack, ";");
        }
        tokens = strtok(NULL, ";");
    }


    strcat(result,
                   "\tmovl\t$0, %eax\n"
                   "\taddq\t$48, %rsp\n"
                   "\tpopq\t%rbp\n"
                   "\n"
                   "\tret");

    strcat(result, "\nend_program:\n"
                   "\tmov $0, %rax\n"
                   "\taddq $48, %rsp\n"
                   "\tpopq %rbp\n"
                   "\tret\n"
                   "division_by_zero:\n"
                   "\tmov $1, %rbx \n"
                   "\tjmp end_program\n"
                   "\tmov $0, %rax\n"
                   "\tmov $0, %rbx\n");

    fclose(fopen("asm.s", "w"));
    file_write("asm.s", result);

    return false;
}

bool syntax_check(char _map[]) {
    char **tokens = str_split(_map, ';');
    int syntax_lvl = 0;
    char tmp_var_name_dec[10];

    bool not_error = true;

    // work with vars
    bool var_declaration = false;
    bool val_find_local_tmp = false;
    bool var_find_local_tmp = false;
    bool var_action = false;
    bool is_value = false;
    bool not_one = false;
    int count_vars_names = 0;
    char *tmp_vars_names[1024];
    char *type_for_check;

    int var_num_local_tmp = 0;
    int find_var = 0;
    char value_int[1024];
    memset(value_int, '\0', sizeof(value_int));

    // Equaling
    bool found_val_or_var = false;

    // Code body
    bool work_space = false;

    // Bool for if
    bool is_if = false;
    bool has_condition_if = false;
    bool then_find = false;
    bool var_find_if = false;

    // Var number for 'if'
    int var_if_number = 0;

    // Cycles bool
    bool is_while = false;
    bool has_val = false;
    bool has_condition_cycle = false;
    bool has_equaling = false;
    bool is_cycle = false;
    bool var_find_cycle = false;
    bool has_value_cycle = false;

    // In function
    bool in_function = false;

    // Program start
    bool was_start = false;
    char tmp_var_type_name[1024];

    int line = 1;

    printf("______________________________\n");
    printf("PROGRAM SYNTAX ANALYSE STARTED\n");
    printf("______________________________\n");

    for (int i = 0; *(tokens + i) != '\0'; i++) {
        if (not_error) {
            if(strlen(tokens[i]) ==0 ) break;
            char **delimited_str = str_split(tokens[i], ',');
            char *table_number = delimited_str[0];
            char *table_val = delimited_str[1];
            if ((strcmp(table_number, "1") == 0 && strcmp(table_val, "1") == 0) ||
                (strcmp(table_number, "1") == 0 && strcmp(table_val, "5") == 0)) {
                line++;
            }
            if (strcmp(table_number, "1") == 0 && strcmp(table_val, "24") == 0) {
                was_start = true;
                line++;
                printf("WORD program FIND\n");
                continue;
            } else if (strcmp(table_number, "1") == 0 && strcmp(table_val, "25") == 0 && was_start) {
                was_start = true;
                printf("WORD var FIND\n");
                continue;
            } else if (!was_start) {
                printf("Error. has not required token 'program var'\n");
                not_error = false;
                break;
            }
            if (syntax_lvl == 0) {
                if (strcmp(table_number, "3") == 0) {
                    char *var_name = delimited_str[1];
                    printf("DECLARATION NAME VAR IS:%s\n", words[3][atoi(var_name)]);
                    strcpy(tmp_var_name_dec, words[3][atoi(var_name)]);

                    for (int name = 0; name < 1024; name++) {
                        if (strcmp(def_vars[name].name, words[3][atoi(var_name)]) == 0) {
                            printf("Error. Var name is used:%s\n", words[3][atoi(var_name)]);
                            not_error = false;
                            break;
                        }
                    }
                    strcpy(def_vars[vars_count].name, words[3][atoi(var_name)]);
                    tmp_vars_names[count_vars_names] = words[3][atoi(var_name)];
                    count_vars_names++;
                }
                if (strcmp(table_number, "1") == 0 && strcmp(table_val, "31") == 0) {
                    not_one = true;
                    printf("WARNING. More than one variable\n");
                    syntax_lvl = 0;
                }
                if (strcmp(table_number, "1") == 0 && strcmp(table_val, "23") == 0) {
                    printf("DELIMITER : FIND\n");
                    syntax_lvl = 1;
                    var_declaration = true;
                }
            }
            if (strcmp(table_number, "1") == 0 && strcmp(delimited_str[1], "5") == 0) {
                syntax_lvl = 3;
                work_space = true;
                line++;
                printf("BEGIN detected\n");
            }

            if (var_declaration) {

                if (syntax_lvl == 1) { // CHECK TYPE VAR
                    char *var_number = delimited_str[1];

                    if (strcmp(table_number, "0") == 0) {
                        printf("TYPE VAR %s IS:%s\n", tmp_var_name_dec, words[0][atoi(var_number)]);
                        strcpy(tmp_var_type_name, words[0][atoi(var_number)]);

                        int pr_len = strlen(prohibited);
                        for (int j = 0; j < pr_len; j++) {
                            if (strchr(words[3][atoi(var_number)], prohibited[j]) != NULL) {
                                printf("syntax error\n");
                                printf("Name '%s' not allowed!\n", words[3][atoi(var_number)]);
                                not_error = false;
                                break;
                            }
                        }
                        if (not_one) {
                            for (int var_name = 0; var_name < count_vars_names; var_name++) {
                                strcpy(def_vars[vars_count].name, tmp_vars_names[var_name]);
                                strcpy(def_vars[vars_count].type, tmp_var_type_name);
                                def_vars[vars_count].isNull = true;
                                printf("Added new var from list of vars %s\n", def_vars[vars_count].name);
                                printf("Type is:%s\n", tmp_var_type_name);
                                vars_count++;
                            }
                        } else {
                            strcpy(def_vars[vars_count].type, tmp_var_type_name);
                            def_vars[vars_count].isNull = true;
                            vars_count++;
                        }

                        syntax_lvl = 0;
                        i++;
                        var_declaration = false;
                        memset(tmp_vars_names, '\0', sizeof(tmp_vars_names));
                        count_vars_names = 0;
                    }
                }
            }
            if (work_space) {

                if (syntax_lvl == 3) { // ENTER AFTER BEGIN

                    if (is_if && !then_find) {
                        printf("Error. Required token 'Then' in if not found\n");
                        not_error = false;
                        break;
                    }

                    if (strcmp(table_number, "1") == 0 && strcmp(table_val, "34") == 0) {
                        printf("FIND Function 'displ'\n");
                        syntax_lvl = 10;
                        printf("Trying to find variable\n");
                        in_function = true;
                    }


                    if ((val_find_local_tmp || var_find_local_tmp) && !var_action) {

                        if (strcmp(table_number, "1") == 0 && strcmp(table_val, "1") == 0) {
                            int valid = 0;
                            if (is_value) {
                                valid = putValToVar(find_var, value_int);
                                if (valid == 1) {
                                    not_error = false;
                                }
                                val_find_local_tmp = false;
                                var_find_local_tmp = false;
                                find_var, var_num_local_tmp = 0;
                            } else {
                                valid = putVarValToVar(find_var, var_num_local_tmp);
                                if (valid == 1) {
                                    not_error = false;
                                }
                                val_find_local_tmp = false;
                                var_find_local_tmp = false;
                                find_var, var_num_local_tmp = 0;
                            }
                        } else {
                            syntax_lvl = 6;
                        }

                    }

                    if (strcmp(table_val, "30") == 0 && strcmp(table_number, "1") == 0 && is_if) {
                        printf("FIND 'END'. IF END.\n");
                        is_if = false;
                        then_find = false;
                    } else if (strcmp(table_val, "30") == 0 && strcmp(table_number, "1") == 0 && !is_if) {
                        printf("Error. If is not open\n");
                        not_error = false;
                        break;
                    }
                    if (strcmp(table_val, "22") == 0 && strcmp(table_number, "1") == 0 && is_cycle) {
                        printf("FIND 'NEXT'. WHILE ENDED.\n");
                        is_cycle = false;
                    }
                    if (strcmp(table_number, "1") == 0) { // Finding delimiter
                        table_val = delimited_str[1];
                        if (strcmp(table_val, "6") == 0) { // If find
                            syntax_lvl = 7;
                            printf("IF FIND\n");
                            is_if = true;
                            continue;
                        } else if ((strcmp(table_val, "18") == 0) ||
                                   (strcmp(table_val, "21") == 0)) { // for | while find
                            syntax_lvl = 8;

                            printf("CYCLE '%s' FIND\n", words[1][atoi(table_val)]);
                            is_while = (strcmp(table_val, "21") == 0);
                            has_val = is_while;
                            has_equaling = is_while;
                            is_cycle = is_while;

                            has_condition_cycle = false;
                            continue;
                        } else {
                            printf("DELIMITER FIND '%s'\n", words[1][atoi(table_val)]);
                        }
                    } else if (strcmp(table_number, "3") == 0) { // var names
                        bool var_find = false;
                        for (int j = 0; j <= 1024; j++) {
                            if (strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0) {
                                printf("FIND VAR %s\n", words[3][atoi(table_val)]);
                                var_find = true;
                                type_for_check = def_vars[j].type;
                                find_var = j;
                                break;
                            }
                        }
                        if (var_find) {
                            syntax_lvl = 4;
                            continue;
                        } else {
                            printf("Error. unexpected token '%s'. Var is undefined\n",
                                   words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    } else { // del
                        printf("FIND '%s'\n", words[atoi(table_number)][atoi(table_val)]);
                    }
                    if (strcmp(table_number, "1") == 0 && strcmp(table_val, "16") == 0) {
                        printf("'end.' FIND, program terminated\n");
                        break;
                    }
                }
                if (syntax_lvl == 4) { // AFTER BEGIN
                    if (strcmp(table_number, "1") == 0) { // del
                        table_val = delimited_str[1];
                        printf("FIND DELIMITER %s\n", words[1][atoi(table_val)]);
                        if (strcmp(table_val, "0") == 0) {
                            printf("EQUATING\n");
                            syntax_lvl = 5;
                            continue;
                        } else if (strcmp(table_val, "6") == 0) { // if
                            syntax_lvl = 7;
                        } else { // ;
                            syntax_lvl = 3;
                        }
                        continue;
                    } else {
                        printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                }
                if (syntax_lvl == 5) { // EQUALING
                    is_value = false;
                    memset(value_int, '\0', sizeof(value_int));
                    table_val = delimited_str[1];
                    if (strcmp(table_number, "3") == 0) { // Var check
                        for (int j = 0; j < 1024; j++) {
                            if (strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0) {
                                if (!def_vars[j].isNull) {
                                    printf("EQUALING VAR %s FIND. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                    var_find_local_tmp = true;
                                    var_action = false;
                                    found_val_or_var = true;
                                    var_num_local_tmp = j;
                                    syntax_lvl = 3;
                                } else {
                                    printf("Error. VAR %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }

                            }
                        }
                        if (!var_find_local_tmp) {
                            printf("Error. unexpected token '%s'. Var is undefined\n",
                                   words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        } else {
                            continue;
                        }
                    } else if (strcmp(table_number, "2") == 0) { // Var value check
                        val_find_local_tmp = true;
                        printf("FIND VALUE IS %s\n", words[atoi(table_number)][atoi(table_val)]);
                        found_val_or_var = true;

                        syntax_lvl = 3;
                        is_value = true;
                        strcpy(value_int, words[atoi(table_number)][atoi(table_val)]);
                        var_action = false;

                        continue;
                    } else if (strcmp(table_number, "1") == 0) {
                        found_val_or_var = false;

                        syntax_lvl = 6;
                    }
                }
                if (syntax_lvl == 6) { // REUSE BLOCK FOR EQUATION
                    bool var_find_eq = false;
                    if (strcmp(table_number, "3") == 0) { // Var check
                        for (int j = 0; j < 1024; j++) {
                            if (strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0) {
                                if (!def_vars[j].isNull) {
                                    printf("_EQUATION_ VAR %s FIND. VALUE IS %s\n", def_vars[j].name,
                                           def_vars[j].value);
                                    var_find_eq = true;
                                    found_val_or_var = true;
                                    var_action = false;
                                    continue;
                                } else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                            }
                        }
                        if (!var_find_eq) {
                            printf("Error. unexpected token '%s'. Var is undefined\n",
                                   words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    } else if (strcmp(table_number, "2") == 0) { // Var value check
                        found_val_or_var = true;
                        printf("_FIND_ VALUE IS %s\n", words[atoi(table_number)][atoi(table_val)]);
                        var_action = false;
                        continue;
                    }

                    if (strcmp(table_number, "1") == 0 && strcmp(table_val, "7") != 0 && strcmp(table_val, "8") != 0 &&
                        !found_val_or_var) {
                        printf("Error. Value or variable not found\n");
                        not_error = false;
                        break;
                    }
                    if (strcmp(table_number, "1") == 0) {
                        val_find_local_tmp = false;
                        var_find_local_tmp = false;
                        found_val_or_var = false;
                        if (strcmp(table_val, "10") == 0) {
                            var_action = true;
                            printf("IS MINUS\n");
                        } else if (strcmp(table_val, "12") == 0) {
                            var_action = true;
                            printf("IS PLUS\n");
                        } else if (strcmp(table_val, "13") == 0) {
                            var_action = true;
                            printf("IS MULTIPLY\n");
                        } else if (strcmp(table_val, "7") == 0) {
                            var_action = true;
                            printf("IS (\n");
                        } else if (strcmp(table_val, "8") == 0) {
                            var_action = true;
                            printf("IS )\n");
                        } else if (strcmp(table_val, "14") == 0) {
                            var_action = true;
                            printf("IS DIV\n");
                            if (strcmp(type_for_check, "int") == 0 || strcmp(type_for_check, "bool") == 0) {
                                printf("Error. Find int in DIV operation.\n");
                                not_error = false;
                                break;
                            }
                        } else if (var_action) {
                            printf("Error. Variable not found.\n");
                            not_error = false;
                            break;
                        } else {
                            syntax_lvl = 3;
                            var_action = false;
                            continue;
                        }
                    } else {
                        syntax_lvl = 3;
                        var_action = false;
                        val_find_local_tmp, var_find_local_tmp = false;
                        if (in_function) {
                            syntax_lvl = 10;
                            continue;
                        }
                        continue;
                    }
                    syntax_lvl = 6;

                }
                if (syntax_lvl == 7) { // IF
                    table_val = delimited_str[1];
                    if (strcmp(table_number, "3") == 0) {
                        for (int j = 0; j < 1024; j++) {
                            if (strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0) {
                                if (!def_vars[j].isNull) {
                                    printf("VAR IN IF %s FIND. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                    var_find_if = true;
                                    var_if_number = j;
                                } else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                            }
                            if (var_find_if) {
                                break;
                            }
                        }
                        if (!var_find_if) {
                            printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    }
                    if (strcmp(table_number, "1") == 0) {
                        if (strcmp(table_val, "2") == 0) { // condition in if
                            printf("FIND '>'\n");
                            has_condition_if = true;
                            if (var_find_if) {
                                var_find_if = false;
                            } else {
                                printf("Error. Undefined token, value not found %s\n",
                                       words[atoi(table_number)][atoi(table_val)]);
                                not_error = false;
                                break;
                            }
                        } else if (strcmp(table_val, "4") == 0) {
                            printf("FIND '<'\n");
                            has_condition_if = true;
                            var_find_if = false;
                            if (var_find_if) {
                                var_find_if = false;
                            } else {
                                printf("Error. Undefined token, value not found %s\n",
                                       words[atoi(table_number)][atoi(table_val)]);
                                not_error = false;
                                break;
                            }
                        } else if (strcmp(table_val, "15") == 0 && has_condition_if && var_find_if) {
                            printf("FIND 'Then'. IF STARTED.\n");
                            syntax_lvl = 3;
                            then_find = true;
                            var_find_if = false;
                            has_condition_if = false;
                        } else if (!var_find_if && has_condition_if) {
                            printf("Error. Undefined token, value not found %s\n",
                                   words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        } else if (!has_condition_if && var_find_if) {
                            printf("FIND 'Then' but condition not found! Checking var value from var '%s'\n",
                                   def_vars[var_if_number].name);
                            then_find = true;
                            var_find_if = false;
                            has_condition_if = false;
                            syntax_lvl = 3;
                            continue;
                        } else {
                            syntax_lvl = 3;
                        }
                        continue;
                    }
                }

                if (syntax_lvl == 8) { // CYCLES
                    table_val = delimited_str[1];
                    if (strcmp(table_number, "3") == 0) {
                        for (int j = 0; j < 1024; j++) {
                            if (strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0) {
                                if (!def_vars[j].isNull) {
                                    printf("VAR IN CYCLE %s FIND. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                    var_find_cycle = true;
                                } else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                            }
                            if (var_find_cycle) {
                                break;
                            }
                        }
                        if (!var_find_cycle) {
                            printf("VAR NOT FOUND '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            strcpy(def_vars[vars_count].name, words[atoi(table_number)][atoi(table_val)]);
                            syntax_lvl = 9;
                        }
                    } else if (strcmp(table_number, "1") == 0 && strcmp(table_val, "19") == 0 && has_equaling) {
                        printf("WORD VAL FOUND\n");
                        has_val = true;
                        continue;
                    } else if (strcmp(table_number, "2") == 0 && has_val) {
                        printf("VAL '%s' FOUND.\n", words[atoi(table_number)][atoi(table_val)]);
                        has_condition_cycle = true;
                        has_value_cycle = true;
                        continue;
                    } else if (strcmp(table_val, "2") == 0 && is_while &&
                               (has_value_cycle || var_find_cycle)) { // condition in while
                        printf("FIND '>'\n");
                        has_condition_cycle = true;
                        has_value_cycle = false;
                        var_find_cycle = false;
                        continue;
                    } else if (strcmp(table_val, "4") == 0 && is_while && (has_value_cycle || var_find_cycle)) {
                        printf("FIND '<'\n");
                        has_condition_cycle = true;
                        has_value_cycle = false;
                        var_find_cycle = false;
                        continue;
                    } else if (strcmp(table_number, "1") == 0 && strcmp(table_val, "20") == 0 && has_condition_cycle) {
                        printf("DO FOUND\n");
                        if (!var_find_cycle && !has_value_cycle && is_while) {
                            printf("Error. Var in condition cycle not find\n");
                            not_error = false;
                            break;
                        }
                        has_val = false;
                        var_find_cycle = false;
                        has_value_cycle = false;
                        syntax_lvl = 3;
                        has_condition_cycle = false;
                    } else {
                        printf("Error. required token not find. Find '%s'.\n",
                               words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                    continue;
                }

                if (syntax_lvl == 9) { // do assign for value in "for"
                    if ((strcmp(table_number, "1") == 0) && (strcmp(table_val, "0") == 0)) {
                        printf("ASSIGN FIND IN FOR\n");
                        continue;
                    } else if ((strcmp(table_number, "2") == 0)) {
                        char *tmp_value_in_for = number_validation(words[atoi(table_number)][atoi(table_val)]);
                        printf("VALUE FOUND '%s'.\n", tmp_value_in_for);
                        strcpy(def_vars[vars_count].value, tmp_value_in_for);
//                        free(tmp_value_in_for);
                        vars_count++;
                        has_equaling = true;
                        var_find_cycle = true;
                        syntax_lvl = 8;
                        continue;
                    } else if ((strcmp(table_number, "3") == 0)) {
                        bool var_find = false;
                        for (int j = 0; j < 1024; j++) {
                            if (strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0) {
                                if (!def_vars[j].isNull) {
                                    printf("VAR %s FIND IN NEW VALUE ADDING. VALUE IS %s\n", def_vars[j].name,
                                           def_vars[j].value);
                                    var_find_cycle = true;
                                } else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                                strcpy(def_vars[vars_count].value, def_vars[j].value);
                                printf("VAR %s IN FOR HAS NEW VAL IS %s\n", def_vars[vars_count].name,
                                       def_vars[vars_count].value);
                                var_find = true;
                                break;
                            }
                        }
                        if (var_find) {
                            vars_count++;
                            has_equaling = true;
                            syntax_lvl = 8;
                        } else {
                            printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                        continue;
                    } else {
                        printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                }
                if (syntax_lvl == 10) { // Function check
                    if (strcmp(table_number, "1") == 0 && strcmp(table_val, "31") == 0) {
                        syntax_lvl = 6;
                        printf("Find ',' function is continue\n");
                        continue;
                    } else if (strcmp(table_val, "1") == 0 && strcmp(table_number, "1") == 0) {
                        if (in_function) {
                            in_function = false;
                            printf("Function is ended\n");
                            syntax_lvl = 3;
                        }
                    } else {
                        syntax_lvl = 6;
                    }
                }
            }
        }
    }
    if (is_cycle) {
        printf("Error. 'Next' after cycle not found.\n");
        not_error = false;
    }
    if (is_if) {
        printf("Error. 'End' after if not found.\n");
        not_error = false;
    }
    if (!not_error) {
        printf("Error. On %d line.\n", line);
    }
    return not_error;
}

bool code_check_file_write(const char chars[1024]) {
    char tmp[100];
    char tmp_type[100];
    char tmp_str[1024];
    memset(map, '\0', sizeof(map));
    memset(tmp, '\0', sizeof(tmp));
    memset(tmp_type, '\0', sizeof(tmp));
    int j = 0;
    int type_index = 0;

    bool delimiter_detected = false;
    bool is_del = false;
    bool type_declaration = true;
    int tmp_del = 0;

    bool we_have_problem = false;

    for (int i = 0; *(chars + i); i++) {
        if (we_have_problem)
            break;

        for (int del = 0; del <= 34; del++) {

            for (int sub_del = 0; *(words[1][del] + sub_del); sub_del++) {

                if (chars[i] == words[1][del][0]) {
                    for (int sub_del_chk = 0; *(words[1][del] + sub_del_chk); sub_del_chk++) {
                        if (chars[i + sub_del_chk] == words[1][del][sub_del_chk]) {
                            is_del = true;
                        } else {
                            is_del = false;
                            break;
                        }
                    }
                }

                if (chars[i] == words[1][del][sub_del] && is_del) {
                    if (words[1][del][sub_del + 1] == '\0' && !delimiter_detected) {
                        delimiter_detected = true;
                        i++;
                        tmp_del = del;
                        del = 0;
                    } else {
                        delimiter_detected = false;
                        i++;
                    }
                } else if (chars[i] != words[1][del][sub_del] && is_del) {
                    i = i - (int) strlen(words[1][del]) + 1;
                    is_del = false;
                }

                if (delimiter_detected) {
                    while (true) {
                        if (strcmp("", tmp_type) == 0) {
                        }


                        if (!type_declaration && (strcmp("", tmp_type) != 0)) {
                            for (int kk = 0; kk < 1024; kk++) {
                                if (strcmp(words[0][kk], tmp_type) == 0) {
                                    printf("[%d][%d] Type: '%s' | tmp: %s \n", 0, kk, words[0][kk], tmp_type);

                                    sprintf(tmp_str, file_view, 0, kk);
                                    strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);
                                    memset(tmp_str, '\0', sizeof(tmp_str));
                                }
                            }
                        }
                        if (strcmp("", tmp) == 0) {
                            break;
                        }
                        char first_tmp[] = {tmp[0]};
                        if (isNumber(first_tmp) || tmp[0] == '.' || strcmp(tmp, "true") == 0 ||
                            strcmp(tmp, "false") == 0) {

                            if (strcmp(tmp, "true") != 0 && strcmp(tmp, "false") != 0) {

                                // TEMPORARY

//                                char *tmp_verification;
//                                tmp_verification = number_validation(tmp);
//                                if (strcmp(tmp_verification, "false_verification") == 0) {
//                                    we_have_problem = true;
//                                    break;
//                                }
//                                printf("[%d][%d] Value: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);
//                                strcpy(tmp, tmp_verification);

// //                               free(tmp_verification);
                                printf("[%d][%d] Binary value is: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);
                            }
                            printf("[%d][%d] Value: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);
                            strcpy(words[2][vars], tmp);
                            sprintf(tmp_str, file_view, 2, vars);
                            strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);

                            memset(tmp_str, '\0', sizeof(tmp_str));

                            vars++;
                        } else {
                            strcpy(words[3][vars], tmp);
                            printf("[%d][%d] Var: '%s' | tmp: %s \n", 3, vars, words[3][vars], tmp);

                            sprintf(tmp_str, file_view, 3, vars);
                            strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);
                            vars++;

                            memset(tmp_str, '\0', sizeof(tmp_str));
                        }

                        break;
                    }
                    j = 0;
                    printf("[%d][%d] Delimiter: '%s' | tmp: %s \n", 1, tmp_del, words[1][tmp_del], tmp);

                    sprintf(tmp_str, file_view, 1, tmp_del);
                    strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);

                    memset(tmp_str, '\0', sizeof(tmp_str));

                    memset(tmp, '\0', sizeof(tmp));
                    delimiter_detected = false;
                    is_del = false;
                    type_declaration = true;
                    break;
                }
            }
        }
        if (type_declaration) {
            for (int type = 0; type < 4; type++) {
                int size_type = 0;
                if (chars[i] == words[0][type][0]) {
                    type_index = 0;
                    memset(tmp_type, '\0', sizeof(tmp_type));

                    int type_size = (int) strlen(words[0][type]);
                    for (int sub_del_chk = 0; *(words[0][type] + sub_del_chk); sub_del_chk++) {
                        if (chars[i + sub_del_chk] == words[0][type][sub_del_chk]) {
                            tmp_type[type_index] = chars[i + sub_del_chk];
                            size_type = sub_del_chk + 1;
                            type_index++;
                        } else {
                            if (type_size > type_index) {
                                memset(tmp_type, '\0', sizeof(tmp_type));
                                type_index = 0;
                                i -= sub_del_chk;
                            }
                            type_index = 0;
                            break;
                        }
                    }
                    type_declaration = false;
                    i += size_type;
                }
            }
        }

        tmp[j] = chars[i];

        if (tmp[0] == words[1][1][0] && !type_declaration) {
            tmp[0] = '\0';
            for (int k = 1; *(tmp + k); k++) {
                tmp[k] = tmp[k];
            }
            i--;
        }
        j++;

    }
    if (!we_have_problem) {
        file_write("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt", map);
        strcat(map_2, map);
//        bool syntax_next = syntax_check(map);
//        if (syntax_next) {
//            code_work();
//        }
    }
    return !we_have_problem;
}


int main() {

    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen("C:\\Users\\rain\\CLionProjects\\CTest\\code2", "rb");
    if (!fp) perror("C:\\Users\\rain\\CLionProjects\\CTest\\code"), exit(1);

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = calloc(1, lSize + 1);
    if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

    if (1 != fread(buffer, lSize, 1, fp))
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

    fclose(fp);

    removeSpacesAndNewlines(buffer);
    fclose(fopen("tmp", "w"));
    file_write("tmp", buffer);
    free(buffer);
    FILE *file_ptr;
    char ch;
    file_ptr = fopen("tmp", "r");
    if (NULL == file_ptr) {
        printf("file can't be opened \n");
        return EXIT_FAILURE;
    }
    int i = 0;
    bool result = false;
    char tmp_buffer[1024];
    memset(tmp_buffer, '\0', sizeof(tmp_buffer));
    fclose(fopen("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt", "w"));

    printf("______________________________\n");
    printf("PROGRAM LEXEME ANALYSE STARTED\n");
    printf("______________________________\n");

    while ((ch = fgetc(file_ptr)) != EOF) {
        tmp_buffer[i] = ch;
        i++;
        if (ch == ';') {
            result = code_check_file_write(tmp_buffer);
            if(!result)
                break;
            memset(tmp_buffer, '\0', sizeof(tmp_buffer));
            i = 0;
        }
    }

    if ((ch = fgetc(file_ptr)) == EOF) {
        result = code_check_file_write(tmp_buffer);
        memset(tmp_buffer, '\0', sizeof(tmp_buffer));
        i = 0;
    }

    if (result) {

        FILE *tmp;
        char ch_tmp;
        file_ptr = fopen("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt", "r");
        if (NULL == file_ptr) {
            printf("file can't be opened \n");
            return EXIT_FAILURE;
        }

        char tmp_buffer_file[1024 * 1024];
        memset(tmp_buffer_file, '\0', sizeof(tmp_buffer_file));

        while ((ch = fgetc(file_ptr)) != EOF) {
            tmp_buffer_file[i] = ch;
            i++;
        }

        bool syntax_next = syntax_check(tmp_buffer_file);
        if (syntax_next) {
            code_work();
        }

    }
    return 0;
}
