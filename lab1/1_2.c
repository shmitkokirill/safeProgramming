#include <stdio.h>
#include <stdlib.h>

const char *menu_ =
    "Type -1 to exit program\n\n1.Create/Open file\n2.Append string\n3.Read file\n4.Delete file\n";
enum
{
    EXIT = -1,
    CR_F = 1,
    AP_F,
    RD_F,
    D_F
};

void clear_scr()
{
    system("@cls||clear");
}

void flush_input()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF)
		;
}

void wait_for_()
{
    int close = 0;
    printf("\n\nType 1 to exit main menu:\n");
    while (close != 1)
        scanf("%d", &close);
}

int main()
{
    FILE *cur_f_ptr = NULL;
    int input = 1;
    char file_nm[20] = "";
    int c = 0, rm_f = 0;
    while (input)
    {
        clear_scr();
        printf("%s", menu_);
        if (strlen(file_nm) != 0)
            printf("\nName of opened file is %s\n", file_nm);
        scanf("%d", &input);
        switch (input)
        {
        case EXIT:
            return 0;
        case CR_F:
            printf("Enter the file name:\n");
            scanf("%s", file_nm);
            cur_f_ptr = fopen(file_nm, "w");
            if (!cur_f_ptr)
            {
                perror(file_nm);
                wait_for_();
                break;
            }
            fclose(cur_f_ptr);
            break;
        case AP_F:
            if (strlen(file_nm) == 0)
            {
                printf("File not found.\n");
                wait_for_();
                break;
            }
            printf("Enter the string:\n");
            cur_f_ptr = fopen(file_nm, "a");
            if (!cur_f_ptr)
            {
                perror(file_nm);
                wait_for_();
                break;
            }
            flush_input();
            while ((c = getchar()) != EOF)
            {
                fputc(c, cur_f_ptr);
                if (c == '\n')
                    break;
            }
            fclose(cur_f_ptr);
            break;
        case RD_F:
            if (strlen(file_nm) == 0)
            {
                printf("File not found.\n");
                wait_for_();
                break;
            }
            cur_f_ptr = fopen(file_nm, "r");
            if (!cur_f_ptr)
            {
                perror(file_nm);
                wait_for_();
                break;
            }
            printf("\nFile %s:\n", file_nm);
            while ((c = getc(cur_f_ptr)) != EOF)
                putchar(c);
            fclose(cur_f_ptr);
            wait_for_();
            break;
        case D_F:
            if (strlen(file_nm) == 0)
            {
                printf("File not found.\n");
                wait_for_();
                break;
            }
            rm_f = remove(file_nm);
            if (rm_f == -1)
            {
                perror(file_nm);
                wait_for_();
                break;
            }
            wait_for_();
            file_nm[0] = '\0';
            break;
        default:
            break;
        }
    }
}