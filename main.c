#include <stdio.h>
#include <ctype.h>
#include <string.h>

int vars = 10;
char words[4][1024][1024] = {{
                                     {'c', 'h', 'a', 'r', '\0'},
                                     {'i', 'n', 't', '\0'},
                                     {'f', 'l', 'o', 'a', 't', '\0'}
                             },
                             {
                                     {'='},
                                     {';'},
                                     {'>'},
                                     {' '},
                             },
                             {
                                     {'a', 'a'},

                             }
};

void code_check(const char chars[300]) {
    char tmp[100];
    memset(tmp, '\0', sizeof(tmp));
    int j = 0;
    for (int i = 0; *(chars + i); i++) {
        for (int del = 0; del < 4; del++) {
            if (chars[i] == *words[1][del]) {
                bool b = false;
                for (int k = 0; 1; k++) {
                    for (int kk = 0; kk < 1024; kk++) {
                        if (strcmp(words[k][kk], tmp) == 0) {
                            printf("[%d][%d] Type: '%s' | tmp: %s \n", k, kk, words[k][kk], tmp);
                            b = true;
                            break;
                        }
                    }
                    if (b) {
                        break;
                    } else {
                        strcpy(words[2][vars], tmp);
                        printf("[%d][%d] Var: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);
                        vars++;
                        break;
                    }
                }
                memset(tmp, '\0', sizeof(tmp));
                j = 0;
                printf("[%d][%d] Delimiter: '%s' | tmp: %s \n", i, del, words[1][del], tmp);
                i++;
                break;
            }
        }
        tmp[j] = chars[i];
        j++;
    }
}

int main() {
    char code[] = {
            'i', 'n', 't', ' ', 'x', '=', '1', ';',
            'c', 'h', 'a', 'r', ' ', 'x', '=', '1', ';',
            'f', 'l', 'o', 'a', 't', ' ', 'y', '=', '5', ';',
            'i', 'n', 't', ' ', 'z', '=', '6', ';'
    };
    int length = sizeof(code) / sizeof(code[0]);

    code_check(code);

    return 0;
}
