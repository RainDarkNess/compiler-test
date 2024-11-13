#include <stdio.h>

int main() {
    char buffer[128];

    printf("Введите строку: ");
    fgets(buffer, sizeof(buffer), stdin);

    printf("Вы ввели: %s", buffer);

    return 0;
}
