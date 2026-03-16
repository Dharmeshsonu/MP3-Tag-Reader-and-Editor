#include<stdio.h>
#include<string.h>

int main()
{
    char filename[100];
    printf("Enter the MP3 filename : ");
    scanf("%[^\n]",filename);

    int size = strlen(filename);

    if (size < 4)
    {
        printf("Invalid filename\n");
        return 0;
    }
    
    

    if (filename[size-4] != '.' || filename[size-3] != 'm' || filename[size-2] != 'p' || filename[size-1] != '3')
    {
        printf("Invalid file.Please provide an .mp3 file\n");
        return 0;
    }

    else
    {
       FILE *fptr = fopen(filename,"rb");
       if (fptr == NULL)    
       {
            printf("Unable to open the file %s\n",filename);
            return 0;
       }
       else
       {
            printf("File opened successfully\n");
       }

       fclose(fptr);
   
       printf("Choose an option: ");
       printf("\n1.View MP3 file\n2.Edit MP3 file\n3.Exit\n");
       printf("Enter your choice: ");
       int choice;
       scanf("%d",&choice);
       switch (choice)
       {
        case 1:
        {
        printf("1.View MP3 file\n");

        FILE *fptr = fopen(filename,"rb");
        if (fptr == NULL)
        {
            printf("Unable to open the file\n");
            return 0;
        }

        unsigned char header[10];
        fread(header, 1, 10, fptr);
       

        if (header[0] == 'I' && header[1] == 'D' && header[2] == '3')
        {
            printf("This file has ID3v2 tags (good MP3)\n");
        }
        else
        {
            printf(" This is NOT an MP3 with ID3v2 tags\n");
        }

        fseek(fptr, 10, SEEK_SET);

      
        
        while(1)
        {
            char frame_id[5];
            if(fread(frame_id,1,4,fptr) != 4 )
            break;
            frame_id[4] = '\0';

            if (frame_id[0] < 'A' || frame_id[0] > 'Z')
            break;


            unsigned char size_bytes[4];
            fread(size_bytes,1,4,fptr);
            int frame_size = (size_bytes[0] << 24 | (size_bytes[1] << 16) | (size_bytes[2] << 8) | (size_bytes[3]));


            if (frame_size <= 0 || frame_size > 5000)
            break;


            fseek(fptr, 2, SEEK_CUR);

            unsigned char frame_data[frame_size + 1];
            fread(frame_data, 1 , frame_size, fptr);
            frame_data[frame_size] = '\0';


           unsigned char encoding = frame_data[0];
           char *text;

           if (encoding == 0x01)
           {
            text = (char *)(frame_data + 3);
           }
           else
           {
            text = (char *)(frame_data + 1);        
           }
           

            char temp[500] = {0};
            if (encoding == 0x01)
            {
                int j = 0;
                for (int i = 0; i < frame_size - 3; i += 2)    
                {
                    temp[j++] = text[i];
                }
                temp[j] = '\0';
                text = temp;
            }


           if (strcmp(frame_id,"TIT2") == 0)    
           {
            printf("Title: %s\n",text);
           }
           else if (strcmp(frame_id,"TPE1") == 0)
           {
            printf("Artist: %s\n",text);
           }
            else if (strcmp(frame_id,"TALB") == 0)
            {
            printf("Album: %s\n",text);
            }
            else if (strcmp(frame_id,"TYER") == 0)
            {
            printf("Year: %s\n",text);
            }
            else if (strcmp(frame_id,"TCON") == 0)
            {
            printf("Genre: %s\n",text);
            }
        
            
        }

        fclose(fptr);

        
        break;
    }
       

    case 2: 
    {
        printf("2.Edit MP3 file\n");

        FILE *fptr1 = fopen(filename,"rb");
        FILE *fptr2 = fopen("new.mp3","wb");

        if (fptr1 == NULL)
        {
            printf("Unable to open the file\n");
            return 0;
        }
    
        unsigned char header[10];

        fread(header,1,10,fptr1);
        fwrite(header,1,10,fptr2);

        while (1)
        {
            char frame_id[5] = {0};                        // buffer to read frame id //
            unsigned char size_bytes[4];             // buffer to read size bytes // 
            unsigned char flags[2];                  // buffer to read flags //

            if (fread(frame_id,1,4,fptr1) != 4)
            {
                break;
            }

            if (frame_id[0] == 0)
            {   
                break;
            }
        
        

            //fread(frame_id,1,4,fptr1);      // reading frame id //
            fread(size_bytes,1,4,fptr1);    // reading size bytes //    
            fread(flags,1,2,fptr1);         // reading flags //

            int frame_size = (size_bytes[0] << 24 | (size_bytes[1] << 16) | (size_bytes[2] << 8) | (size_bytes[3]));

            unsigned char frame_data[frame_size];   // using a buffer to read frame data //

            fread(frame_data,1,frame_size,fptr1);    // reading frame data from original file //

            char new_text[100];

            int edited = 0;

            if (strcmp(frame_id ,"TIT2") == 0)
            {
                printf("Enter new title: ");
                scanf(" %[^\n]",new_text);
                edited = 1;
            }
            else if (strcmp(frame_id ,"TPE1") == 0)
            {
                printf("Enter new artist: ");
                scanf(" %[^\n]",new_text);
                edited = 1;
            }
            else if (strcmp(frame_id , "TALB") == 0)
            {
                printf("Enter new album: ");
                scanf(" %[^\n]",new_text);
                edited = 1;
            }
            else if (strcmp(frame_id , "TYER") == 0)
            {
                printf("Enter new year: ");
                scanf(" %[^\n]",new_text);
                edited = 1;
            }
            else if (strcmp(frame_id , "TCON") == 0)
            {
                printf("Enter new genre: ");
                scanf(" %[^\n]",new_text);
                edited = 1;
            }
        

            if(edited)
            {
                unsigned char encoding = 0x00;   // using UTF-8 encoding //
                int new_size = strlen(new_text) + 1;     // +1 for encoding byte  //
        
                size_bytes[0] = (new_size >> 24) & 0xFF;
                size_bytes[1] = (new_size >> 16) & 0xFF;
                size_bytes[2] = (new_size >> 8) & 0xFF;
                size_bytes[3] = new_size & 0xFF;


                fwrite(frame_id,1,4,fptr2);        // writing frame id to new file //
                fwrite(size_bytes,1,4,fptr2);      // writing updated size bytes to new file //
                fwrite(flags,1,2,fptr2);           // writing flags to new file // 
                fwrite(&encoding,1,1,fptr2);       // writing encoding byte to new file //
                fwrite(new_text,1,strlen(new_text),fptr2);   // writing new text to new file //

            }
            
            else
            {
                fwrite(frame_id,1,4,fptr2);        // writing frame id to new file //
                fwrite(size_bytes,1,4,fptr2);      // writing size bytes to new file //
                fwrite(flags,1,2,fptr2);           // writing flags to new file //  
                fwrite(frame_data,1,frame_size,fptr2);   // writing frame data to new file //
            }


        }

        unsigned char buffer[1024];
        size_t bytes;
        while ((bytes = fread(buffer,1, sizeof(buffer),fptr1)) > 0)
        {
            fwrite(buffer,1,bytes,fptr2);
        }

        fclose(fptr1);
        fclose(fptr2);

        printf("MP3 file updated successfully (saved as new.mp3)\n");

        break;
        
        case 3:
        printf("Exiting program.\n");
        return 0;


    }

    
    

  }
    
 
}


}



