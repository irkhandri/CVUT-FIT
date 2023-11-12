#include <stdio.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX 1000000



int SearchLastId (int * arr_id, int * arr_apper, int cnt){
    if (cnt == 0) return -1;
    for (int i = cnt - 1; i >= 0; i--){
        if (arr_id[i] == arr_id[cnt]){
            return i;
        }
    }

    return -1;
}

// return how times 
int SearchBin (int * arr, int cnt, int find){
    if (cnt == -1) return 1; 
    int frst = 0, lst =  cnt , mdl = (frst+lst)/2;
    int pos;

    while (frst <= lst){
        if (arr[mdl] < find)
            frst = mdl + 1;
        else if (arr[mdl] == find){
            pos = mdl;
            break;
        }
        else 
            lst = mdl - 1;

        mdl = (frst + lst)/2;
    }

    if (frst > lst)
        return 0;

    //int flg = pos;

    while (arr[pos-1] == find){
        pos--;
    }
    
    int times = 0;

    for (int i = pos; i <= cnt; i ++){
        if (arr[i] == find)
            times ++;
        else 
            return times;
    }

    

    return times;
}



int intCmp ( const int * aPtr, const int * bPtr ){
  return (*bPtr < *aPtr) - (*aPtr < *bPtr);
}

int main (void) {
    int arr_id     [MAX];
    //int arr_id
    int arr_appear [MAX];
    
    char first;
    int cnt = 0, fr, to;

    printf ("Pozadavky:\n");

    while ((first = getchar()) != EOF){

        if     (first == '+'){
            int new_id;
            if (scanf(" %d ", &new_id) != 1 || new_id < 0 
                || new_id > 99999 || cnt > 1000000){
                printf ("Nespravny vstup.\n");
                return 1;
            }

            arr_id[cnt] = new_id;

            if (arr_appear[new_id] == 0)   arr_appear[new_id] = 1;
            else (arr_appear[new_id])++;

            if (arr_appear[new_id] == 1)
                printf ("> prvni navsteva\n");
            else 
                printf ("> navsteva #%d\n", arr_appear[new_id]);

            cnt++;
        }

        else if (first == '?'){
            int * dyn_arr = (int *)(malloc(MAX * sizeof(*dyn_arr)));
            if (scanf (" %d %d ", &fr, &to) != 2 || fr < 0 || to >= cnt || fr > to){
                printf ("Nespravny vstup.\n");
                //free(arr);
                return 1;
            }

            int uniq= 0;
            int difference = to - fr + 1;


           //int * dyn_arr = (int *)(malloc(MAX * sizeof(*dyn_arr)));

           for (int i = fr; i <= to; i ++){
               if (dyn_arr[arr_id[i]] == 0) dyn_arr[arr_id[i]] = 1;
               else (dyn_arr[arr_id[i]]) ++ ;
               if (dyn_arr[arr_id[i]] != 1)   uniq ++;
               //printf ("I %d\n", dyn_arr[arr_id[i]]);
               //printf ("fr %d, to %d \n", fr , to);
           }
            printf ("> %d / %d\n",  difference-uniq, difference);
           

            dyn_arr = NULL;
            free (dyn_arr);
        }
        else {           
            printf ("Nespravny vstup.\n");
            return 1;
        }

    } 






    return 0;
}