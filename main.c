#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#define FP2BIN_STRING_MAX 1077
#define OBJ_FILE_NAME "myobj.o"
#define TEMP_OBJ_FILE_NAME "tmp_myobj"
#define TEMP_OBJ_FILE_NAME_END "..\\tmp_myobj_end"
#define TEMP_OBJ_FILE_NAME_DATA "tmp_myobj_data"
#define TEMP_OBJ_FILE_RELOCATIONS "tmp_myobj_relocations"
#define TEMP_OBJ_FILE_SYMBOLS "tmp_myobj_symbols"

#define MOV_VAR_RAX "mov_var_rax"
#define MOV_VAR_RAX "mov_var_rax"



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
                                     {"!"}, // 10 "-"
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

double ieee754ToDouble(uint64_t bits) {
    uint64_t sign = (bits >> 63) & 0x1;
    uint64_t exponent = (bits >> 52) & 0x7FF;
    uint64_t mantissa = bits & 0xFFFFFFFFFFFFF;

    // Вычисление фактической экспоненты
    int64_t actual_exponent = exponent - 1023;

    // Вычисление мантиссы
    double mantissa_value = 1.0; // Имплицитная единица
    for (int i = 0; i < 52; i++) {
        if (mantissa & (1ULL << (51 - i))) {
            mantissa_value += (1.0 / (1ULL << (i + 1)));
        }
    }

    // Вычисление окончательного значения
    double result = (sign ? -1 : 1) * mantissa_value * (1ULL << actual_exponent);
    return result;
}

typedef union {
    double value;
    uint64_t bits;
} DoubleUnion;

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
            printf("Plus or Minus find...Exponential read.\n");
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
            if ((value[i] == '.' || !isdigit(value[i])) && value[i] != 'E' && value[i] != '+' && value[i] != '-') {
                printf("Error. Find not digit value after exp\n");
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
        long long answer_int = 0;

        char last_char = value[i];

        if (last_char == 'H' || last_char == 'h') {
            printf("Digit is hex \n");
            int hex_digit = (int)strtol(tmp_value, NULL, 16);
            sprintf(tmp_value, "%d", hex_digit);
            answer = tmp_value;
        } else if (last_char == 'B' || last_char == 'b') {
            printf("Digit is binary \n");
        } else if (last_char == 'O' || last_char == 'o') {
            printf("Digit is oct \n");
//            answer = octToBinary(tmp_value);

            int oct_digit = 1;
            int octal = atoi(tmp_value);
            int temp = octal;
            int decimalvalue = 0;
            while (temp)
            {
                // Finding the last digit
                int lastdigit = temp % 10;
                temp = temp / 10;

                // Multiplying last digit with appropriate
                // base value and adding it to decimalvalue
                decimalvalue += lastdigit * oct_digit;

                oct_digit = oct_digit * 8;
            }
            sprintf(tmp_value, "%d", decimalvalue);
            answer = tmp_value;
        } else if (last_char == '.') {
            printf("Error. Real format number not valid\n");
            valid = false;
        } else {
            char *endptr_l;

            if (exponential_write_exp) {
                if(is_hex){
                    answer_double = atof(tmp_value);
                    snprintf(tmp_value, 10, "%f", answer_double);
                }else{
//                    answer_int = atoi(tmp_value);
                    answer_double = atof(tmp_value);
                    DoubleUnion du;
                    du.value = answer_double;

                    char test_value[19];
                    memset(test_value, '\0', 19);

                    sprintf(test_value, "%f", du.value);
                    snprintf(tmp_value, 1024, "%llu", du.value);
                    bool has_dot = false;
                    bool is_int = true;
                    for(int j = 0; j < 19; j++){
                        if(has_dot){
                            if(test_value[j] != '0'){
                                is_int = false;
                                break;
                            }
                        }
                        if(test_value[j] == '.'){
                            has_dot = true;
                        }
                    }
                    if(is_int){
                        answer_int = (long long)(answer_double);
                        snprintf(tmp_value, 1024, "%lld", answer_int);
                    }
//                    snprintf(tmp_value, 10, "%f", answer_double);
                }
            } else if (is_real) {

                answer_float = strtof(tmp_value, &endptr_l);
                DoubleUnion du;
                du.value = answer_float;
                snprintf(tmp_value, 1024, "%llu", du.value);

                printf("Double: %lf\n", du.value);
                printf("Bits: %llu\n", du.bits);
            } else {
                answer_int = atoi(tmp_value);
                snprintf(tmp_value, 1024, "%d", answer_int);
            }

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

const char *check_type_data(char *value) {
    const char *type_data = "int";
    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        type_data = "bool";
        return type_data;

    }
    double test = 0.0;
    if(strlen(value)>=19){
        char *endptr;
        test  = ieee754ToDouble(strtoull(value, &endptr, 10));
        char test_value[19];
        memset(test_value, '\0', 19);

        sprintf(test_value, "%f", test);
        bool has_dot = false;
        printf("Double detected: %f\n", test);
        for(int i = 0; i < 19; i++){
            if(has_dot){
                if(test_value[i] != '0'){
                    return "float";
                }
            }
            if(test_value[i] == '.'){
                has_dot = true;
            }
        }
        return "int";
    }

    if (strcmp(value, "true") == 0 || strcmp(value, "false") == 0) {
        return "bool";
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
        case '!':
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

void append_bytes_from_file(const char *source_file_path, const char *destination_file_path) {
    // Открываем исходный файл для чтения
    FILE *source_file = fopen(source_file_path, "rb");
    if (source_file == NULL) {
        perror("err append_bytes_from_file");
        return;
    }

    // Открываем файл назначения для добавления
    FILE *destination_file = fopen(destination_file_path, "ab");
    if (destination_file == NULL) {
        perror("err append_bytes_from_file");
        fclose(source_file);
        return;
    }

    // Чтение и запись байтов
    unsigned char buffer[1024]; // Буфер для чтения данных
    size_t bytes_read;

    while ((bytes_read = fread(buffer, sizeof(unsigned char), sizeof(buffer), source_file)) > 0) {
        fwrite(buffer, sizeof(unsigned char), bytes_read, destination_file);
    }

    // Закрываем файлы
    fclose(source_file);
    fclose(destination_file);
}

void appendToFile(const char *filename, const void *data, size_t dataSize) {
    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        perror("err appendToFile\n");
        return;
    }
//    printf("%x writed to file %s \n", *(const int *)data, filename);
    size_t writtenBytes = fwrite(data, 1, dataSize, file);
    if (writtenBytes != dataSize) {
        perror("err appendToFile\n");
    }

    fclose(file);
}
void appendToFile_little_en(const char *filename, const void *data, size_t size) {
    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
//    printf("%x writed to file %s \n", *(const int *)data, filename);

    fwrite(data, sizeof(uint8_t), size, file);
    fclose(file);
}

long getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}


struct hex_values{
    char name[1024];
    unsigned char addr;
    long long value;
};

typedef struct IMAGE_FILE_HEADER {
    uint32_t Machine;
    uint32_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint32_t SizeOfOptionalHeader;
    uint32_t Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct  {
    uint32_t physical_address;        // 4 байта
    uint32_t virtual_address;         // 4 байта
    uint32_t size_of_raw_data;       // 4 байта
    uint32_t file_pointer_to_raw_data; // 4 байта
    uint32_t file_pointer_to_relocation_table; // 4 байта
    uint32_t file_pointer_to_line_numbers; // 4 байта
    uint16_t number_of_relocations;   // 2 байта
    uint16_t number_of_line_numbers;  // 2 байта
    uint32_t flags;                   // 4 байта
}COFFHeader;

typedef struct {
    uint32_t r_offset;
    uint32_t r_symbol;
    uint16_t r_type;
} RelocationEntry;

typedef struct {
    union {
        char   s_name[8];
        uint32_t s_strx;
    };
    uint32_t s_value;
    uint16_t s_section;
    uint16_t s_type;
    uint8_t  s_storage_class;
    uint8_t  s_num_aux;
} SymbolEntry;

struct hex_values hex_values_l[1024];



void correct_hex_presentation(char *digit, char *filename) {
    long long decimalNumber = atoll(digit);

    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }

    unsigned char hex_bytes[4];
    for (int i = 0; i < 4; i++) {
        hex_bytes[i] = (decimalNumber >> (i * 8)) & 0xFF;
    }

    // Запись байтов в файл
    fwrite(hex_bytes, sizeof(unsigned char), sizeof(hex_bytes), file);

    fclose(file);
}

void add_relocations_section(RelocationEntry relocationEntry){
    appendToFile_little_en(TEMP_OBJ_FILE_RELOCATIONS, &relocationEntry.r_offset, 4);
    appendToFile_little_en(TEMP_OBJ_FILE_RELOCATIONS, &relocationEntry.r_symbol, 4);
    appendToFile_little_en(TEMP_OBJ_FILE_RELOCATIONS, &relocationEntry.r_type, 2);
}
int machine_templates(char *op, char *value, int index_vars, int relocation_count){

    RelocationEntry relocationEntry;

    int hex = 0x0;
    if(strcmp(op, "mov_var_rax_legacy") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xc7;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xc0;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "mov_rax_addr") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x89;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x05;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){
                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }

    }else if(strcmp(op, "mov_rbx_addr") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x89;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x1d;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){
                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);


                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }

    }else if(strcmp(op, "mov_rdx_addr") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x89;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x15;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){
                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
    }else if(strcmp(op, "mov_addr_rax") == 0){
        bool found = false;
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x8b;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x05;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
//                long code_count = (getFileSize(TEMP_OBJ_FILE_NAME)/16)*10;

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);
                found = true;

