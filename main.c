#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int vars = 0;
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
                                     {112},

                             },
                             {
                                     {'a', 'a'},

                             }
};

char map[1024];

// NOT MINE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void file_write(const char *filepath, const char *data)
{
    FILE *fp = fopen(filepath, "ab");
    if (fp != NULL)
    {
        fputs(data, fp);
        fclose(fp);
    }
}

bool isNumber(char const* const text) {
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

void code_check_file_write(const char chars[300]) {
    char tmp[100];
    char tmp_str[1024];
    memset(map, '\0', sizeof(map));
    memset(tmp, '\0', sizeof(tmp));
    int j = 0;
    for (int i = 0; *(chars + i); i++) {
        for (int del = 0; del < 4; del++) {
            if (chars[i] == *words[1][del]) {
                bool b = false;
                while(true){
                    for (int kk = 0; kk < 1024; kk++) {
                        if (strcmp(words[0][kk], tmp) == 0) {
                            printf("[%d][%d] Type: '%s' | tmp: %s \n", 0, kk, words[0][kk], tmp);

                            sprintf(tmp_str, "[%d][%d];",  0, kk);
                            strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);
                            memset(tmp_str, '\0', sizeof(tmp_str));

                            b = true;
                            break;
                        }
                    }
                    if (b) {
                        break;
                    } else {
                        if(isNumber(tmp)){
                            strcpy(words[2][vars], tmp);
                            printf("[%d][%d] Int: '%s' | tmp: %s \n", 2, vars, words[2][vars], tmp);

                            sprintf(tmp_str, "[%d][%d];", 2, vars);
                            strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);

                            memset(tmp_str, '\0', sizeof(tmp_str));

                            vars++;
                        }else{
                            strcpy(words[3][vars], tmp);
                            printf("[%d][%d] Var: '%s' | tmp: %s \n", 3, vars, words[3][vars], tmp);

                            sprintf(tmp_str, "[%d][%d];", 3, vars);
                            strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);

                            memset(tmp_str, '\0', sizeof(tmp_str));
                        }

                    }
                    break;
                }
                j = 0;
                printf("[%d][%d] Delimiter: '%s' | tmp: %s \n", 1, del, words[1][del], tmp);

                sprintf(tmp_str, "[%d][%d];", 1, del);
                strncat(map, tmp_str, sizeof(map) - strlen(map) - 1);

                memset(tmp_str, '\0', sizeof(tmp_str));

                memset(tmp, '\0', sizeof(tmp));
                i++;
                break;
            }
        }
        tmp[j] = chars[i];
        j++;
    }
    file_write("C:\\Users\\rain\\CLionProjects\\CTest\\1.txt", map);
    printf("%s", map);
}

int main() {
    char code[] = {
            'i', 'n', 't', ' ', 'x', '=', '1', ';',
            'c', 'h', 'a', 'r', ' ', 'x', '=', '1', ';',
            'f', 'l', 'o', 'a', 't', ' ', 'y', '=', '5', ';',
            'i', 'n', 't', ' ', 'z', '=', '6', ';'
    };

    code_check_file_write(code);

    return 0;
}
