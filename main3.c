#include "stdio.h"
int main(){
    int y = 100;
    int af2w = 0;
    for(int i = 1; i < 10; i++){
        af2w += y + 2;
    }
    printf("%d\n", af2w);
    return 0;
}