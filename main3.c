#include <stdio.h>

int main() {
    char buffer[128];

    printf("������� ������: ");
    fgets(buffer, sizeof(buffer), stdin);

    printf("�� �����: %s", buffer);

    return 0;
}
