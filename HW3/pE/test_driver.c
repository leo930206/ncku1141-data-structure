#include <stdio.h>
#include <stdlib.h>

typedef struct Data {
    int key;
    int value;
} Data;

void insert(Data *data);
void remove(int key);
int search(int key);

int main() {
    int command_count;
    if (scanf("%d", &command_count) != 1) return 0;

    for (int i = 0; i < command_count; i++) {
        char command[8];
        scanf("%s", command);
        if (command[0] == 'i') {
            Data *data = malloc(sizeof(Data));
            scanf("%d %d", &data->key, &data->value);
            insert(data);
        } else if (command[0] == 'd') {
            int key;
            scanf("%d", &key);
            remove(key);
        } else if (command[0] == 's') {
            int key;
            scanf("%d", &key);
            printf("%d\n", search(key));
        }
    }

    return 0;
}
