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

    int b_count = 0;//count # of bytes
    int jpg = 0; //get nameing convention ready

    fseek(file, 0, SEEK_END);// seek to end of file
    b_count = ftell(file);// get current file pointer
    fseek(file, 0, SEEK_SET);//go back to beginning of file
    char jpg_name[8] = "";


    BYTE mem[b_count];//temp location for the file in the stack
    BYTE mem_2[b_count];
    
    
    
    
    fread(mem, sizeof(BYTE),b_count,file);
    fseek(file, 0, SEEK_SET);
    int seek_start = 0;
    int file_size;

    for (int i = 0; i < b_count; i++)
    {
       mem_2[i] = mem[i]; 
    }
    
    for (int i = 0; i < b_count; i++, seek_start++)
    {
        
        file_size = 0;
        if (mem[i] == 0xff && mem[i + 1] == 0xd8 && mem[i + 2] == 0xff && (mem[i + 3] & 0xf0) == 0xe0)
        {
            if (jpg < 10)
            {
                sprintf(jpg_name, "00%i.jpg", jpg);
               // printf("%s\n", jpg_name); 
            }
            
            else if (jpg > 9)
            {
                sprintf(jpg_name, "0%i.jpg", jpg);
                //printf("%s\n", jpg_name);
            }
            
            to_write = fopen(jpg_name, "a");//open a new file to write too
            
            int j = i;
            while (!(mem_2[j + 1] == 0xff && mem_2[j + 2] == 0xd8 && mem_2[j + 3] == 0xff && (mem_2[j + 4] & 0xf0) == 0xe0))
            
            {
                 
                 fwrite(&mem[j], sizeof(BYTE),1, to_write);
                 file_size++;
                 j++;
            }
            
            //printf("File Size:%i\n", file_size);
            fclose(to_write);//CLOSE JPEG
            jpg++; //advance jpg naming counter
            //printf("%x %x %x %x \n", mem[i], mem[i+1], mem[i+2], mem[i+3]);
        }
    }

    fclose(file);
    printf("BYTES: %i\njpgs: %i\n", b_count, jpg);
}
