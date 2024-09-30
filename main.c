#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct var {
    char name[1024];
    int value_int;
    char value_str[1024];
    float value_float;
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
                                     {'>'}, // 2
                                     {' '}, // 3
                                     {'<'}, // 4
                                     {"begin"}, // 5
                                     {"if"}, // 6
                                     {'('}, // 7
                                     {')'}, // 8
                                     {'-','-'}, // 9
                                     {"disa"}, // 10
                                     {'+','+'}, // 11
                                     {"add"}, // 12
                                     {'*'}, // 13
                                     {'/'}, // 14
                                     {"then"}, // 15
                                     {"end"}, // 16
                                     {"print"}, // 17
                                     {"for"}, // 18
                                     {"val"}, // 19
                                     {"do"}, // 20
                                     {"while"}, // 21
                                     {"next"}, // 22
                                     {":"}, // 23
                                     {"program"}, // 24
                                     {"var"}, // 25
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
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            *result = str[i];
            result++;
        }
//        if (str[i] != '\n') {
//            *result = str[i];
//            result++;
//        }
    }
    *result = '\0';
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool code_work(char _map[]){
    int lvl = 0;
    char **tokens = str_split(_map, ';');
    bool start = false;
    int tmp_value;

    for (int i = 0; *(tokens + i); i++) {
        char **delimited_str = str_split(tokens[i], ',');
        char *table_number = delimited_str[0];
        char *table_value  = delimited_str[1];
        printf("%s\n", table_number);
        if(strcmp(table_number, "1")==0 && strcmp(table_value, "5")==0){
            start = true;
            continue;
        }
        if(start){
            table_value  = delimited_str[1];
            if(lvl == 0) {
                if (strcmp(table_number, "3") == 0) { // vars find
                    tmp_value = atoi(table_value);
                    lvl = 1; // var work
                    continue;
                }
            }
            if(lvl == 1){
                table_value  = delimited_str[1];

                if(strcmp(table_number, "1")==0 && strcmp(table_value, "0")==0){ // equaling
                    lvl = 2;
                    continue;
                }
            }
            if(lvl == 2){
                int value = 0;
                bool var_find = false;
                for(int j=0;j<1024;j++){
                    if(strcmp(def_vars[j].name, words[3][tmp_value]) == 0){
                        var_find = true;
                        value = def_vars[j].value_int;
                        break;
                    }
                }
                if(var_find){
                    printf("%d\n", value);
                }
            }

        }
    }
    return false;
}

