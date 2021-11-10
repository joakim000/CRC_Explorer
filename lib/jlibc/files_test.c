#include "common.h"
#include "files.h"

void main(int argc, char* argv[] ) {
    uint8_t read_error;
    char* text;

    if (argc > 1) {
        text = ReadTextFromFile(argv[1], 0x100000, true, &read_error);
    }
    else {
         // text = ReadTextFromFile("../assets/lorem32k.asc", 0x100000, true, &read_error);
        text = ReadTextFromFile("test.txt", 0x100000, true, &read_error);
    } 

   


    printf("Error code:%d\n", read_error);

    if (read_error == 0) {
    printf("  Text len:%d\n", strlen(text));
    // printf("  Text len:%d\n", strlen(text));


    printf("Message|%s|\n", text);

    printf("Message bytes:\n", text);
    for I2(strlen(text) + 1)
        printf("%d ", text[i]);
    puts("");
    }


}