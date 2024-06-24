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
