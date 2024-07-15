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
            vars[count_vars].value = atoi(value);
            count_vars++;
            free(*(tokens + i));
        }
    }

    char **test = str_split(vars_place[1], ';');
    for (int i = 0; *(test + i); i++) {
        if (strcmp(test[i], "@") != 0) {

            char **check = str_split(test[i], ' ');
            for (int l = 0; *(check + l); l++) {
                if ((strcmp(check[l], "+=") == 0)) {
                    bool find = false;
                    for (int j = 0; j < count_vars; j++) {
                        if (strcmp(vars[j].name, check[l - 1]) == 0) {
                            find = true;

                            for (int y = 0; *(check + y); y++) {
                                if (strcmp(check[y], "+") == 0) {
                                    for (int o = 0; o < count_vars; o++) {
                                        for (int k = 0; *(check + k); k++) {
                                            if (strcmp(vars[j].name, check[k]) == 0) {
                                                int tmp = 0;
                                                *check[k] = snprintf(check[k], sizeof(check[k]), "%d", tmp);
                                            }
                                        }
                                    }

                                    vars[j].value += atoi(check[y - 1]) + atoi(check[y + 1]);
                                    *check[y - 1] = '0';
                                    *check[y + 1] = '0';
                                    printf("\nThe integer value of %s is %d ", vars[j].name, vars[j].value);
                                }
                            }

                            break;
                        }
                    }
                    if (!find) {
                        printf("\n%s", "'");
                        printf("%s", check[l - 1]);
                        printf("'not found in %d line", i);
                    }
                }
            }
        } else {
            break;
        }
    }
    return 0;
}

int main() {

    char code[] = "int x = 100;"
                  "int y = 2;"
                  "~"
                  "x += 1 + 2 + x;"
                  "y += x + x;"
                  "print(j);"
                  "@;"
                  "!";
    parser(code);
    return 0;
}