//                relocationEntry.r_offset = hex_values_l[i].addr + (code_count_sub - code_count - 2) + code_count;
                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                relocation_count++;
                add_relocations_section(relocationEntry);
                break;
            }
        }
        if(!found){

            hex = 0x48;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC7;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC0;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

            correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
        }
    }else if(strcmp(op, "mov_addr_rbx") == 0){

        bool found = false;
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x8B;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x1D;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);
                found = true;

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
        if(!found){

            hex = 0x48;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC7;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC3;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
//            add_relocations_section(relocationEntry);

            correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
        }
    }else if(strcmp(op, "mov_addr_rcx") == 0){

        bool found = false;
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x8B;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x0D;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);
                found = true;

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
        if(!found){

            hex = 0x48;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC7;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC1;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
//            add_relocations_section(relocationEntry);

            correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
        }
    }else if(strcmp(op, "mov_addr_rdx") == 0){

        bool found = false;
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x8B;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x15;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);
                found = true;

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
        if(!found){

            hex = 0x48;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC7;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC3;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            add_relocations_section(relocationEntry);

            correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
        }
    }else if(strcmp(op, "mov_addr_rcx") == 0){

        bool found = false;
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x8B;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x15;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);
                found = true;

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
        if(!found){

            hex = 0x48;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC7;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC1;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
//            add_relocations_section(relocationEntry);

            correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
        }
    }else if(strcmp(op, "add_addr_rbx") == 0){

        bool found = false;
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x03;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x03;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);
                found = true;

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
        if(!found){

            hex = 0x48;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0x81;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xC3;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

            correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
        }
    }else if(strcmp(op, "add_rbx_addr") == 0){
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x01;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x1D;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
    }else if(strcmp(op, "lea_addr") == 0){

//        bool found = false;
//        for(int i = 0; i < index_vars; i++){
//            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x8d;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x05;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                int count_data_set = (index_vars * 8);

                sprintf(adr, "%d", count_data_set);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

    }else if(strcmp(op, "add_rbx_rax") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x01;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xD8;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "add_rbx_rdx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x01;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xDA;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "add_rax_rcx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x01;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xC1;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "add_rcx_rbx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x01;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xCB;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "mov_rbx_rax") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x89;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xD8;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "mov_rax_rbx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x89;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xC3;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "mov_rbx_rcx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x89;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xD9;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "add_rax_rbx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x01;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xC3;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "sub_rax_rbx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x29;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xC3;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "sub_rax_rcx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x29;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xC1;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "sub_rcx_rax") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x29;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xC8;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "mul_rax") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xF7;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xE0;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "mul_rcx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xF7;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xE1;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "div_rax") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xF7;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xF0;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "div_rcx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xF7;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xF1;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "jmp_addr") == 0){
        hex = 0xEB;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

        correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
    }else if(strcmp(op, "cmp_rax_rbx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x39;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "cmp_addr_rcx") == 0){

        bool found = false;
        for(int i = 0; i < index_vars; i++){
            if(strcmp(hex_values_l[i].name, value) == 0){

                hex = 0x48;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x3B;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
                hex = 0x0D;
                appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);

                char adr[100];
                memset(adr, '\0', 100);
                sprintf(adr, "%d", hex_values_l[i].addr);
                long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
                correct_hex_presentation(adr, TEMP_OBJ_FILE_NAME);
                found = true;

                relocationEntry.r_offset = code_count;
                relocationEntry.r_symbol = index_vars + 6;
                relocationEntry.r_type = 0x0004;
                add_relocations_section(relocationEntry);
                relocation_count++;

                break;
            }
        }
        if(!found){

            hex = 0x48;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0x83;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            hex = 0xF9;
            appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
            add_relocations_section(relocationEntry);

            correct_hex_presentation(value, TEMP_OBJ_FILE_NAME);
        }
    }else if(strcmp(op, "mov_rax_rcx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x89;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xC1;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "xor_rdx_rdx") == 0){
        hex = 0x48;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x31;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0xD2;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
    }else if(strcmp(op, "print") == 0){
        hex = 0xE8;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        long code_count = getFileSize(TEMP_OBJ_FILE_NAME);
        hex = 0x00;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x00;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x00;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        hex = 0x00;
        appendToFile(TEMP_OBJ_FILE_NAME, &hex, 1);
        relocationEntry.r_offset = code_count;
        relocationEntry.r_symbol = index_vars + 11;
        relocationEntry.r_type = 0x0004;
        add_relocations_section(relocationEntry);
        relocation_count++;
    }
    printf("COMMAND %s, VALUE/ADDR: %s\n", op, value);
    return relocation_count;
}


