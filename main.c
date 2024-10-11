#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

struct var {
    char name[1024];
    int value_int; // Legacy for history NOT USED
    char value_str[1024]; // Legacy for history NOT USED
    float value_float; // Legacy for history NOT USED
    unsigned char value[1024];
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
                                     {'-','-'}, // 9
                                     {"disa"}, // 10
                                     {'+','+'}, // 11
                                     {"add"}, // 12
                                     {"umn"}, // 13
                                     {"del"}, // 14
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
                             },
                             { // vars values
                                     {112},

                             },
                             { // vars names
                                     {'a', 'a'},

                             }
};

char map[1024];

char prohibited[] = {';', '=', '>', '<', '?', ':', '\0'};

char file_view[] = {'%', 'd', ',', '%', 'd', ';'};

struct var def_vars[1024];
int vars_count = 0;
int vars_count_replace = 0;

// NOT MINE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void file_write(const char *filepath, const char *data) {
    FILE *fp = fopen(filepath, "ab");
    if (fp != NULL) {
        fputs(data, fp);
        fclose(fp);
    }
}


char* hexToBinary(char hex[]) {
    int len = strlen(hex);
    char* binary = (char*)malloc((len * 4 + 1) * sizeof(char));
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

char* decToBinary(char dec[]) {
    double num = atof(dec);
    long long intPart = (long long) num;
    double fracPart = num - intPart;
    char *binary;
    bool all_empty = (intPart==0)&&(fracPart == 0);
    bool int_empty = intPart==0;
    if(!int_empty) {
        long long len = floor(log2(intPart)) + 1;
        binary = (char *) malloc((len + 1) * sizeof(char));
        for (int i = len - 1; i >= 0; i--) {
            int rem = intPart % 2;
            intPart /= 2;
            if (rem == 0) {
                binary[i] = '0';
            } else {
                binary[i] = '1';
            }
        }
        binary[len] = '\0';
    }
    if (fracPart > 0) {
        char* fracBinary = (char*)malloc(100 * sizeof(char));
        fracBinary[0] = '.';
        int idx = 1;
        while (fracPart > 0 && idx < 100) {
            fracPart *= 2;
            int bit = (int)fracPart;
            if (bit == 0) {
                fracBinary[idx++] = '0';
            } else {
                fracBinary[idx++] = '1';
                fracPart -= 1;
            }
        }
        fracBinary[idx] = '\0';

        // Concatenate integer and fractional parts
        char* result;
        if(!int_empty){
            result = (char*)malloc((strlen(binary) + strlen(fracBinary) + 1) * sizeof(char));
            strcpy(result, binary);
            free(binary);
        }else{
            result = (char*)malloc((strlen(fracBinary) + 1) * sizeof(char));
            strcpy(result, "0");
        }
        strcat(result, fracBinary);
        free(fracBinary);
        return result;
    }
    if(!all_empty)
        return binary;
    else
        return "0000";
}

char* octToBinary(char oct[]) {
    int len = strlen(oct);
    char* binary = (char*)malloc((len * 3 + 1) * sizeof(char));
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


const char *number_validation(const char *value){

    char latin_hex_alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f', 'x'};

    bool start_with_number = isdigit(value[0]);
    bool exponential_write_plus = false;
    bool exponential_write_exp = false;
    bool is_hex = false;
    bool valid = true;
    bool is_real = false;
    bool is_real_has_digit = false;

    const char *answer = nullptr;
//    memset(answer, '\0', sizeof(answer));

    int i = 0;
    char tmp_value[1024];
    memset(tmp_value, '\0', sizeof(tmp_value));
//    tmp_value[0] = value[0];
//    for(; *(value+i); i++){
//    unsigned long long str_len_value = strlen(value) > 1 ? strlen(value) - 1 : strlen(value) ;
    unsigned long long str_len_value = !isdigit(value[strlen(value)-1]) && strlen(value) > 1 ? strlen(value) - 1 : strlen(value) ;
    for(; i < str_len_value; i++){
        tmp_value[i] = value[i];

        if(value[i] == 'E' || value[i] == 'e'){
            printf("Exponential read?\n");
            exponential_write_exp = true;
        }
        if(value[i] == '+' || value[i] == '-'){
            printf("Plus find...Exponential read.\n");
            if(exponential_write_exp)
                exponential_write_plus = true;
            else
                printf("Error. In exp writing not found 'E'\n");
        }
        if(!exponential_write_plus && !exponential_write_exp) {
            if (!isdigit(value[i])) {
                bool for_real_hex_check = false;
                for (int j = 0; j < sizeof(latin_hex_alphabet) + 1; j++) {
                    if (value[i] == latin_hex_alphabet[j]) {
                        is_hex = true;
                        for_real_hex_check = true;
                        break;
                    }
                }
                if(value[i] == '.'){
                    is_real = true;
                    continue;
                } else if(!is_hex) {
                    printf("Error. Find letter but it not in hex table\n");
                    valid = false;
                    break;
                }
                if(is_real && !for_real_hex_check){
                    printf("Error. Real format number not valid\n");
                    valid = false;
                    break;
                }
            }
        }
    }

    if(exponential_write_exp && !exponential_write_plus){
        printf("In exp writing not found '+'\n");
    }

    if(valid) {

        double answer_double = 0;
        float answer_float = 0;
        int answer_int = 0;

        char last_char = value[i];
//        tmp_value[i-1] = '\0';

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
        }
        else {
            if (exponential_write_exp) {
                answer_double = atof(tmp_value);
                snprintf(tmp_value, 50, "%f", answer_double);
            } else if (is_real) {
                answer_float = atof(tmp_value);
                snprintf(tmp_value, 50, "%f", answer_float);
            } else {
                answer_int = atoi(tmp_value);
                snprintf(tmp_value, 50, "%d", answer_int);
            }
            printf("Digit is decimal \n");
            answer = decToBinary(tmp_value);
        }
        int a = 1;
    }
    if(answer==NULL)
        valid = false;

    if(valid)
        return answer;
    else
        return "false_verification";
}

char **str_split(char *a_str, const char a_delim) {
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result) {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
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

        if(str[i] == '#') {
            if(comment_found){
                i++;
            }
            comment_found = !comment_found;
        }

        if (!isspace((unsigned char)str[i]) && !comment_found) {
            *result = str[i];
            result++;
        }
    }
    *result = '\0';
}

