#include <stdio.h>
#include<stdlib.h>

unsigned short matrix[256][256]; //creates matrix short size
unsigned short vector[256]; //creates array of short size

int main(){
    for(unsigned short i=0; i<256; i++){
        for(unsigned short j=0; j<256; j++){
            matrix[i][j] = j+i*256;}}

    for(unsigned short i=0; i<256; i++){ vector[i] = i; }


    FILE *f = fopen("matrix.data", "wb");
    fwrite(matrix, sizeof(unsigned short), sizeof(matrix), f);
    fclose(f);

    f = fopen("vector.data", "wb");
    fwrite(vector, sizeof(unsigned short), sizeof(vector), f);
    fclose(f);

    //prints matrix
    for(unsigned short i=0; i<256; i++){
        for(unsigned short j=0; j<256; j++){
            printf("%i ",matrix[i][j]);}
            printf("\n\n");}
    //prints array
    for(unsigned short i=0; i<256; i++){ printf("%i ",vector[i]);}


    return 0;
}