int block_count = 0;


struct IMAGE_FILE_HEADER fileHeader;
COFFHeader text;
COFFHeader data;
COFFHeader bss;

void add_coff_header(){
    appendToFile(OBJ_FILE_NAME, &fileHeader.Machine, 2);
    appendToFile(OBJ_FILE_NAME, &fileHeader.NumberOfSections, 2);
    appendToFile(OBJ_FILE_NAME, &fileHeader.TimeDateStamp, 4);
    appendToFile(OBJ_FILE_NAME, &fileHeader.PointerToSymbolTable, 4);
    appendToFile(OBJ_FILE_NAME, &fileHeader.NumberOfSymbols, 4);
    appendToFile(OBJ_FILE_NAME, &fileHeader.SizeOfOptionalHeader, 2);
    appendToFile(OBJ_FILE_NAME, &fileHeader.Characteristics, 2);
}
void add_section_header(COFFHeader header){
    appendToFile_little_en(OBJ_FILE_NAME, &header.physical_address, 4);
    appendToFile_little_en(OBJ_FILE_NAME, &header.virtual_address, 4);
    appendToFile_little_en(OBJ_FILE_NAME, &header.size_of_raw_data, 4);
    appendToFile_little_en(OBJ_FILE_NAME, &header.file_pointer_to_raw_data, 4);
    appendToFile_little_en(OBJ_FILE_NAME, &header.file_pointer_to_relocation_table, 4);
    appendToFile_little_en(OBJ_FILE_NAME, &header.file_pointer_to_line_numbers, 4);
    appendToFile_little_en(OBJ_FILE_NAME, &header.number_of_relocations, 2);
    appendToFile_little_en(OBJ_FILE_NAME, &header.number_of_line_numbers, 2);
    appendToFile_little_en(OBJ_FILE_NAME, &header.flags, 4);
}

void add_data_section(struct hex_values hex){
    appendToFile_little_en(TEMP_OBJ_FILE_NAME_DATA, &hex.value, 8);
}

void add_symbol_table(SymbolEntry symbolEntry) {

    appendToFile_little_en(TEMP_OBJ_FILE_SYMBOLS, &symbolEntry.s_name, strlen(symbolEntry.s_name));
    unsigned long long size_str = strlen(symbolEntry.s_name);
    uint32_t null_hex = 0x00;
    while (size_str < 8){
        size_str++;
        appendToFile_little_en(TEMP_OBJ_FILE_SYMBOLS, &null_hex, 1);
    }
    appendToFile_little_en(TEMP_OBJ_FILE_SYMBOLS, &symbolEntry.s_value, 4);
    appendToFile_little_en(TEMP_OBJ_FILE_SYMBOLS, &symbolEntry.s_section, 2);
    appendToFile_little_en(TEMP_OBJ_FILE_SYMBOLS, &symbolEntry.s_type, 2);
    appendToFile_little_en(TEMP_OBJ_FILE_SYMBOLS, &symbolEntry.s_storage_class, 1);
    appendToFile_little_en(TEMP_OBJ_FILE_SYMBOLS, &symbolEntry.s_num_aux, 1);
}




void competition(char *path, uint32_t data_rem){
    long size = getFileSize(path);

    while((size % 16)!= 0){
        appendToFile_little_en(path, &data_rem, 1);
        size = getFileSize(path);
    }
}


