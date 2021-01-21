#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <math.h>
#include <complex.h>

#define BLOCK   3072          // samples processed each invocation

#define Fs 16000             // sample rate 

/**SinusoidalFrequency Estimation Based on Time-Domain 

 * @param double *input, signal, size 
 * @return estimated frequency 
 */
double estimate_freq( double * input, int size)
{  
    double x1 = 0;
    double mean = 0.0;
    double complex freq[BLOCK];
    double fReal[BLOCK];
    for( int i = 0; i < size; i++)
    {
        /* http://www.claysturner.com/dsp/4pointfrequency.pdf
        Turner also provides a method for estimating a real-valued sinusoid's frequency 
                based on four time domain samples  
                   
                      . .              . .
                     .   * s3         .   .
                 s2 *     .          .     .
       ------------.-------.--------.------ .----                (s4 - s1)
                  .         .      .                           ------------
              s1 *           * s4 .                            2*(s3 - s2)
                .             . .


       Turner's Real 4-Samplealgorithm is[2].:*/
        if( ( input[i+1] - input[i+2]) == 0 ) {  /* x(1) ≠ x(2) */
            x1 = 0;
        } 
        else {
            x1 = (( input[i+3] - input[i])/( input[i+2] - input[i+1])) - 1;
        }
        /*[2]*/ freq[i] = (Fs/(2*M_PI))*acos(x1/2);
    }
    for( int i = 0; i < size; i++) {
        fReal[i] = creal(freq[i]); // complex to real number conversion 

        if (isnan(fReal[i])) // fReal value is Not-A-Number. set zero. 
        {
            fReal[i] = 0;
        }
        mean = mean + fReal[i];
    }
    mean /= (double)BLOCK;

    return mean;
}

void floatToInt( double *input, int16_t *output, int length )
{
    int i;
 
    for ( i = 0; i < length; i++ ) {
        if ( input[i] > 32767.0 ) {
            input[i] = 32767.0;
        } else if ( input[i] < -32768.0 ) {
            input[i] = -32768.0;
        }
        // convert
        output[i] = (int16_t)input[i];
    }
}

/* @printRepeating not used */ 
void printRepeating(double arr[], int size) 
{ 
  int i, j; 
  for(i = 0; i < size; i++) 
    for(j = i+1; j < size; j++) 
      if( ( arr[i] < ( arr[j] + 50) ) && ( arr[i] < ( arr[j] - 50) ) )  
        printf(" %f \n ", arr[i]); 
}   

// convert 16 bit ints to doubles
void intToFloat( int16_t *input, double *output, int length )
{
    int i;
 
    for ( i = 0; i < length; i++ ) {
        output[i] = (double)input[i];
    }
}

int main( int argc, char *argv[] )
{
    int16_t intData[BLOCK];
    double inputData[BLOCK];
    double frequency;
    int numWords;
    int sampleCount;
    char *inFileName;
    FILE *inFile;
 
    inFileName = NULL;
 
    if ( argc == 2 )
    {
        inFileName = argv[1];
        printf("input file name = %s\n",inFileName);
    }
    else
    {
        printf("Incorrect arguments, you must specify a file name\n");
        return(1);
    }
 
    inFile = fopen(inFileName,"rb");
    if ( !inFile )
    {
        printf("Exiting.Cannot open input file %s\n",inFileName);
        fclose( inFile );
        return(1);
    }
 
    // start counting frames
    sampleCount = 0;
 
    numWords = fread(intData, sizeof(int16_t), BLOCK, inFile ); // inFile = /home/ilter/Desktop/matlab/chirp.wav
    int freqCount = 0;
    // until end of file
    while( numWords == BLOCK )
    {
        intToFloat( intData, inputData, numWords );

        frequency = estimate_freq(inputData, numWords);

        printf("Mean freq = %f\n", frequency );

        sampleCount += BLOCK;
        numWords = fread(intData, sizeof(int16_t), BLOCK, inFile );
    }
 
    printf("\nFinished. sampleCount = %d\n",sampleCount);
 
    fclose( inFile );
    return 0;
}