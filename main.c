//
// Created by rain on 19.05.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct int_value {
    char name[1024];
    int value;
};

char **str_split(char *a_str, const char a_delim) {
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

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

int parser(char *code) {
    int count_vars = 0;
    int vars_size = 1024;
    struct int_value vars[vars_size];
    // tmp REPLACE
    char **vars_place = str_split(code, '~');

    char **tokens = str_split(vars_place[0], ';');
    for (int i = 0; *(tokens + i); i++) {
        if (strcmp(tokens[i], "!") != 0) {
            char **format_str = str_split(tokens[i], ' ');

            char *format = format_str[0];
            char *name = format_str[1];
            char *value = format_str[3];
            strcpy(vars[count_vars].name, name);
            vars[count_vars].value = (int) *value;
            count_vars++;
//            printf("format = %s\n", format);
//            printf("name = %s\n", name);
//            printf("value = %s\n\n", value);
            free(*(tokens + i));
        }
    }

    char **test = str_split(vars_place[1], ';');
    for (int i = 0; *(test + i); i++) {
        if (strcmp(test[i], "@") != 0) {

            char **check = str_split(test[i], ' ');
            printf("%s\n", check[1]);
            if (strcmp(check[1], "=") != 0) {
//                for(int j = 0; j < count_vars; j++){
//                    char *res = strstr(check[0], vars[j].name);
//                    printf("%s", res);
//                }
            }

        }
    }

//    printf("%s\n", vars[0].name);
//    printf("%s\n", vars[1].name);
    return 0;
}

int main() {

    char code[] = "int x = 1;"
                  "int y = 2;"
                  "~"
                  "x = x + y;"
                  "print(j);"
                  "@;"
                  "!";
    parser(code);
    return 0;
}