bool code_work() {

    int raw_data = 0x0;
    int byte_count = 0;
    char result[4068];
    memset(result, '\0', 4068);

    fclose(fopen(OBJ_FILE_NAME, "w"));
    fclose(fopen(TEMP_OBJ_FILE_NAME, "w"));
    fclose(fopen(TEMP_OBJ_FILE_SYMBOLS, "w"));
    fclose(fopen(TEMP_OBJ_FILE_NAME_DATA, "w"));
    fclose(fopen(TEMP_OBJ_FILE_RELOCATIONS, "w"));

    char template[200];
    int relocation_count = 0;
    memset(template, '\0', sizeof(template));
    char hexval[1024];

    memset(hexval, '\0', 1024);

//    copyFile("C:\\Users\\rain\\CLionProjects\\CTest\\template.bin", OBJ_FILE_NAME);
    unsigned char hex_local = 0x25;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x66;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x0A;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x55;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x48;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x89;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0xE5;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x48;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x83;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0xEC;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    hex_local = 0x30;
    appendToFile(TEMP_OBJ_FILE_NAME, &hex_local, 1);
    byte_count+=11;
    // init variables
    int index_vars = 0;
    int max_hex_addr = 0;
    printf("adress | value | name \n");
    for (int i = 0; i < vars_count; i++) {
            if (strcmp(def_vars[i].value, "true") == 0) {

                strcpy(hex_values_l[index_vars].name, def_vars[i].name);
                hex_values_l[index_vars].addr = max_hex_addr;
                hex_values_l[index_vars].value = 1;
                max_hex_addr+=8;
//                printf("%x | %s | %s\n", hex_values_l[index_vars].addr, hex_values_l[index_vars].value, hex_values_l[index_vars].name);
                index_vars++;
            } else if (strcmp(def_vars[i].value, "false") == 0) {

                strcpy(hex_values_l[index_vars].name, def_vars[i].name);
                hex_values_l[index_vars].addr = max_hex_addr;
                hex_values_l[index_vars].value = 0;
                max_hex_addr+=8;
//                printf("%x | %d | %s\n", hex_values_l[index_vars].addr, hex_values_l[index_vars].value, hex_values_l[index_vars].name);
                index_vars++;
            } else {
                if(strcmp(def_vars[i].value, "")!=0) {
                    sprintf(hexval, "%04x", atoi(def_vars[i].value));

                    char *endPtr;
                    long long decimalNumber = atoll(def_vars[i].value);

                    size_t numBytes = sizeof(decimalNumber);

                    char hex_tmp[1024];
                    memset(hex_tmp, '\0', 1024);
                    sprintf(hex_tmp, "%llx", decimalNumber);

                    strcpy(hex_values_l[index_vars].name, def_vars[i].name);
                    hex_values_l[index_vars].addr = max_hex_addr;
                    hex_values_l[index_vars].value = decimalNumber;
                    max_hex_addr+=8;
//                    printf("%x | %d | %s\n", hex_values_l[index_vars].addr, hex_values_l[index_vars].value, hex_values_l[index_vars].name);
                    index_vars++;
                }else{

                    strcpy(hex_values_l[index_vars].name, def_vars[i].name);
                    hex_values_l[index_vars].addr = max_hex_addr;
                    hex_values_l[index_vars].value = 0;
                    max_hex_addr+=8;
//                    printf("%x | %d | %s\n", hex_values_l[index_vars].addr, hex_values_l[index_vars].value, hex_values_l[index_vars].name);
                    index_vars++;
                }
            }
    }
    strcat(result, ".text\n"
                   ".globl\tmain\n"
                   ".extern printf\n");
    strcat(result, ".data\n");

    // init variables
    for (int i = 0; i < vars_count; i++) {

        if(strcmp(def_vars[i].value, "true") == 0){
            sprintf(template, "\t%s: .%s %s\n", def_vars[i].name, "quad", "1");
        }
        else if(strcmp(def_vars[i].value, "false") == 0){
            sprintf(template, "\t%s: .%s %s\n", def_vars[i].name, "quad", "0");
        }else{
            sprintf(template, "\t%s: .%s %s\n", def_vars[i].name, "quad", def_vars[i].value);
        }

        strcat(result, template);
        memset(template, '\0', sizeof(template));

    }

    // Init main function program
    strcat(result, ".LC0:\n"
                   "\t.ascii \"%lld\\n\"\n"
                   "\t.text\n"
                   "\t.globl main\n"
                   ".LC1:\n"
                   "\t.ascii \"%f\\n\"\n"
                   "\t.text\n"
                   "\t.globl main\n"
                   "main:\n"
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
    bool has_next = false;
    bool has_next_while = false;
    bool cycle_for = false;
    bool cycle_while = false;
    bool has_displ = false;
    char cycle_while_str[100];
    memset(cycle_while_str, '\0', sizeof(cycle_while_str));

    char tmp_block_name[20];
    char tmp_block_next_name[20];
    char format[5] = ".LC0";

    memset(tmp_block_name, '\0', sizeof(tmp_block_name));

    char stack[100];
    memset(stack, '\0', sizeof(stack));

    char infix[MAX], postfix[MAX];
    memset(postfix, '\0', sizeof(postfix));
    memset(infix, '\0', sizeof(infix));


    char variable[100];
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
//                                   ;                             val                            do
                if (strcmp(tokens, "1,1") == 0 || strcmp(tokens, "1,19") == 0 || strcmp(tokens, "1,20") == 0) {
                    has_displ = false;
                    is_condition_start = false;
                    infix[strcspn(infix, "\n")] = 0;
                    infixToPostfix(infix, postfix);
                    printf("Val:%s\n", postfix);
                    char stack_asm[100][100];
                    char s_a[10][1024];
                    memset(stack_asm, '\0', sizeof(stack_asm));

                    int index_stack = 0;
                    int v_index = 0;

                    memset(template, '\0', sizeof(template));
                    bool was_val = false;

                    bool has_condition = false;
                    for (int i = 0; *(postfix + i); ++i) {
                        if(postfix[i] == '|' || postfix[i] == '!' || postfix[i] == '*' || postfix[i] == '/'){
                            has_condition = true;
                        }
                    }
                    if(has_condition){
                        relocation_count = machine_templates("mov_addr_rbx", "0", 0, relocation_count);
                        for (int j = 0; *(postfix + j); j++) {
                            if (postfix[j] != '|' && postfix[j] != '!' && postfix[j] != '*' && postfix[j] != '/' && postfix[j] != ' ') {

                                stack_asm[index_stack][v_index] = postfix[j];
                                s_a[index_stack][v_index] = postfix[j];

                                was_val = true;
                                v_index++;
                            } else {
                                if(!was_val)
                                    index_stack--;
                                if (postfix[j] == '|') {
                                    if (strcmp(s_a[index_stack - 1], "") != 0 && strcmp(s_a[index_stack - 0], "") != 0) {

                                        relocation_count = machine_templates("mov_addr_rax", s_a[index_stack - 1], index_vars, relocation_count);
                                        relocation_count = machine_templates("mov_addr_rcx", s_a[index_stack], index_vars, relocation_count);

                                        relocation_count = machine_templates("add_rax_rcx", 0x00, index_vars, relocation_count);
                                        relocation_count = machine_templates("mov_addr_rax", "0", 0, relocation_count);

                                        memset(s_a[index_stack - 1], '\0', 1024);
                                        memset(s_a[index_stack - 0], '\0', 1024);
                                        index_stack = 0;

                                        relocation_count = machine_templates("add_rcx_rbx", 0x00, index_vars, relocation_count);
                                    } else {
                                        if (strcmp(s_a[index_stack], "") != 0) {
                                            relocation_count = machine_templates("add_addr_rbx", s_a[index_stack], index_vars, relocation_count);
                                            memset(s_a[index_stack - 0], '\0', 1024);
                                        }
                                    }

                                } else if (postfix[j] == '*') {

                                    if (strcmp(s_a[index_stack - 0], "") != 0 && strcmp(s_a[index_stack - 1], "") == 0) {
                                        relocation_count = machine_templates("mov_rbx_rax", 0x00, index_vars, relocation_count);
                                        relocation_count = machine_templates("mov_addr_rcx", s_a[index_stack - 0], index_vars, relocation_count);
                                        relocation_count = machine_templates("mul_rcx", 0x00, 0, relocation_count);
                                        relocation_count = machine_templates("mov_rax_rbx", 0x00, 0, relocation_count);
                                        relocation_count = machine_templates("mov_addr_rax", 0x00, 0, relocation_count);
                                        memset(s_a[index_stack - 0], '\0', 1024);
                                        memset(s_a[index_stack - 1], '\0', 1024);
                                    } else {
                                        if (strcmp(s_a[index_stack - 1], "") != 0) {
                                            relocation_count = machine_templates("mov_addr_rax", s_a[index_stack - 1], index_vars, relocation_count);
                                            memset(s_a[index_stack - 1], '\0', 1024);
                                        }
                                        if (strcmp(s_a[index_stack - 0], "") != 0) {
                                            relocation_count = machine_templates("mov_addr_rcx", s_a[index_stack - 0], index_vars, relocation_count);
                                            relocation_count = machine_templates("mul_rcx", 0x00, 0, relocation_count);
                                            relocation_count = machine_templates("mov_rax_rbx", 0x00, 0, relocation_count);
                                            relocation_count = machine_templates("mov_addr_rax", 0x00, 0, relocation_count);
                                            memset(s_a[index_stack - 0], '\0', 1024);
                                        }
                                        memset(s_a, '\0', 100);
                                        index_stack = 0;
                                    }

                                } else if (postfix[j] == '/') {
                                    if (strcmp(s_a[index_stack - 0], "") != 0 && strcmp(s_a[index_stack - 1], "") == 0) {
                                            relocation_count = machine_templates("mov_addr_rcx", s_a[index_stack - 0], index_vars, relocation_count);

                                            relocation_count = machine_templates("mov_rbx_rax", 0x00, index_vars, relocation_count);
                                            relocation_count = machine_templates("div_rcx", 0x00, index_vars, relocation_count);
                                            relocation_count = machine_templates("xor_rdx_rdx", 0x00, index_vars, relocation_count);
                                            relocation_count = machine_templates("mov_rax_rbx", 0x00, index_vars, relocation_count);
                                            relocation_count = machine_templates("mov_addr_rax", "0", index_vars, relocation_count);
                                            relocation_count = machine_templates("mov_addr_rcx", "0", index_vars, relocation_count);

                                    } else {
                                        if (strcmp(s_a[index_stack - 1], "") != 0) {
                                            relocation_count = machine_templates("mov_addr_rax", s_a[index_stack - 1], index_vars, relocation_count);
                                        }
                                        if (strcmp(s_a[index_stack - 0], "") != 0) {
                                            relocation_count = machine_templates("mov_addr_rcx", s_a[index_stack - 0], index_vars, relocation_count);
                                            relocation_count = machine_templates("xor_rdx_rdx", 0x00, index_vars, relocation_count);
                                            relocation_count = machine_templates("div_rcx", 0x00, index_vars, relocation_count);
                                            relocation_count = machine_templates("mov_rax_rbx", 0x00, 0, relocation_count);

                                        }
                                        memset(s_a[index_stack - 0], '\0', sizeof(s_a[index_stack - 0]));
                                        memset(s_a[index_stack - 1], '\0', sizeof(s_a[index_stack - 1]));
                                        index_stack = 0;
                                    }

                                    memset(s_a[index_stack - 0], '\0', sizeof(s_a[index_stack - 0]));
                                    memset(s_a[index_stack - 1], '\0', sizeof(s_a[index_stack - 1]));

                                } else if (postfix[j] == '!') {

                                    if (strcmp(s_a[index_stack - 0], "") != 0 && strcmp(s_a[index_stack - 1], "") == 0) {

                                        relocation_count = machine_templates("mov_addr_rax", s_a[index_stack - 0], index_vars, relocation_count);
                                        relocation_count = machine_templates("sub_rax_rbx", 0x00, index_vars, relocation_count);
                                        relocation_count = machine_templates("mov_addr_rax", "0", index_vars, relocation_count);

                                    } else {
                                        if (strcmp(s_a[index_stack - 1], "") != 0) {
                                            relocation_count = machine_templates("mov_addr_rax", s_a[index_stack - 1], index_vars, relocation_count);
                                        }
                                        if (strcmp(s_a[index_stack - 0], "") != 0) {
                                            relocation_count = machine_templates("mov_addr_rcx", s_a[index_stack - 0], index_vars, relocation_count);
                                            relocation_count = machine_templates("sub_rcx_rax", 0x00, index_vars, relocation_count);
                                            relocation_count = machine_templates("mov_rax_rbx", 0x00, 0, relocation_count);
                                            relocation_count = machine_templates("mov_addr_rax", "0", index_vars, relocation_count);
                                        }
                                        memset(s_a[index_stack - 0], '\0',sizeof(s_a[index_stack - 0]));
                                        memset(s_a[index_stack - 1], '\0',sizeof(s_a[index_stack - 1]));
                                        index_stack = 0;
                                    }
                                    memset(s_a[index_stack - 0], '\0',sizeof(s_a[index_stack - 0]));
                                    memset(s_a[index_stack - 1], '\0',sizeof(s_a[index_stack - 1]));
                                }

                                index_stack++;
                                was_val = false;
                                v_index = 0;
                            }
                        }
//                        sprintf(template, "\tmov %crbx, %s(%rip)\n"
//                                              "\tmov $0, %rbx\n", '%', variable);

                        relocation_count = machine_templates("mov_rbx_addr", variable, index_vars, relocation_count);
                        relocation_count = machine_templates("mov_addr_rbx", "0", 0, relocation_count);
//                            strcat(result, template);
//                            memset(template, '\0', sizeof(template));

//                        }else{
//                            sprintf(template, "\tmov %rbx, %rax\n"
//                                              "\tmov $0, %rbx\n");
//                            strcat(result, template);
//                            memset(template, '\0', sizeof(template));
//                        }
//                    memset(variable, '\0', sizeof(variable));

                        memset(stack, '\0', sizeof(stack));
                        memset(infix, '\0', sizeof(infix));
                        memset(postfix, '\0', sizeof(postfix));
                        tokens = strtok(NULL, ";");
                    }else{
                        // ??????????????
                        char str_ch[50];
                        if(isdigit(*postfix)){

                            relocation_count = machine_templates("mov_addr_rax", postfix, index_vars, relocation_count);

                            relocation_count = machine_templates("mov_rax_addr", variable, index_vars, relocation_count);

                            relocation_count = machine_templates("mov_addr_rax", "0", 0, relocation_count);

                        }else{
                            sprintf(str_ch, "\tmov %s(%crip), %s(%rip)\n", postfix, '%', variable);
                        }
//                        sprintf(template, "%s\tmov %crax, %s(%rip)\n"
//                                                        "\tmov $0, %rax\n", str_ch, '%', variable);
                        strcat(result, str_ch);
                        memset(str_ch, '\0', sizeof(str_ch));
                    }
                }
                get_buffer_from_token(tokens, buffer[0], buffer[1]);
                if(strcmp(words[atoi(buffer[0])][atoi(buffer[1])], "true") == 0){
                    strcat(infix, "1");
                }
                else if(strcmp(words[atoi(buffer[0])][atoi(buffer[1])], "false") == 0){
                    strcat(infix, "0");
                }else{
                    strcat(infix, words[atoi(buffer[0])][atoi(buffer[1])]);
                }
            }else{
                if(strcmp(tokens, "1,0")!=0) {
                    get_buffer_from_token(tokens, buffer[0], buffer[1]);
                    strcpy(variable, words[atoi(buffer[0])][atoi(buffer[1])]);
                    int a =1;
                }
            }

            if (strcmp(tokens, "1,0") == 0) {
                is_condition_start = true;
                memset(template, '\0', sizeof((char *) malloc(13 * sizeof(char))));
                memset(infix, '\0', sizeof(infix));
                memset(postfix, '\0', sizeof(postfix));
            }
            //                   if                             for                         while
            if (strcmp(tokens, "1,6") == 0 || strcmp(tokens, "1,18") == 0 || strcmp(tokens, "1,21") == 0) {
                has_oper = true;
                if(strcmp(tokens, "1,18") == 0)
                    cycle_for = true;
                if(strcmp(tokens, "1,21") == 0)
                    cycle_while = true;
            }

            //                      end                             next
            if(!cycle_for) {
                if (strcmp(tokens, "1,30") == 0 || strcmp(tokens, "1,22") == 0) {
                    if(cycle_while){
                        sprintf(template, "%s"
                                          "\tjg %s\n"
                                          "\tjmp %s\n", cycle_while_str, tmp_block_next_name, tmp_block_name);
                        strcat(result, template);
                        memset(template, '\0', sizeof(template));
                        memset(tmp_block_name, '\0', sizeof(tmp_block_name));
                        cycle_while = false;
                    }
                    sprintf(template, "%s:\n", tmp_block_next_name);
                    strcat(result, template);
                    memset(template, '\0', sizeof(template));
                    memset(tmp_block_next_name, '\0', sizeof(tmp_block_next_name));
                }
            }
            if(has_oper){
                get_buffer_from_token(tokens, buffer[0], buffer[1]);
                if((strcmp(tokens, "1,15")==0 || strcmp(tokens, "1,20")==0) && !cycle_for){ // THEN OR DO
                    sprintf(tmp_block_name, "b%d", block_count);
                    sprintf(tmp_block_next_name, "n%d", block_count++);
                    sprintf(template, "\tcmp %crax, %crbx\n"
                                      "\tje %s\n",'%','%', tmp_block_name);
                    strcat(result, template);
                    memset(template, '\0', sizeof(template));

                    sprintf(template, "%s:\n", tmp_block_name);
                    strcat(result, template);
                    memset(template, '\0', sizeof(template));
                    if(!cycle_while)
                        memset(tmp_block_name, '\0', sizeof(tmp_block_name));
                    has_oper = false;
                }
                if(!cycle_for) {
                    if(cycle_while){
                        if (strcmp(buffer[0], "3") == 0) {
                            if(!has_next_while) {
                                sprintf(template, "\tcmp %s(%rip)", words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(cycle_while_str, template);
                                memset(template, '\0', sizeof(template));
                                has_next_while = true;
                            }else{
                                sprintf(template, ", %s(%rip)\n", words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(cycle_while_str, template);
                                memset(template, '\0', sizeof(template));
                                has_next_while = false;
                            }
                        }else if (strcmp(buffer[0], "2") == 0) {
                            if(!has_next_while) {
                                sprintf(template, "\tmov $%s, %rbp\n"
                                                  "\tcmp %rbp", words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(cycle_while_str, template);
                                memset(template, '\0', sizeof(template));
                                has_next_while = true;
                            }else{
                                sprintf(template, ", $%s\n", words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(cycle_while_str, template);
                                memset(template, '\0', sizeof(template));
                                has_next_while = false;
                            }
                        }
                    }else{
                        if (strcmp(buffer[0], "3") == 0) {
                            if(!has_next) {
                                sprintf(template, "\tmov $0, %crax\n"
                                                  "\tadd %s(%rip), %rax\n", '%', words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(result, template);
                                memset(template, '\0', sizeof(template));
                                has_next = true;
                            }else{
                                sprintf(template, "\tmov $0, %crbx\n"
                                                  "\tadd %s(%rip), %rbx\n", '%', words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(result, template);
                                memset(template, '\0', sizeof(template));
                                has_next = false;
                            }
                        }else if (strcmp(buffer[0], "2") == 0) {
                            if(!has_next) {
                                sprintf(template, "\tmov $0, %crax\n"
                                                  "\tadd $%s, %rax\n", '%', words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(result, template);
                                memset(template, '\0', sizeof(template));
                                has_next = true;
                            }else{
                                sprintf(template, "\tmov $0, %crbx\n"
                                                  "\tadd $%s, %rbx\n", '%', words[atoi(buffer[0])][atoi(buffer[1])]);
                                strcat(result, template);
                                memset(template, '\0', sizeof(template));
                                has_next = false;
                            }
                        }
                    }
                }else{
                    if (strcmp(tokens, "1,19") == 0) { // DETECT VAL

                        tokens = strtok(NULL, ";");
                        get_buffer_from_token(tokens, buffer[0], buffer[1]);
                        if (strcmp(buffer[0], "3") == 0) {
                            sprintf(template, "\tmov %s(%rip), %rbp\n", words[atoi(buffer[0])][atoi(buffer[1])]);
                            strcat(result, template);
                            memset(template, '\0', sizeof(template));
                        } else if (strcmp(buffer[0], "2") == 0) {
                            sprintf(template, "\tmov $%s, %rbp\n", words[atoi(buffer[0])][atoi(buffer[1])]);
                            strcat(result, template);
                            memset(template, '\0', sizeof(template));
                        }

                        sprintf(tmp_block_name, "b%d", block_count);
                        sprintf(tmp_block_next_name, "n%d", block_count++);

                        sprintf(template, "%s:\n", tmp_block_name);
                        strcat(result, template);
                        memset(template, '\0', sizeof(template));


                        sprintf(template, "\tcmp %crbp, %s(%crip)\n"
                                          "\tjg %s\n"
                                          "\tadd $1, %s(%crip)\n", '%', variable, '%', tmp_block_next_name, variable,
                                '%');
                        strcat(result, template);
                        memset(template, '\0', sizeof(template));

                    }
                    if (strcmp(tokens, "1,22") == 0) { // DETECT NEXT
                        sprintf(template, "\tjmp %s\n"
                                          "%s:\n", tmp_block_name, tmp_block_next_name);
                        strcat(result, template);
                        memset(template, '\0', sizeof(template));
                        cycle_for = false;
                        has_oper = false;
                    }
                }
            }
//            sprintf(template, "\tmovq a(%rip), %rax\n"
//                              "\tmovq\t%rax, -8(%rbp)\n"
//                              "\tmovq\t-8(%rbp), %rax", );
//        }
        //               display func
        if(strcmp(tokens, "1,34")==0){
            has_displ = true;
        }
        if(strcmp(tokens, "1,1")==0){
            has_displ = false;
        }
        if(has_displ){
            bool valid = false;
            get_buffer_from_token(tokens, buffer[0], buffer[1]);
            if(strcmp(buffer[0], "3") == 0){
                valid = true;
                sprintf(template, "\tmov %s(%rip), %rdx\n", words[atoi(buffer[0])][atoi(buffer[1])]);
                relocation_count = machine_templates("mov_addr_rdx", words[atoi(buffer[0])][atoi(buffer[1])], index_vars, relocation_count);
                strcat(result, template);
                memset(template, '\0', sizeof(template));
                for(int i = 0; i < vars_count; i++){
                    if(strcmp(def_vars[i].name, words[atoi(buffer[0])][atoi(buffer[1])]) ==0){
                        if(strcmp(def_vars[i].type, "float") == 0) {
                            strcpy(format, ".LC1");
                        }else{
                            strcpy(format, ".LC0");
                        }
                        break;
                    }
                }

            }else if(strcmp(buffer[0], "2") == 0){
                valid = true;
                sprintf(template, "\tmov $%s, %rdx\n", words[atoi(buffer[0])][atoi(buffer[1])]);
                relocation_count = machine_templates("mov_addr_rdx", words[atoi(buffer[0])][atoi(buffer[1])], index_vars, relocation_count);
                strcat(result, template);
                memset(template, '\0', sizeof(template));
            }
            if(valid) {
                sprintf(template, "\tmov $0, %crcx\n"
                                  "\tlea %s(%rip), %rax\n"
                                  "\tmov %rax, %rcx\n"
                                  "\tcall printf\n", '%', format);
                strcat(result, template);
                memset(template, '\0', sizeof(template));
                relocation_count = machine_templates("mov_addr_rcx", 0x00, 0, relocation_count);
                relocation_count = machine_templates("lea_addr", 0x00, index_vars, relocation_count);
                relocation_count = machine_templates("mov_rax_rcx", 0x00, 0, relocation_count);
                relocation_count = machine_templates("print", 0x00, index_vars, relocation_count);
            }
        }
            get_buffer_from_token(tokens, buffer[0], buffer[1]);

//            strcat(stack, words[atoi(buffer[0])][atoi(buffer[1])]);
            strcat(stack, tokens);
            strcat(stack, ";");
        }
        tokens = strtok(NULL, ";");
    }

    long code_count_text = 0;
    code_count_text = (getFileSize(TEMP_OBJ_FILE_NAME));

    append_bytes_from_file(TEMP_OBJ_FILE_NAME_END, TEMP_OBJ_FILE_NAME);
    competition(TEMP_OBJ_FILE_NAME, 0x90);

    SymbolEntry symbolEntry = {
            .s_name = ".file",
            .s_value = 0x00000000,
            .s_section = 0xfffe,
            .s_type = 0x0000,
            .s_storage_class = 0x67,
            .s_num_aux = 0x01
    };

    add_symbol_table(symbolEntry);

    SymbolEntry symbolEntry_fake = {
            .s_name = "fake",
            .s_value = 0x00000000,
            .s_section = 0x0000,
            .s_type = 0x0000,
            .s_storage_class = 0x00,
            .s_num_aux = 0x00
    };

    add_symbol_table(symbolEntry_fake);


    if(index_vars > 1) {
        for (int k = 0; k < index_vars; k++) {
            SymbolEntry symbolEntryVar;
            strcpy(symbolEntryVar.s_name, hex_values_l[k].name);
            symbolEntryVar.s_value = hex_values_l[k].addr;
            symbolEntryVar.s_section = 0x0002;
            symbolEntryVar.s_type = 0x0000,
                    symbolEntryVar.s_storage_class = 0x03;
            symbolEntryVar.s_num_aux = 0x00;
            add_symbol_table(symbolEntryVar);

            add_data_section(hex_values_l[k]);
        }
    }else{
        SymbolEntry symbolEntryVar;
        strcpy(symbolEntryVar.s_name, hex_values_l[0].name);
        symbolEntryVar.s_value = hex_values_l[0].addr;
        symbolEntryVar.s_section = 0x0002;
        symbolEntryVar.s_type = 0x0000,
        symbolEntryVar.s_storage_class = 0x03;
        symbolEntryVar.s_num_aux = 0x00;
        add_symbol_table(symbolEntryVar);

        add_data_section(hex_values_l[0]);
    }


    char* format_data = "%lld\0\0\0\0";
    appendToFile_little_en(TEMP_OBJ_FILE_NAME_DATA, format_data, 8);
    competition(TEMP_OBJ_FILE_NAME_DATA, 0x00);

    SymbolEntry symbolEntry_ep = {
            .s_name = "ep",
            .s_value = code_count_text,
            .s_section = 0x0001,
            .s_type = 0x0000,
            .s_storage_class = 0x06,
            .s_num_aux = 0x00
    };
    add_symbol_table(symbolEntry_ep);

    SymbolEntry symbolEntry_dz = {
            .s_name = "dz",
            .s_value = code_count_text+13,
            .s_section = 0x0001,
            .s_type = 0x0000,
            .s_storage_class = 0x06,
            .s_num_aux = 0x00
    };

    add_symbol_table(symbolEntry_dz);

    SymbolEntry symbolEntry_text = {
            .s_name = ".text",
            .s_value = 0x00000000,
            .s_section = 0x0001,
            .s_type = 0x0000,
            .s_storage_class = 0x03,
            .s_num_aux = 0x01
    };

    add_symbol_table(symbolEntry_text);

    SymbolEntry symbolEntry_bd = {
            .s_name = "bd",
            .s_value = 0x00000000,
            .s_section = 0x0000,
            .s_type = 0x0000,
            .s_storage_class = 0x00,
            .s_num_aux = 0x00
    };

    add_symbol_table(symbolEntry_bd);

    SymbolEntry symbolEntry_data = {
            .s_name = ".data",
            .s_value = 0x00000000,
            .s_section = 0x0002,
            .s_type = 0x0000,
            .s_storage_class = 0x03,
            .s_num_aux = 0x01
    };

    add_symbol_table(symbolEntry_data);

    SymbolEntry symbolEntry_dd = {
            .s_name = "1d",
            .s_value = 0x00000000,
            .s_section = 0x0000,
            .s_type = 0x0000,
            .s_storage_class = 0x00,
            .s_num_aux = 0x00
    };

    add_symbol_table(symbolEntry_dd);

    SymbolEntry symbolEntry_bss = {
            .s_name = ".bss",
            .s_value = 0x00000000,
            .s_section = 0x0003,
            .s_type = 0x0000,
            .s_storage_class = 0x03,
            .s_num_aux = 0x01
    };

    add_symbol_table(symbolEntry_bss);

    SymbolEntry symbolEntry_null = {
            .s_name = 0x0000000000000000,
            .s_value = 0x00000000,
            .s_section = 0x0000,
            .s_type = 0x0000,
            .s_storage_class = 0x00,
            .s_num_aux = 0x00
    };

    add_symbol_table(symbolEntry_null);


    SymbolEntry symbolEntry_main = {
            .s_name = "main",
            .s_value = 0x00000003,
            .s_section = 0x0001,
            .s_type = 0x0000,
            .s_storage_class = 0x02,
            .s_num_aux = 0x00
    };

    add_symbol_table(symbolEntry_main);

//    SymbolEntry symbolEntry_print = {
//            .s_name = "print",
//            .s_value = 0x00000000,
//            .s_section = 0x0001,
//            .s_type = 0x0000,
//            .s_storage_class = 0x02,
//            .s_num_aux = 0x00
//    };

    uint8_t machine_code[] = {
            0x70, 0x72, 0x69, 0x6E, 0x74, 0x66, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
    };

    SymbolEntry symbolEntry_print;
    memcpy(&symbolEntry_print, machine_code, sizeof(SymbolEntry));

    add_symbol_table(symbolEntry_print);

    uint32_t hex_local_l = 0x00000004;
    appendToFile(TEMP_OBJ_FILE_SYMBOLS, &hex_local_l, 4);

    competition(TEMP_OBJ_FILE_NAME, 0x00);
    competition(TEMP_OBJ_FILE_RELOCATIONS, 0x00);
    competition(TEMP_OBJ_FILE_SYMBOLS, 0x00);

    long code_count_data = 0;
    code_count_data = getFileSize(TEMP_OBJ_FILE_NAME_DATA);

    long code_count_relocations = 0;
    code_count_relocations = getFileSize(TEMP_OBJ_FILE_RELOCATIONS);
    raw_data = getFileSize(TEMP_OBJ_FILE_NAME);
    append_bytes_from_file(TEMP_OBJ_FILE_NAME_DATA, TEMP_OBJ_FILE_NAME);

    fileHeader.Machine = 0x8664;
    fileHeader.NumberOfSections = 0x0003;
    fileHeader.TimeDateStamp = 0x00000000;
    fileHeader.PointerToSymbolTable =  raw_data + code_count_data + code_count_relocations - 4 + (48*3);
    fileHeader.NumberOfSymbols =  index_vars + 12;
    fileHeader.SizeOfOptionalHeader = 0x0000;
    fileHeader.Characteristics = 0x0005;

    add_coff_header();

    text.physical_address = 0x00000000;
    text.virtual_address  = 0x00000000;
    text.size_of_raw_data = raw_data;
    text.file_pointer_to_raw_data =  0x0000008c; // !!!!
    text.file_pointer_to_relocation_table = 0x0000008c+raw_data+code_count_data;
    text.file_pointer_to_line_numbers = 0x00000000;
    text.number_of_relocations = relocation_count;
    text.number_of_line_numbers = 0x0000;
    text.flags = 0x60500020;

    hex_local_l = 0x2E; // name .
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 1);
    hex_local_l = 0x74786574; // name text
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 4);

    hex_local_l = 0x000000; // name ____
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 3);

    add_section_header(text);


    data.physical_address = 0x00000000;
    data.virtual_address  = 0x00000000;
    data.size_of_raw_data = code_count_data;
    data.file_pointer_to_raw_data = 0x0000008c + raw_data; // !!!!
    data.file_pointer_to_relocation_table = 0x00000000;
    data.file_pointer_to_line_numbers = 0x00000000;
    data.number_of_relocations = 0x0000;
    data.number_of_line_numbers = 0x0000;
    data.flags = 0xC0500040;

    hex_local_l = 0x2E; // name .
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 1);
    hex_local_l = 0x61746164; // name data
    appendToFile_little_en(OBJ_FILE_NAME, &hex_local_l, 4);
    hex_local_l = 0x000000; // name ____
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 3);

    add_section_header(data);

    bss.physical_address = 0x00000000;
    bss.virtual_address  = 0x00000000;
    bss.size_of_raw_data = 0x00000000;
    bss.file_pointer_to_raw_data =  0x00000000; // !!!!
    bss.file_pointer_to_relocation_table = 0x00000000;
    bss.file_pointer_to_line_numbers = 0x00000000;
    bss.number_of_relocations = 0x0000;
    bss.number_of_line_numbers = 0x0000;
    bss.flags = 0xC0500080;

    hex_local_l = 0x2E; // name .
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 1);
    hex_local_l = 0x737362; // name bss
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 3);

    hex_local_l = 0x00000000; // name ____
    appendToFile(OBJ_FILE_NAME, &hex_local_l, 4);

    add_section_header(bss);


    append_bytes_from_file(TEMP_OBJ_FILE_NAME, OBJ_FILE_NAME);
    append_bytes_from_file(TEMP_OBJ_FILE_RELOCATIONS, OBJ_FILE_NAME);
    append_bytes_from_file(TEMP_OBJ_FILE_SYMBOLS, OBJ_FILE_NAME);

    competition(OBJ_FILE_NAME, 0x00);

