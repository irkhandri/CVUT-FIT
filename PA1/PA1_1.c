#include <stdio.h>
#include <math.h>
#include <float.h>

int check_size (double a, double b, double c, double prekryv){
    if (a == b && c <= prekryv) return 1;
    else return 0; 
}

double count (double p, double l, double prekryv){
    int cnt = 1 + ceil((double)((p-l)/(l-prekryv)));
    double mezi = (p-l)/(l-prekryv);
    double flor = floor(mezi);
    if (p<=l) return 1;
    if (l<=prekryv) return 0;
    if (fabs(mezi-flor)<= DBL_EPSILON*(mezi))
        return 1 + flor;
    return cnt;
}

int main (void){
    double hl, wl;
    double hp, wp;
    double prekryv;

    printf ("Velikost latky:\n");
    if (scanf (" %lf %lf ", &wl, &hl) != 2 || wl <= 0 || hl <= 0){
        printf ("Nespravny vstup.\n");
        return 1;
    }

    printf ("Velikost plachty:\n");
    if (scanf (" %lf %lf ", &wp, &hp) != 2 || wp <= 0 || hp <= 0){
        printf ("Nespravny vstup.\n");
        return 2;
    }

    double sqrt_l = hl * wl;
    double sqrt_p = hp * wp;

    if (sqrt_p <= sqrt_l){
        printf ("Pocet kusu latky: 1\n");
        return 0;
    } 
    printf ("Prekryv:\n");
    if (scanf (" %lf ", &prekryv) != 1 || prekryv < 0){
        printf ("Nespravny vstup.\n");
        return  3;
    }
    
    if (check_size(hl, hp, wl, prekryv) == 1){
        printf ("Nelze vyrobit.\n");
        return 1;
    } 

    if (check_size(wl, wp, hl, prekryv) == 1){
        printf ("Nelze vyrobit.\n");
        return 1;
    } 

    int h_cnt_1 = count(hp, hl, prekryv); 
    int w_cnt_1 = count(wp, wl, prekryv);
    int cnt_1 = h_cnt_1*w_cnt_1;

    int h_cnt_2 = count(hp, wl, prekryv); 
    int w_cnt_2 = count(wp, hl, prekryv);
    int cnt_2 = h_cnt_2*w_cnt_2;

    int result = cnt_1;
    if (cnt_1 > cnt_2) result = cnt_2;
    if (cnt_1 == 0) result = cnt_2;
    result = cnt_2 == 0 ? cnt_1 : result;

    //printf (" |%d %d|\n",cnt_1, cnt_2);
    printf ("Pocet kusu latky: %d\n", result);
    return 0;
}
