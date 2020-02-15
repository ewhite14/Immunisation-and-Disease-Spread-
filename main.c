#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define size 25
#define avg_size 50

//Function to sum an array
int sum_array(int p[size][size]){

    int i, j, sum=0;

    for (j = 0; j <= (size-1); j++) {

        for (i = 0; i <= (size-1); i++) {

            sum = p[j][i] + sum;
        }
    }

    return sum;
}

//Recursive function to spread disease
int disease(int p[size][size], int dy, int dx){

    int up = dy+1, down = dy-1, right = dx+1, left = dx-1;

    if( ( p[up][dx] == 0 ) && ( dy != size-1 ) ) {
            p[up][dx] = 2;
            disease( p, up, dx );
    }

    if( ( p[down][dx] == 0 ) && ( dy != 0 ) ) {
            p[down][dx] = 2;
            disease( p, down, dx );
    }


    if( ( p[dy][right] == 0 ) && ( dx!= size-1 ) ) {
            p[dy][right] = 2;
            disease( p, dy, right );
    }

    if( ( p[dy][left] == 0 ) && ( dx != 0 ) ) {
            p[dy][left] = 2;
            disease( p, dy, left );
    }
    return EXIT_SUCCESS;
}

//Function to average array
double mean_array(double k[avg_size]){

    int j, sum=0;

    double avg;

    for ( j = 0; j < (avg_size); j++ ) {

            sum = (int) k[j] + sum;
    }

    avg = (double) sum / avg_size;

    return avg;
}

double SD_array(double k[avg_size]){
    double mean, SD = 0.0;

    int i;

    mean = mean_array( k );

    for(i=0; i<avg_size; i++){
        SD += pow( k[i] - mean, 2);
    }

    return sqrt( SD/avg_size );
}

int main() {

    FILE * blob;
    blob = fopen("Disease spread.xls", "w");

    fprintf(blob, "PERCENT IMMUNISED\tPERCENT DISEASED\tERROR\tPERCENT OF VULNERABLE THAT CATCH DISEASE\tERROR\n");

    int k;

    //Arrays in which information is saved
    double avg_disease[avg_size] = {0};
    double num_vulnerable_before[avg_size] = {0};
    double num_vulnerable_after[avg_size] = {0};

    double avg_percent_diseased;

    //Number of vulnerable
    double avg_v_after;
    double avg_v_before;

    //Percentage of vulnerable that catch disease
    double percentage_vulnerable_diseased;

    //Errors
    double error_d;
    double error_v_after;
    double error_percent_v;

    //Population size
    double pop_size = pow ( size, 2 );

    //Indexing variables for random assignment
    int rx, ry;
    int dx, dy;

    //Seed for rand()
    srand( time(0) );

    //Declare percentage of immunisation
    double percentage_immunised;
    //Number of immunised individuals
    double immun;

    int counter;


        //Loop to do spread for all percentages of immunised
        for (percentage_immunised = 0.0; percentage_immunised < 1; percentage_immunised += 0.001) {

            //Total population times percent immunised
            immun = pop_size * percentage_immunised;

            //Loop to do disease a number of times to take average
            for ( k = 0; k < avg_size; k++ ) {

                //Set population to be 0
                int p[size][size] = {0};

                //Random x and y coordinate for immunisation to get certain number of immunised people
                counter = 0;
                while ( counter < immun ) {
                    rx = rand() % (size);
                    ry = rand() % (size);

                    if (p[ry][rx] == 0) {
                        p[ry][rx] = 1;
                        counter++;
                    }
                }

                //Set random value in array thats not immunised to disease
                if ( counter != (int) pop_size ) {
                    do {
                        dx = rand() % (size);
                        dy = rand() % (size);
                    } while (p[dy][dx] == 1);
                }
                p[dy][dx] = 2;


                //Disease spread function applied to array
                disease( p, dy, dx );

                //working out number of diseased, and vulnerables from sum function
                avg_disease[k] = ( (double) sum_array(p) - (double) counter ) * (0.5);
                num_vulnerable_before[k] = pop_size - 1 - counter;
                num_vulnerable_after[k] = pop_size - avg_disease[k] - counter;

            }
            //Percentage of array diseased
            avg_percent_diseased = 100 * mean_array( avg_disease ) / pop_size;

            //Working out percentage of vulnerable
            avg_v_after = mean_array( num_vulnerable_after );
            avg_v_before = mean_array( num_vulnerable_before );
            percentage_vulnerable_diseased = 100 * ( 1 - ( avg_v_after  / avg_v_before ) );

            //Working out error for disease
            error_d = 100 * ( SD_array( avg_disease ) / sqrt( avg_size - 1 ) ) / pop_size;

            //Working out error for Vulnerable percent
            error_v_after = SD_array( num_vulnerable_after ) / sqrt( avg_size - 1 );
            error_percent_v = 100 * ( error_v_after / avg_v_before );


            //Printing out Average Value
            fprintf(blob, "%g\t%g\t%g\t%g\t%g\n", 100*percentage_immunised, avg_percent_diseased, error_d, percentage_vulnerable_diseased, error_percent_v);
        }

   return 0;
}