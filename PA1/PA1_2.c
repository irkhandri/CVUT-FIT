#include <stdio.h>
#include <math.h>


int main (void){
    int error , soustava ;
    long   pozice ;
    printf("Pozice a soustava:\n");

        while (1){
            error = scanf(" %ld %d ", &pozice, &soustava) ;
        if ( error == 2) {   
        //while ((error = scanf(" %d %d ", &pozice, &soustava) == 2 ) ) {
           //printf ("|%d|\n", error);
           if (pozice < 0 || soustava < 2 || soustava > 36 || error <= 0 ){
               printf ("Nespravny vstup.\n");
               return 2;
           }

            long cnt = 0, i = 1 ;
            //last added value
            long last = 0 ;
            long actual = 0;
            // find next greater cnt then position and power
            while ( 1 ){
                last = (pow(soustava, i) - pow(soustava, i-1) ) * i; 
                if (cnt + last >= pozice) break;
                cnt += last;
                i++;  
            }
            cnt++;                                                                  // add 0
            actual = (int)pow(soustava, i-1) ;                                              // count in dec
            //printf ("!!! actual =  %d, %d, power = %d\n", actual, cnt, i);
            long diff = (pozice - cnt) / i;                                          // difference between positions in dec
            long modul = (pozice - cnt) % i;                                         // position in current value
            actual += diff;
            cnt += diff * i;    
            for (int j = i ; j > 0; j--){
                int check = (int)(actual/(pow(soustava, j-1)));
                if (check < 10) printf ("%d", check);
                else printf ("%c", check + 87);
                actual -= check * pow (soustava, j-1);
            }
            printf("\n");
            for (int sp = 0; sp < modul; sp ++){
                printf(" ");
            }
            printf ("^\n");
            
            //error = -1;
            
        }
        else break;
    
        }
   

    if ( !feof ( stdin )  )
  { 
    printf ( "Nespravny vstup.\n" );
    return 1;
  }

    if (error != EOF){
        printf ("Nespravny vstup.\n");
        return 1;
    }


    return 0;
}

