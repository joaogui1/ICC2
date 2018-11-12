#include <stdio.h>

int main(){
    FILE *fpw;
    int is_new, number = 0;
    char pixels[512], file[10];
    char begin[3] = {0xff, 0xd8, 0xff};
    while(scanf("%c%c%c", &pixels[0], &pixels[1], &pixels[2]) > 2){ //do the loop while there's still something to be read
        scanf("%c", &pixels[3]);
        is_new = (begin[0] == pixels[0]) && (begin[1] == pixels[1]) && (begin[2] == pixels[2]); // checks if the first 3 bytes have the right values
        is_new = is_new && ((pixels[3]&0xf0) == 0xe0);//checks if the next 4 bits are 1110
        for(int i = 4; i < 512; ++i)    scanf("%c", &pixels[i]);
        //if there's a new image
        if(is_new){
            sprintf(file, "%03d.jpg", number);
            if(number > 0)  fclose(fpw);
            ++number;
            fpw = fopen(file, "w+");
            for(int i = 0; i < 512; ++i)    fprintf(fpw, "%c", pixels[i]);
        }

        //if there's no new image but we were already processing an older one
        else if(number > 0){
            for(int i = 0; i < 512; ++i)    fprintf(fpw, "%c", pixels[i]);
        }


    }

    //in case no image was found
    if(number == 0) printf("Could not find pictures\n");
    //else, close the last file
    else    fclose(fpw);

    return 0;
}