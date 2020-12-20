#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READFILECONTENTSIZE 2000

char *justComment[255];
char *rightSideOfEqual[255];
char *leftSideOfEqual[255];
char *newLine[255];

int lineNumber        = 0;
int commentLineNumber = 0;
int confNumber        = 0;

const char delim[]    = "\n";
const char equal[]    = "=";
const char comment[]  = "#";

static char *readcontent(const char *filename)
{
    char *fcontent = NULL, c;
    int index = 0, pagenum = 1;
    FILE *fp;
    fp = fopen(filename, "r");    
    if (fp == NULL){
        printf("Could not open file %s\n",filename);
        return NULL;
    }
    fcontent = (char *)malloc( sizeof(char *)*READFILECONTENTSIZE);
    if(fp) {
    	//while (fgets(fcontent, PAGE_SIZE*4, fp) != NULL);
        fread( fcontent, sizeof(char*), READFILECONTENTSIZE, fp);
        //printf("%s", fcontent);
        fclose(fp);
    }
    return fcontent;
}

void parsed( char * cont )
{
	newLine[lineNumber] = strtok(cont, delim);
    while (newLine[lineNumber] != NULL) {
        newLine[++lineNumber] = strtok(NULL, delim);
    }
    
    for( int i = 0; i<lineNumber; i++) {
        if ( strstr( newLine[i], comment) != NULL ) {
            justComment[commentLineNumber] = newLine[i];
            commentLineNumber++;
        }
        else if ( strstr( newLine[i], " ") != NULL ) {
            leftSideOfEqual[confNumber] = strtok(newLine[i], equal );
            rightSideOfEqual[confNumber] = strtok(NULL, equal );
            if( rightSideOfEqual[confNumber] == NULL )
                continue;
            else
                confNumber++;
        }
        else {
            // TODO
        }
    }
}

char *content = NULL;

int main(int argc, char **argv)
{
    if( argc < 2  )
    {
        printf(" please, Specify the config file \n");
        return 0;
    }
    else if ( argc > 2 )
    {
        printf(" Too many args \n");
        return 0;
    }
    content = (char *)malloc( sizeof(char *)*READFILECONTENTSIZE);
    content = readcontent(argv[1]);
    parsed(content);

    for( int i = 0; i< commentLineNumber; i++)
        printf( "just a comment %s \n  ", justComment[i] );


    for( int i = 0; i< confNumber; i++)
        printf( "%s =  %s \n  ", leftSideOfEqual[i], rightSideOfEqual[i] );
}