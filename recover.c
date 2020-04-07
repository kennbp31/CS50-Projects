#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2) //check that a command line argument was provided
        {
            printf("missing command-line argument\n");
            return 1;
        }

    typedef uint8_t BYTE;
    FILE *file = fopen(argv[1], "r");
    FILE *to_write;//temp location for each jpgs data
    
    if (file == NULL)
    {
        fclose(file);
        printf("Unable to open file\n");
        return 1;
    }

    int b_count = 0;//count # of bytes
    int jpg = 0; //get nameing convention ready

    fseek(file, 0, SEEK_END);// seek to end of file
    b_count = ftell(file);// get current file pointer
    fseek(file, 0, SEEK_SET);//go back to beginning of file
    char jpg_name[8] = "000.jpg";

    BYTE mem[b_count];
    //mem = (BYTE*) malloc(b_count);//temp location for the file in the stack
    
    fread(mem, sizeof(BYTE), b_count, file);
    fclose(file);
    int j, l;
    for (int i = 0; i < b_count; i = i + 512)
    {
        if (mem[i] == 0xff && mem[i + 1] == 0xd8 && mem[i + 2] == 0xff && (mem[i + 3] & 0xf0) == 0xe0)
        {
            if (jpg < 10)
            {
                sprintf(jpg_name, "00%i.jpg", jpg);
            }
            
            else if (jpg > 9)
            {
                sprintf(jpg_name, "0%i.jpg", jpg);
            }
            
            to_write = fopen(jpg_name, "a");//open a new file to write too
            j = i + 512;
            while(!(mem[j] == 0xff && mem[j + 1] == 0xd8 && mem[j + 2] == 0xff && (mem[j + 3] & 0xf0) == 0xe0) && j + 512 < b_count)
            {
                l = j - 512;
                for (int k = l; k < j; k++)
                {
                  fwrite(&mem[k], sizeof(BYTE), 1, to_write);  
                }
               
                j = j + 512;
                // fputc(mem[j],to_write);
            }
            
            
        
            //printf("File Size:%i\n", file_size);
            //CLOSE JPEG
            jpg++; //advance jpg naming counter
            //printf("%x %x %x %x \n", mem[i], mem[i+1], mem[i+2], mem[i+3]);
        }
    }
    //free(mem);
    fclose(to_write);
}