//hex_values_l[index_vars]
    strcat(result,
                   "\tmovl\t$0, %eax\n"
                   "\taddq\t$48, %rsp\n"
                   "\tpopq\t%rbp\n"
                   "\n"
                   "\tret");

    strcat(result, "\nep:\n"
                   "\tmov $0, %rax\n"
                   "\taddq $48, %rsp\n"
                   "\tpopq %rbp\n"
                   "\tret\n"
                   "dz:\n"
                   "\tmov $1, %rbx \n"
                   "\tjmp ep\n"
                   "\tmov $0, %rax\n"
                   "\tmov $0, %rbx\n");

    fclose(fopen("asm.s", "w"));
    file_write("asm.s", result);
//    compile("asm.s");
    return true;
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
                    } else if (strcmp(table_number, "2") == 0) { // Var value check
                        var_find_if = true;
                        printf("_FIND VALUE IS %s\n", words[atoi(table_number)][atoi(table_val)]);
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
                    }  else if (strcmp(table_val, "29") == 0 && is_while && (has_value_cycle || var_find_cycle)) {
                        printf("FIND 'GRE'\n");
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
                        // TEMPORARY
                        char *tmp_value_in_for = number_validation(words[atoi(table_number)][atoi(table_val)]);
                        printf("VALUE FOUND '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        strcpy(def_vars[vars_count].value, words[atoi(table_number)][atoi(table_val)]);
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
                                if (def_vars[j].isNull) {
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
//    free(tokens);
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

                                char *tmp_verification;
                                tmp_verification = number_validation(tmp);
                                if (strcmp(tmp_verification, "false_verification") == 0) {
                                    we_have_problem = true;
                                    break;
                                }
                                printf("[%d][%d] Value: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);
                                strcpy(tmp, tmp_verification);

 //                               free(tmp_verification);
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

    fp = fopen("C:\\Users\\rain\\CLionProjects\\CTest\\code2.vl", "rb");
    if (!fp) perror("C:\\Users\\rain\\CLionProjects\\CTest\\code.vl"), exit(1);

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