bool syntax_check(char _map[]) {
    char **tokens = str_split(_map, ';');
    int syntax_lvl = 0;
    char type_var[10];

    bool not_error = true;
    bool var_declaration = false;
    bool var_replacing = false;
    bool work_space = false;
    bool is_if = false;
    bool has_next_token = false;

    bool was_start = false;
    char tmp_var_name[1024];

    for (int i = 0; *(tokens + i); i++) {
        if (not_error) {

            char **delimited_str = str_split(tokens[i], ',');
            char *table_number = delimited_str[0];
            char *table_val = delimited_str[1];

            if(strcmp(table_number, "1")==0 && strcmp(table_val, "24")==0){
                was_start = true;
                printf("WORD program FIND\n");
                continue;
            }
            else if(strcmp(table_number, "1")==0 && strcmp(table_val, "25")==0 && was_start){
                was_start = true;
                printf("WORD var FIND\n");
                continue;
            }
            if(syntax_lvl == 0) {
                if (strcmp(table_number, "3") == 0) {
                    char *var_name = delimited_str[1];
                    printf("DECLARATION NAME VAR IS:%s\n", words[3][atoi(var_name)]);
                    strcpy(type_var, words[3][atoi(var_name)]);

                    for (int name = 0; name < 1024; name++) {
                        if (strcmp(def_vars[name].name, words[3][atoi(var_name)]) == 0) {
                            vars_count_replace = vars_count;
                            var_replacing = true;
                            vars_count = name;
                        }
                    }
                    strcpy(def_vars[vars_count].name, words[3][atoi(var_name)]);
                    syntax_lvl = 1;
                    var_declaration = true;
                }
            }
            if(strcmp(table_number, "1") == 0 && strcmp(delimited_str[1], "5") == 0){
                syntax_lvl = 3;
                work_space = true;
                printf("BEGIN detected\n");
            }

            if (var_declaration) {

                if (syntax_lvl == 1) { // CHECK TYPE VAR
                    char *var_number = delimited_str[1];
                    if(strcmp(table_number, "1")==0 && strcmp(table_val, "1")==0){
                        printf("DELIMITER : FIND\n");
                        continue;
                    }
                    if (strcmp(table_number, "0") == 0) {
                        printf("TYPE VAR IS:%s\n", words[0][atoi(var_number)]);
                        strcpy(tmp_var_name, words[0][atoi(var_number)]);

                        int pr_len = strlen(prohibited);
                        for (int j = 0; j < pr_len; j++) {
                            if (strchr(words[3][atoi(var_number)], prohibited[j]) != NULL) {
                                printf("syntax error\n");
                                printf("Name '%s' not allowed!\n", words[3][atoi(var_number)]);
                                not_error = false;
                                break;
                            }
                        }
                        syntax_lvl = 0;
                        i++;
                        var_declaration = false;
                        vars_count++;
                    }
                } else if (syntax_lvl == 2) { // CHECK VALUE OF VAR
                    char *var_val = delimited_str[1];
                    if (strcmp(table_number, "1") == 0) {
                        char *del = delimited_str[1];
                        if ((strcmp(words[1][atoi(del)], "=") == 0)) {
                            i++;
                        } else {
                            printf("Has not 'assign'\n");
                            not_error = false; break;
                        }
                    }
                    if (strcmp(table_number, "2") == 0) {
                        if (strcmp(type_var, "int") == 0) {
                            if (isNumber(words[2][atoi(var_val)]) == 0) {
                                printf("type error in '%s'. '%s' not integer.\n", tmp_var_name,
                                       words[2][atoi(var_val)]);
                                not_error = false;
                                break;
                            }
                        }
                        printf("VALUE OF VAR IS:%s\n", words[2][atoi(var_val)]);
                        def_vars[vars_count].value_int = atoi(words[2][atoi(var_val)]);
                        vars_count++;
                        syntax_lvl = 0;
                        var_declaration = false;
                    } else {
                        bool var_not_pass = true;
                        for (int var_i = 0; var_i < 1024; var_i++) {
                            if (strcmp(def_vars[var_i].name, words[3][atoi(var_val)]) == 0) {
                                printf("NEW VALUE OF VAR IS:%d\n", def_vars[var_i].value_int);
                                def_vars[vars_count].value_int = def_vars[var_i].value_int;
                                var_not_pass = false;
                                var_declaration = false;
                                vars_count++;
                                syntax_lvl = 0;
                            }
                        }
                        if (var_not_pass) {
                            printf("unexpected token '%s'.\n", words[3][atoi(var_val)]);
                            not_error = false;
                            break;
                        }
                    }
                } else {
                    printf("error\n");
                }
            }
            if (var_replacing) {
                vars_count = vars_count_replace;
                var_replacing = false;
            }
            if (work_space){
                if (syntax_lvl == 3) { // ENTER AFTER BEGIN
                    if(strcmp(table_val, "16")==0 && strcmp(table_number, "1")==0 && is_if){
                        printf("FIND 'END'. IF END.\n");
                        is_if = false;
                    }
                    if (strcmp(table_number, "1") == 0){ // Finding if
                        table_val = delimited_str[1];
                        if(strcmp(table_val, "6") == 0){
//                            syntax_lvl = 6;
                            syntax_lvl = 7;
                            printf("IF FIND\n");
                            is_if = true;
                            continue;
                        }else if(strcmp(table_val, "18") == 0){ // Finding for
                            syntax_lvl = 8;
                            printf("FOR FIND\n");
                            is_if = true;
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
                                break;
                            }
                        }
                        if (var_find) {
                            syntax_lvl = 4;
                            continue;
                        } else {
                            printf("unexpected token '%s'.\n", words[3][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    }else{ // delims
                        printf("FIND '%s'\n", words[atoi(table_number)][atoi(table_val)]);
                    }
                }
                if(syntax_lvl == 4){ // AFTER BEGIN
                    if (strcmp(table_number, "1") == 0) { // delims
                        table_val = delimited_str[1];
                        printf("FIND DELIMITER %s\n", words[1][atoi(table_val)]);
                        if (strcmp(table_val, "0") == 0){
                            printf("EQUATING\n");
                            syntax_lvl = 5;
                        }else if (strcmp(table_val, "6") == 0) { // if
//                            syntax_lvl = 6;
                            syntax_lvl = 7;
                        }else if (strcmp(table_val, "1") == 0){ // ;
                            syntax_lvl = 3;
                        }
                        continue;
                    }else{
                        printf("unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                }
                if(syntax_lvl == 5){ // EQUALING
                    table_val = delimited_str[1];

                    if(strcmp(table_number, "3")==0){ // Var check
                        bool var_find = false;
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
                                printf("VAR %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
                                var_find = true;
                            }
                            if(var_find){
                                syntax_lvl = 5;
                                break;
                            }
                        }
                        if(!var_find){
                            printf("unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                        continue;
                    }
                    if(strcmp(table_number, "2")==0){ // Var value check
                        printf("FIND VALUE IS %s\n", words[atoi(table_number)][atoi(table_val)]);
                        syntax_lvl = 5;
                        continue;
                    }

                    if(strcmp(table_val, "10") == 0){
                        printf("IS MINUS\n");
                    }else if(strcmp(table_val, "12") == 0){
                        printf("IS PLUS\n");
                    }else if(strcmp(table_val, "13") == 0){
                        printf("IS MULTIPLY\n");
                    }else if(strcmp(table_val, "14") == 0){
                        printf("IS DIV\n");
                    }else{
                        syntax_lvl = 3;
                    }
                    continue;
                }
                if(syntax_lvl == 6){ // ENTER IF...OLD VERSION FOR {}: NOT USED
                    table_val = delimited_str[1];
                    if(strcmp(table_val, "7")==0){
                        printf("FIND ENTER IF\n");
                        syntax_lvl = 7;
                        continue;
                    }else{
                        printf("unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                }
                if(syntax_lvl == 7){ // IF
                    table_val = delimited_str[1];
                    if(strcmp(table_number, "3")==0){
                        bool var_find = false;
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
                                printf("VAR %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
                                var_find = true;
                            }
                            if(var_find){
                                break;
                            }
                        }
                        if(!var_find){
                            printf("unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                    }
                    if(strcmp(table_val, "2") == 0){ // condition in if
                        printf("FIND '>'\n");
                    }else if(strcmp(table_val, "4") == 0){
                        printf("FIND '<'\n");
                    }else if(strcmp(table_val, "15") == 0){
                        printf("FIND 'Then'. IF STARTED.\n");
                        syntax_lvl = 3;
                        is_if = true;
                    }
                    continue;
                }

                if(syntax_lvl == 8){ // FOR
                    table_val = delimited_str[1];
                    if(strcmp(table_number, "3")==0){
                        bool var_find = false;
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
                                printf("VAR %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
                                var_find = true;
                            }
                            if(var_find){
                                break;
                            }
                        }
                        if(!var_find){
                            printf("VAR NOT FOUND '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            strcpy(def_vars[vars_count].name, words[atoi(table_number)][atoi(table_val)]);
                            syntax_lvl = 9;
//                            not_error = false;
//                            break;
                        }
                    }
                    else if(strcmp(table_number, "1")==0 && strcmp(table_val, "19")==0){
                        printf("WORD VAL FOUND\n");
                        has_next_token = true;
                        continue;
                    }
                    else if(strcmp(table_number, "2")==0 && has_next_token){
                        printf("VAL '%s' FOUND.\n", words[atoi(table_number)][atoi(table_val)]);
                        has_next_token = true;
                        continue;
                    }
                    else if(strcmp(table_number, "1")==0 && strcmp(table_val, "20")==0 && has_next_token){
                        printf("DO FOUND\n");
                        syntax_lvl = 3;
                    }else{
                        printf("unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                    if(strcmp(table_val, "2") == 0){ // condition in if
                        printf("FIND '>'\n");
                    }else if(strcmp(table_val, "4") == 0){
                        printf("FIND '<'\n");
                    }else if(strcmp(table_val, "15") == 0){
                        printf("FIND 'Then'. IF STARTED.\n");
                        syntax_lvl = 3;
                        is_if = true;
                    }
                    continue;
                }

                if(syntax_lvl == 9){ // do assign for value in "for"
                    if((strcmp(table_number, "1")==0) && (strcmp(table_val, "0")==0)){
                        printf("ASSIGN FIND IN FOR\n");
                        continue;
                    }else if((strcmp(table_number, "2")==0)){
                        printf("VALUE FOUND '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        def_vars[vars_count].value_int = atoi(words[2][atoi(table_val)]);
                        vars_count++;
                        syntax_lvl = 8;
                        continue;
                    }else if((strcmp(table_number, "3")==0)){
                        bool var_find = false;
                        for (int j = 0; j < 1024; j++){
                            if(strcmp(def_vars[j].name, words[3][atoi(table_val)]) == 0){
                                printf("VAR %s FIND. VALUE IS %d\n", def_vars[j].name, def_vars[j].value_int);
                                def_vars[vars_count].value_int = def_vars[j].value_int;
                                printf("VAR %s IN FOR HAS NEW VAL IS %d\n", def_vars[vars_count].name, def_vars[vars_count].value_int);
                                var_find = true;
                                break;
                            }
                        }
                        if(var_find){
                            vars_count++;
                            syntax_lvl = 8;
                        }else{
                            printf("unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                            not_error = false;
                            break;
                        }
                        continue;
                    }else{
                        printf("unexpected token '%s'.\n", words[atoi(table_number)][atoi(table_val)]);
                        not_error = false;
                        break;
                    }
                }
            }
        }
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
    int i=0;

    for (int i = 0; *(chars + i); i++) {
        for (int del = 0; del <= 25; del++) {

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
                        if (isNumber(tmp) || tmp[0] == '"') {
                            strcpy(words[2][vars], tmp);
                            printf("[%d][%d] Value: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);

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
        for (int type = 0; type < 3; type++) {
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

file_write("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt", map);
bool syntax_next = syntax_check(map);
    if(syntax_next){
        code_work(map);
    }
}


int main() {
    char code[] =
            "program var\n"
            "a:int;\n"
            //                  "char my assign \"2\";\n"
            "self:float;\n"
            "kill:int;\n"
//            "kill2:int;\n"
            //                  "int kill assign 2;\n"
            "z:int;\n"
            "y:int;\n"
//            "a:int;\n"
            "begin\n"
            ";a assign 10"
            ";z assign 10"
            ";y assign 10"
            ";kill assign 10"
            ";if y>z then\n"
            ";y assign kill add y\n"
            ";z assign y*y add z disa y\n"
            "end\n"
            //"for i assign z val 10 do y assign 10 add z;\n"
            //"while z val 10 do y assign 10 add z next;\n"
            //                  "y++;\n"
            //                  "print(z);"
            "\\0";

    removeSpacesAndNewlines(code);
    code_check_file_write(code);

    return 0;
}
