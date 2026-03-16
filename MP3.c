#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

struct ID3v2
{
    char id[3];
    unsigned char version;
    unsigned char revison;
    unsigned char flags;
    unsigned char size[4];
};
struct headerfile
{
    unsigned char frame_id[4];
    unsigned char flags[2];
    unsigned char size[4];
};

void view_tag(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Error opening file\n");
        return;
    }

    fseek(fp, 10, SEEK_SET);
    printf("MP3 Information:\n");

    while (1)
    {
        char frame_id[5] = {0};
        unsigned char size_byte[4];
        unsigned char flags[2];
        int frame_size;

        if (fread(frame_id, 1, 4, fp) != 4)
            break;
        if (frame_id[0] == 0)
            break;
        if (fread(size_byte, 1, 4, fp) != 4)
            break;
        if (fread(flags, 1, 2, fp) != 2)
            break;

        frame_size = (size_byte[0] << 21) | (size_byte[1] << 14) | (size_byte[2] << 7) | (size_byte[3]);
        if (frame_size <= 0)
            break;

        char *data = (char *)malloc(frame_size + 1);
        fread(data, 1, frame_size, fp);
        data[frame_size] = '\0';
        char *text = data;
        if (frame_size > 1)
            text = data + 1;

        if (strcmp(frame_id, "TIT2") == 0)
            printf("Title: %s\n", text);
        else if (strcmp(frame_id, "TPE1") == 0)
            printf("Artist: %s\n", text);
        else if (strcmp(frame_id, "TALB") == 0)
            printf("Album: %s\n", text);
        else if (strcmp(frame_id, "TYER") == 0 || strcmp(frame_id, "TRDC") == 0)
            printf("Year: %s\n", text);
        else if (strcmp(frame_id, "TCON") == 0)
            printf("Genre: %s\n", text);
        else if (strcmp(frame_id, "TCOM") == 0)
            printf("Composer: %s\n", text);
        else if (strcmp(frame_id, "COMM") == 0)
            printf("Comment: %s\n", text);

        free(data);
    }

    fclose(fp);
}

void edit_tag(const char *filename)
{
    FILE *fp = fopen(filename, "r+b");
    if (!fp)
    {
        printf("Error opening file\n");
        return;
    }

    char tag[5], new_value[200];
    printf("Enter tag (TIT2, TPE1, TALB, TYER, TCON, TCOM, COMM): ");
    scanf("%s", tag);
    printf("Enter new value: ");
    getchar();
    fgets(new_value, 200, stdin);
    new_value[strcspn(new_value, "\n")] = 0;

    fseek(fp, 10, SEEK_SET);

    while (1)
    {
        long pos = ftell(fp);
        char frame_id[5] = {0};
        unsigned char size_byte[4];
        unsigned char flags[2];
        int frame_size;

        if (fread(frame_id, 1, 4, fp) != 4)
            break;
        if (frame_id[0] == 0)
            break;
        if (fread(size_byte, 1, 4, fp) != 4)
            break;
        if (fread(flags, 1, 2, fp) != 2)
            break;

        frame_size = (size_byte[0] << 21) | (size_byte[1] << 14) | (size_byte[2] << 7) | (size_byte[3]);
        if (frame_size <= 0)
            break;

        if (strcmp(frame_id, tag) == 0)
        {
            long data_pos = ftell(fp);
            fseek(fp, data_pos, SEEK_SET);
            fputc(0, fp);
            fwrite(new_value, 1, strlen(new_value), fp);
            printf("Tag updated successfully.\n");
            fclose(fp);
            return;
        }

        fseek(fp, frame_size, SEEK_CUR);
    }

    fclose(fp);
    printf("Tag not found.\n");
}

int main()
{
    char filename[100];
    int option;

    printf("Enter MP3 file name: ");
    scanf("%s", filename);

    do
    {
        printf("\n1. View Tag Info\n");
        printf("2. Edit Tag Info\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option == 1)
            view_tag(filename);
        else if (option == 2)
            edit_tag(filename);
        else if (option == 3)
            exit(0);
        else
            printf("Invalid choice\n");
    } while (option != 3);

    return 0;
}