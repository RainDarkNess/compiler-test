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
                                     {'c', 'h', 'a', 'r', '\0'},
                                     {'i', 'n', 't', '\0'},
                                     {'f', 'l', 'o', 'a', 't', '\0'}
                             },
                             { // delimiters
                                     {'='},
                                     {';'},
                                     {'>'},
                                     {' '},
                                     {'+'},
                                     {'b', 'e', 'g', 'i', 'n',},
                                     {'i','f'},
                                     {'('},
                                     {')'},
                             },
                             { // vars
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool syntax_check(char _map[]) {
    char **tokens = str_split(_map, ';');
    int syntax_lvl = 0;
    char type_var[10];
    bool not_error = true;
    bool var_declaration = false;
    bool var_replacing = false;
    char tmp_var_name[1024];

    for (int i = 0; *(tokens + i); i++) {
        if (not_error) {

            char **delimited_str = str_split(tokens[i], ',');
            char *table_number = delimited_str[0];

            if (strcmp(table_number, "0") == 0) {
                char *var_type = delimited_str[1];
                printf("TYPE VAR IS:%s\n", words[0][atoi(var_type)]);
                strcpy(type_var, words[0][atoi(var_type)]);
                syntax_lvl = 1;
                var_declaration = true;
            }
            if (var_declaration) {

                if (syntax_lvl == 1) {
                    char *var_number = delimited_str[1];
                    if (strcmp(table_number, "3") == 0) {
                        printf("NAME VAR IS:%s\n", words[3][atoi(var_number)]);
                        strcpy(tmp_var_name, words[3][atoi(var_number)]);

                        for (int name = 0; name < 1024; name++) {
                            if (strcmp(def_vars[name].name, words[3][atoi(var_number)]) == 0) {
                                vars_count_replace = vars_count;
                                var_replacing = true;
                                vars_count = name;
                            }
                        }
                        strcpy(def_vars[vars_count].name, words[3][atoi(var_number)]);

                        int pr_len = strlen(prohibited);
                        for (int j = 0; j < pr_len; j++) {
                            if (strchr(words[3][atoi(var_number)], prohibited[j]) != NULL) {
                                printf("syntax error\n");
                                printf("Name '%s' not allowed!\n", words[3][atoi(var_number)]);
                                not_error = false;
                                break;
                            }
                        }
                        syntax_lvl = 2;
                        i++;
                        continue;
                    }
                } else if (syntax_lvl == 2) {
                    char *var_val = delimited_str[1];
                    if (strcmp(table_number, "1") == 0) {
                        char *del = delimited_str[1];
                        if ((strcmp(words[1][atoi(del)], "=") == 0)) {
                            i++;
                            continue;
                        } else {
                            printf("Has not '='\n");
                            not_error = false;
                            break;
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
        }
    }
    return false;
}

void code_check_file_write(const char chars[300]) {
    char tmp[100];
    char tmp_str[1024];
    memset(map, '\0', sizeof(map));
    memset(tmp, '\0', sizeof(tmp));
    int j = 0;

    bool delimiter_detected = true;
    bool is_del = false;

    for (int i = 0; *(chars + i); i++) {
        for (int del = 0; del <= 8; del++) {
            for (int sub_del = 0; *(words[1][del] + sub_del); sub_del++) {

                if(chars[i] == words[1][del][0]){
                    is_del = true;
                }
                if (chars[i] == words[1][del][sub_del] && is_del) {
                    if (words[1][del][sub_del + 1] == '\0' && !delimiter_detected) {
                        delimiter_detected = true;
                        i++;
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
                    for (int kk = 0; kk < 1024; kk++) {
                        if (strcmp(words[0][kk], tmp) == 0) {
                            printf("[%d][%d] Type: '%s' | tmp: %s \n", 0, kk, words[0][kk], tmp);

                            sprintf(tmp_str, file_view, 0, kk);
                            strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);
                            memset(tmp_str, '\0', sizeof(tmp_str));

                            b = true;
                            break;
                        }
                    }
                    if (b) {
                        break;
                    } else {
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

                    }
                    break;
                }
                j = 0;
                printf("[%d][%d] Delimiter: '%s' | tmp: %s \n", 1, del, words[1][del], tmp);

                sprintf(tmp_str, file_view, 1, del);
                strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);

                memset(tmp_str, '\0', sizeof(tmp_str));

                memset(tmp, '\0', sizeof(tmp));
                delimiter_detected = false;
                is_del = false;
                break;
            }
        }
    }
    tmp[j] = chars[i];
    j++;
}

file_write("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt", map);
syntax_check(map);
}


int main() {
    char code[] = {
            'i', 'n', 't', ' ', 'k', 'i', 'l', 'l', '=', '1', ';',
            'c', 'h', 'a', 'r', ' ', 'm', 'y', '=', '"', '2', '"', ';',
            'f', 'l', 'o', 'a', 't', ' ', 's', 'e', 'l', 'f', '=', '3', ';',
            'i', 'n', 't', ' ', 'k', 'i', 'l', 'l', '=', '2', ';',
            'i', 'n', 't', ' ', 'z', '=', '9', ';',
            'i', 'n', 't', ' ', 'y', '=', 'z', ';',
            'i', 'n', 't', ' ', 'a', '=', 'y', ';',
            'b', 'e', 'g', 'i', 'n',
            'i','f',
            'a', '=', 'z', '+', 'y', ';', '\0'
    };

    code_check_file_write(code);

    return 0;
}