const char *check_type_data(const char *value){
    const char *type_data = "int";

    if(strcmp(value, "true")==0 || strcmp(value, "false")==0){
        type_data = "bool";
    }

    for(int i = 0; *(value + i); i++){
        if(value[i] == '.'){
            type_data = "float";
            break;
        }
    }
    return type_data;
}

int putVarValToVar(int number_dist, int number_source){
    int answer = 0;
    if(strcmp(def_vars[number_source].type, def_vars[number_dist].type)==0){
        printf("Var %s with old value: ", def_vars[number_dist].name);
        printf("%s ", def_vars[number_dist].value);
        strcpy(def_vars[number_dist].value, def_vars[number_source].value);
        printf("has new value: %s type: %s\n", def_vars[number_dist].value, def_vars[number_source].type);
        printf(" from %s\n", def_vars[number_source].name);
        def_vars[number_dist].isNull = false;

    }else{
        printf("Type of var: %s is %s, given %s, from var %s\n", def_vars[number_dist].name, def_vars[number_dist].type, def_vars[number_source].type, def_vars[number_source].name);
        answer = 1;
    }

    return answer;
}

int putValToVar(int number_dist, char value[]){
    int answer = 0;

    const char* type;
    type = check_type_data(value);

    if(strcmp(def_vars[number_dist].type, type)==0){
        printf("Var %s with old value: ", def_vars[number_dist].name);
        printf("%s ", def_vars[number_dist].value);
        printf(" from %s\n", value);

       strcpy(def_vars[number_dist].value, value);
       printf("has new value: %s type: %s\n", value,  def_vars[number_dist].type);

    }else{
        printf("Type of var: %s is %s, given %s\n", def_vars[number_dist].name, def_vars[number_dist].type, type);
        answer = 1;
    }
    def_vars[number_dist].isNull = false;

    return answer;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool code_work(char _map[]){

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
    char* tmp_vars_names[1024];
    char* type_for_check;

    int var_num_local_tmp = 0;
    int find_var = 0;
//    int value_int = 0;
    char value_int[1024];
    memset(value_int, '\0', sizeof(value_int));

    // Equaling
    bool found_val_or_var = false;

    // not used
    bool var_replacing = false;

    // code body
    bool work_space = false;

    // bools for if
    bool is_if = false;
    bool has_condition_if = false;
    bool then_find = false;
    bool var_find_if = false;

    // var number for 'if'
    int var_if_number = 0;

    // Cycles bools
    bool has_next_token = false;
    bool is_while = false;
    bool has_val = false;
    bool has_condition_cycle = false;
    bool has_equaling = false;
    bool is_cycle = false;
    bool var_find_cycle = false;
    bool has_value_cycle = false;

    // program start
    bool was_start = false;
    char tmp_var_type_name[1024];

    int line=1;

   printf("______________________________\n");
   printf("PROGRAM SYNTAX ANALYSE STARTED\n");
   printf("______________________________\n");

    for (int i = 0; *(tokens + i); i++) {
        if (not_error) {

            char **delimited_str = str_split(tokens[i], ',');
            char *table_number = delimited_str[0];
            char *table_val = delimited_str[1];
            if((strcmp(table_number, "1")==0 && strcmp(table_val, "1")==0) ||
            (strcmp(table_number, "1")==0 && strcmp(table_val, "5")==0)){
                line++;
            }
            if(strcmp(table_number, "1")==0 && strcmp(table_val, "24")==0){
                was_start = true;
                line++;
               printf("WORD program FIND\n");
                continue;
            }
            else if(strcmp(table_number, "1")==0 && strcmp(table_val, "25")==0 && was_start){
                was_start = true;
               printf("WORD var FIND\n");
                continue;
            }else if(!was_start){
                printf("Error. has not required token 'program var'\n");
                not_error = false;
                break;
            }
            if(syntax_lvl == 0) {
                if (strcmp(table_number, "3") == 0) {
                    char *var_name = delimited_str[1];
                   printf("DECLARATION NAME VAR IS:%s\n", words[3][atoi(var_name)]);
                    strcpy(tmp_var_name_dec, words[3][atoi(var_name)]);

                    for (int name = 0; name < 1024; name++) {
                        if (strcmp(def_vars[name].name, words[3][atoi(var_name)]) == 0) {
//                            vars_count_replace = vars_count;
//                            var_replacing = true;
//                            vars_count = name;
                            printf("Error. Var name is used:%s\n", words[3][atoi(var_name)]);
                            not_error = false;
                            break;
                        }
                    }
                    strcpy(def_vars[vars_count].name, words[3][atoi(var_name)]);
                    tmp_vars_names[count_vars_names] = words[3][atoi(var_name)];
                    count_vars_names++;
                }
                if(strcmp(table_number, "1") == 0 && strcmp(table_val, "31") == 0){
                    not_one = true;
                    printf("WARNING. More than one variable\n");
                    syntax_lvl = 0;
                }
                if(strcmp(table_number, "1")==0 && strcmp(table_val, "23")==0){
                    printf("DELIMITER : FIND\n");
                    syntax_lvl = 1;
                    var_declaration = true;
                }
            }
            if(strcmp(table_number, "1") == 0 && strcmp(delimited_str[1], "5") == 0){
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
                        if(not_one){
                            for(int var_name = 0; var_name<count_vars_names; var_name++){
                                strcpy(def_vars[vars_count].name, tmp_vars_names[var_name]);
                                strcpy(def_vars[vars_count].type, tmp_var_type_name);
                                def_vars[vars_count].isNull = true;
                                printf("Added new var from list of vars %s\n", def_vars[vars_count].name);
                                printf("Type is:%s\n", tmp_var_type_name);
                                vars_count++;
                            }
                        }else{
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
                } else if (syntax_lvl == 2) { // CHECK VALUE OF VAR UNUSED

                }
            }
            if (work_space){

                if (syntax_lvl == 3) { // ENTER AFTER BEGIN

                    if(is_if && !then_find){
                        printf("Error. Required token 'Then' in if not found\n");
                        not_error = false;
                        break;
                    }

                    if((val_find_local_tmp || var_find_local_tmp)&& !var_action){

                        if(strcmp(table_number, "1")==0 && strcmp(table_val, "1")==0){
                            int valid = 0;
                            if(is_value){
                                valid = putValToVar(find_var, value_int);
                                if(valid == 1){
                                    not_error = false;
                                }
                                val_find_local_tmp = false;
                                var_find_local_tmp = false;
                                find_var, var_num_local_tmp = 0;
                            }else{
                                valid = putVarValToVar(find_var, var_num_local_tmp);
                                if(valid == 1){
                                    not_error = false;
                                }
                                val_find_local_tmp = false;
                                var_find_local_tmp = false;
                                find_var, var_num_local_tmp = 0;
                            }
                        }else{
                            syntax_lvl = 6;
                        }

                    }

                    if(strcmp(table_val, "30")==0 && strcmp(table_number, "1")==0 && is_if){
                       printf("FIND 'END'. IF END.\n");
                        is_if = false;
                        then_find = false;
                    }else if(strcmp(table_val, "30")==0 && strcmp(table_number, "1")==0 && !is_if){
                        printf("Error. If is not open\n");
                        not_error = false;
                        break;
                    }
                    if(strcmp(table_val, "22")==0 && strcmp(table_number, "1")==0 && is_cycle){
                       printf("FIND 'NEXT'. WHILE ENDED.\n");
                        is_cycle = false;
                    }
                    if (strcmp(table_number, "1") == 0){ // Finding delimiter
                        table_val = delimited_str[1];
                        if(strcmp(table_val, "6") == 0){ // if find
//                            syntax_lvl = 6;
                            syntax_lvl = 7;
                            printf("IF FIND\n");
                            is_if = true;
                            continue;
                        }else if((strcmp(table_val, "18") == 0) || (strcmp(table_val, "21") == 0)){ // for | while find
                            syntax_lvl = 8;

                            printf("CYCLE '%s' FIND\n", words[1][atoi(table_val)]);
                            is_while = (strcmp(table_val, "21") == 0);
                            has_val = is_while;
                            has_equaling = is_while;
                            is_cycle = is_while;

                            has_condition_cycle = false;
//                            is_if = true;
                            continue;
                        }else{
                           printf("DELIMITER FIND '%s'\n", words[1][atoi(table_val)]);
                        }
                    }else if (strcmp(table_number, "3") == 0) { // var names
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
                            printf("Error. unexpected token '%s'. Var is undefined\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    }else{ // delims
                       printf("FIND '%s'\n", words[atoi(table_number)][atoi(table_val)]);
                    }
                    if(strcmp(table_number, "1") == 0 && strcmp(table_val, "16") == 0){
                        printf("'end.' FIND, program terminated\n");
                        break;
                    }
                }
                if(syntax_lvl == 4){ // AFTER BEGIN
                    if (strcmp(table_number, "1") == 0) { // delims
                        table_val = delimited_str[1];
                       printf("FIND DELIMITER %s\n", words[1][atoi(table_val)]);
                        if (strcmp(table_val, "0") == 0){
                           printf("EQUATING\n");
                            syntax_lvl = 5;
                            continue;
                        }else if (strcmp(table_val, "6") == 0) { // if
//                            syntax_lvl = 6;
                            syntax_lvl = 7;
//                        }else if (strcmp(table_val, "1") == 0){ // ;
                        }else{ // ;
                            syntax_lvl = 3;
                        }
                        continue;
                    }else{
                        printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                }
                if(syntax_lvl == 5){ // EQUALING
                    is_value = false;
//                    value_int = 0;
                    memset(value_int, '\0', sizeof(value_int));
                    table_val = delimited_str[1];
                   // var_find_local_tmp, val_find_local_tmp = false;
                    if(strcmp(table_number, "3")==0){ // Var check
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
                                if(!def_vars[j].isNull){
                                   printf("EQUALING VAR %s FIND. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                    var_find_local_tmp = true;
                                    var_action = false;
                                    found_val_or_var = true;
                                    var_num_local_tmp = j;
//                                    syntax_lvl = 5;
                                    syntax_lvl = 3;
//                                    break;
                                }else {
                                    printf("Error. VAR %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }

                            }
                        }
                        if(!var_find_local_tmp){
                            printf("Error. unexpected token '%s'. Var is undefined\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                        else{
                            continue;
                        }
                    }
                    else if(strcmp(table_number, "2")==0){ // Var value check
                        val_find_local_tmp = true;
                       printf("FIND VALUE IS %s\n", words[atoi(table_number)][atoi(table_val)]);
                        found_val_or_var = true;

                        syntax_lvl = 3;
//                        syntax_lvl = 5;
                        is_value = true;
//                        value_int =atoi(words[atoi(table_number)][atoi(table_val)]);
                        strcpy(value_int, words[atoi(table_number)][atoi(table_val)]);
                        var_action = false;

                        continue;
                    }else if(strcmp(table_number, "1")==0){
                        found_val_or_var = false;

                        syntax_lvl = 6;
                    }
                }
                if(syntax_lvl == 6){ // REUSE BLOCK FOR EQUATION
                    bool var_find_eq = false;
//                    bool val_find_eq = false;
                    if(strcmp(table_number, "3")==0){ // Var check
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
                                if(!def_vars[j].isNull){
                                    printf("_EQUATION_ VAR %s FIND. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                    var_find_eq = true;
                                    found_val_or_var = true;
                                }else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                            }
                        }
                        if(!var_find_eq){
                            printf("Error. unexpected token '%s'. Var is undefined\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    }
                    else if(strcmp(table_number, "2")==0){ // Var value check
//                        val_find_eq = true;
                        found_val_or_var = true;
                        printf("FIND VALUE IS %s\n", words[atoi(table_number)][atoi(table_val)]);
                    }

                    if(strcmp(table_number, "1") == 0 && !found_val_or_var) {
                        printf("Error. Value or variable not found\n");
                        not_error = false;
                        break;
                    }
                    if(strcmp(table_number, "1") == 0) {
                        val_find_local_tmp = false;
                        var_find_local_tmp = false;
                        found_val_or_var = false;
                        if (strcmp(table_val, "10") == 0) {
                            var_action = true;
                           printf("IS MINUS\n");
//                            continue;
                        } else if (strcmp(table_val, "12") == 0) {
                            var_action = true;
                           printf("IS PLUS\n");
//                            continue;
                        } else if (strcmp(table_val, "13") == 0) {
                            var_action = true;
                           printf("IS MULTIPLY\n");
//                            continue;
                        } else if (strcmp(table_val, "14") == 0) {
                            var_action = true;
                           printf("IS DIV\n");
                           if(strcmp(type_for_check, "int") == 0 || strcmp(type_for_check, "bool") == 0){
                               printf("Error. Find int in DIV operation.\n");
                               not_error = false;
                               break;
                           }
//                            continue;
                        }else if (var_action) {
                            printf("Error. Variable not found.\n");
                            not_error = false;
                            break;
                        }else {
                            syntax_lvl = 3;
                            var_action = false;
                            continue;
                        }
                    }else {
                        syntax_lvl = 3;
                        var_action = false;
                        val_find_local_tmp, var_find_local_tmp = false;
                        continue;
                    }
                    syntax_lvl = 6;

                }
                if(syntax_lvl == 7){ // IF
                    table_val = delimited_str[1];
                    if(strcmp(table_number, "3")==0){
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
//                               printf("VAR %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
//                                var_find_if = true;
                                if(!def_vars[j].isNull){
                                   printf("VAR IN IF %s FIND. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                   var_find_if = true;
                                   var_if_number = j;
                                }else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                            }
                            if(var_find_if){
                                break;
                            }
                        }
                        if(!var_find_if){
                            printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    }
                    if(strcmp(table_number, "1")==0) {
                        if (strcmp(table_val, "2") == 0) { // condition in if
                           printf("FIND '>'\n");
                            has_condition_if = true;
                            if(var_find_if) {
                                var_find_if = false;
                            }else {
                                printf("Error. Undefined token, value not found %s\n",
                                       words[atoi(table_number)][atoi(table_val)]);
                                not_error = false;
                                break;
                            }
                        } else if (strcmp(table_val, "4") == 0) {
                           printf("FIND '<'\n");
                            has_condition_if = true;
                            var_find_if = false;
                            if(var_find_if) {
                                var_find_if = false;
                            }else {
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
                        } else if(!var_find_if && has_condition_if) {
                            printf("Error. Undefined token, value not found %s\n",
                                   words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        } else if (!has_condition_if && var_find_if) {
                           printf("FIND 'Then' but condition not found! Checking var value from var '%s'\n", def_vars[var_if_number].name);
                           then_find = true;
                           var_find_if = false;
                           has_condition_if = false;
                           syntax_lvl = 3;
                           continue;
//                            for (int j = 0; j < 1024; j++){
//                                if(strcmp(def_vars[j].name, words[atoi(table_number)][atoi(table_val)]) == 0){
//
//                                    if(!def_vars[j].isNull){
//                                        printf("VAR IN IF %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
//                                        var_find_cycle = true;
//                                    }else {
//                                        printf("Error. Value of var: %s is undefind\n", def_vars[j].name);
//                                        not_error = false;
//                                        break;
//                                    }
//                                }
//                                if(var_find_cycle){
//                                    break;
//                                }
//                            }
                        }else{
                            syntax_lvl = 3;
                        }
                        continue;
                    }
                }

                if(syntax_lvl == 8){ // CYCLES
                    table_val = delimited_str[1];
                    if(strcmp(table_number, "3")==0){
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
                                //printf("VAR %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
                                //var_find_cycle = true;

                                if(!def_vars[j].isNull){
                                   printf("VAR IN CYCLE %s FIND. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                    var_find_cycle = true;
                                }else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                            }
                            if(var_find_cycle){
                                break;
                            }
                        }
                        if(!var_find_cycle){
                           printf("VAR NOT FOUND '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            strcpy(def_vars[vars_count].name, words[atoi(table_number)][atoi(table_val)]);
                            syntax_lvl = 9;
//                            not_error = false;
//                            break;
                        }
                    }
                    else if(strcmp(table_number, "1")==0 && strcmp(table_val, "19")==0 && has_equaling){
                       printf("WORD VAL FOUND\n");
                        has_val = true;
                        continue;
                    }
                    else if(strcmp(table_number, "2")==0 && has_val){
                       printf("VAL '%s' FOUND.\n", words[atoi(table_number)][atoi(table_val)]);
                        has_condition_cycle = true;
                        has_value_cycle = true;
                        continue;
                    }else if(strcmp(table_val, "2") == 0 && is_while && (has_value_cycle || var_find_cycle)){ // condition in while
                       printf("FIND '>'\n");
                        has_condition_cycle = true;
                        //has_val = false;
                        has_value_cycle = false;
                        var_find_cycle = false;
                        continue;
                    }else if(strcmp(table_val, "4") == 0 && is_while && (has_value_cycle || var_find_cycle)){
                       printf("FIND '<'\n");
                        has_condition_cycle = true;
                        //has_val = false;
                        has_value_cycle = false;
                        var_find_cycle = false;
                        continue;
                    }else if(strcmp(table_number, "1")==0 && strcmp(table_val, "20")==0 && has_condition_cycle){
                       printf("DO FOUND\n");
                        if(!var_find_cycle && !has_value_cycle && is_while){
                            printf("Error. Var in condition cycle not find\n");
                            not_error = false;
                            break;
                        }
                        has_val = false;
                        var_find_cycle = false;
                        has_value_cycle = false;
                        syntax_lvl = 3;
                        has_condition_cycle = false;
                    }else{
                        printf("Error. required token not find. Find '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
//                    if(strcmp(table_val, "6") == 0){
//                       printf("FIND IF STARTED.\n");
//                        syntax_lvl = 7;
//                        is_if = true;
//                    }else if((strcmp(table_val, "21") == 0) || (strcmp(table_val, "18") == 0)){
//                       printf("FIND 'Then'. IF STARTED.\n");
//                        syntax_lvl = 8;
//                        is_if = true;
//                    }
                    continue;
                }

                if(syntax_lvl == 9){ // do assign for value in "for"
                    if((strcmp(table_number, "1")==0) && (strcmp(table_val, "0")==0)){
                       printf("ASSIGN FIND IN FOR\n");
                        continue;
                    }else if((strcmp(table_number, "2")==0)){
                        const char *tmp_value_in_for = number_validation(words[atoi(table_number)][atoi(table_val)]);
                        printf("VALUE FOUND '%s'.\n", tmp_value_in_for);
                        strcpy(def_vars[vars_count].value, tmp_value_in_for);
                        vars_count++;
                        has_equaling = true;
                        var_find_cycle = true;
                        syntax_lvl = 8;
                        continue;
                    }else if((strcmp(table_number, "3")==0)){
                        bool var_find = false;
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
//                               printf("VAR %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
                                if(!def_vars[j].isNull){
                                    printf("VAR %s FIND IN NEW VALUE ADDING. VALUE IS %s\n", def_vars[j].name, def_vars[j].value);
                                    var_find_cycle = true;
                                }else {
                                    printf("Error. Value of var: %s is undefined\n", def_vars[j].name);
                                    not_error = false;
                                    break;
                                }
                                strcpy(def_vars[vars_count].value, def_vars[j].value);
                                printf("VAR %s IN FOR HAS NEW VAL IS %s\n", def_vars[vars_count].name, def_vars[vars_count].value);
                                var_find = true;
                                break;
                            }
                        }
                        if(var_find){
                            vars_count++;
                            has_equaling = true;
                            syntax_lvl = 8;
                        }else{
                            printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                        continue;
                    }else{
                        printf("Error. unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                }
            }
        }
    }
    if(is_cycle){
        printf("Error. 'Next' after cycle not found.\n");
        not_error = false;
    }
    if(is_if){
        printf("Error. 'End' after if not found.\n");
        not_error = false;
    }
    if(!not_error) {
       printf("Error. On %d line.\n", line);
    }
    return not_error;
}

void code_check_file_write(const char chars[300]) {
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
    bool not_type = true;

    bool we_have_problem = false;

   printf("______________________________\n");
   printf("PROGRAM LEXEME ANALYSE STARTED\n");
   printf("______________________________\n");
    for (int i = 0; *(chars + i); i++) {
        if(we_have_problem)
            break;

        for (int del = 0; del <= 33; del++) {

            for (int sub_del = 0; *(words[1][del] + sub_del); sub_del++) {

                if(chars[i] == words[1][del][0]){
                    for(int sub_del_chk = 0; *(words[1][del] + sub_del_chk); sub_del_chk++ ){
                        if(chars[i+sub_del_chk] == words[1][del][sub_del_chk]){
                            is_del = true;
                        }else{
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
                    }else{
                        delimiter_detected = false;
                        i++;
                    }
                }else if(chars[i] != words[1][del][sub_del] && is_del){
                    i = i - (int) strlen(words[1][del])+1;
                    is_del = false;
                }

            if (delimiter_detected) {
                bool b = false;
                while (true) {
                    if(strcmp("", tmp_type)==0){
//                        break;
                    }


                    if(!type_declaration && (strcmp("", tmp_type)!=0)) {
                        for (int kk = 0; kk < 1024; kk++) {
                            if (strcmp(words[0][kk], tmp_type) == 0) {
                               printf("[%d][%d] Type: '%s' | tmp: %s \n", 0, kk, words[0][kk], tmp_type);

                                sprintf(tmp_str, file_view, 0, kk);
                                strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);
                                memset(tmp_str, '\0', sizeof(tmp_str));
                            }
                        }
                    }
                    if(strcmp("", tmp)==0){
                        break;
                    }
                    char first_tmp[] = {tmp[0]};
                        if (isNumber(first_tmp) || tmp[0] == '.' || strcmp(tmp, "true") == 0 || strcmp(tmp, "false") == 0) {

                            if(strcmp(tmp, "true") != 0 && strcmp(tmp, "false") != 0) {
                                const char* tmp_verification;
                                tmp_verification = number_validation(tmp);
                                if (strcmp(tmp_verification, "false_verification") == 0) {
                                    we_have_problem = true;
                                    break;
                                }
                                printf("[%d][%d] Value: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);
                                strcpy(tmp, tmp_verification);
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
    if(type_declaration) {
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
        for(int k = 1; *(tmp+k); k++){
            tmp[k] = tmp[k];
        }
        i--;
    }
    j++;

}
    if(!we_have_problem) {
        file_write("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt", map);

        bool syntax_next = syntax_check(map);
        if (syntax_next) {

            code_work(map);
        }
    }
}


int main() {

    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen ( "C:\\Users\\rain\\CLionProjects\\CTest\\code" , "rb" );
    if( !fp ) perror("C:\\Users\\rain\\CLionProjects\\CTest\\code"),exit(1);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

    if( 1!=fread( buffer , lSize, 1 , fp) )
        fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

    fclose(fp);

    removeSpacesAndNewlines(buffer);
    code_check_file_write(buffer);
    free(buffer);

    return 0;
}
