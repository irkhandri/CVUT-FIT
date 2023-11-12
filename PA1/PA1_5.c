#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char name [100];
    int  appear;
} GOODS;

int InputData (GOODS * arr, int * cnt, int * act){
     
    char nam [100];
    scanf(" %s ", nam);
    //printf ("%s\n", nam);
    int len = 0;
    if ((len=(int)strlen(nam)) >= 100 || nam[0] == '\0')    return 1;
    
    strcpy(arr[*cnt].name,nam); 
     arr[*cnt].appear=1;   
       
    /*
    if (*cnt==0){
        strcpy(arr[*cnt].name,nam); 
        arr[*cnt].appear=1;   
        (*cnt)++;
        return 0 ;
    }
    for (int i=0;i<*cnt;i++){
        if (strcmp(nam,arr[i].name)==0){
            arr[i].appear+=1;
            return 0;
        }
    }
    strcpy(arr[*cnt].name,nam); 
    arr[*cnt].appear=1;  
    */ 
    (*cnt)++;
    return 0;
}


void AlmostOutput (GOODS * arr, int cnt, int flwng, int * pop){
    int doubl = 0;
    for (int i = 0; i < flwng && i < cnt; i++){
        int j = i + 1;
        if (arr[i].appear == arr[j].appear ){
            while (arr[i].appear == arr[j].appear){
                doubl = j+1;
                j++;
                if (j == cnt) break;
            }
            for (int q = i; q < doubl; q++){
                printf ("%d.-%d. %s, %dx\n", i+1, doubl, arr[q].name, arr[q].appear);
                *pop += arr[q].appear;
            }
            //printf ("!!%d\n", doubl);
            i = doubl - 1;
        }
        else{
            printf ("%d. %s, %dx\n", i+1, arr[i].name, arr[i].appear);
            *pop += arr[i].appear;
        }   
    }
}


int ReturnPopular (GOODS *arr, int flwng, int cnt){
    int num = 0, i = 0,to;

    if (cnt <= flwng) to = cnt;
    else to = flwng ;   

    
    while (arr[i].appear>=arr[to-1].appear){
        //printf ("!!!%s, %d\n", arr[i].name, arr[i].appear);
        num += arr[i].appear;
        if (i==cnt-1) return num;

        i++;
    }
    return num;
}



void Resort (GOODS * arr, GOODS * ptr, int cnt, int * cnt_ptr, int * act){
    /*for (int i=0; i < cnt-1; i++){
                int app=1;
                int next=i;
                strcpy (ptr[*cnt_ptr].name, arr[i].name);
                
                for (int j=0; j< cnt; j++){
                    if (strcmp(arr[i].name, arr[j].name)==0){
                        app++;
                        //printf("!!!!%s, %s,%d!!!!!\n", arr[i].name, arr[j].name, i);
                        next = j;
                    }
                    else break;
                }

                
                ptr[*cnt_ptr].appear=app;
                (*cnt_ptr)++;
                i=next;

            }  */


    *cnt_ptr=0;
    int i = 0;
    while (i < cnt ){

        strcpy (ptr[*cnt_ptr].name, arr[i].name);
        //printf("!!!!%s!!!!!\n", ptr[*cnt_ptr].name);
        ptr[*cnt_ptr].appear=arr[i].appear;

        for (int j = i + 1; j < cnt; j++){
            if (strcmp(arr[i].name, arr[j].name)==0){
                ptr[*cnt_ptr].appear++;
                i = j;
            }
            else {

                break;
            }
        }
        i++;
        (*cnt_ptr)++;

        if (*cnt_ptr==*act-1){
                int poz = (*act) * 1.5;
                ptr=(GOODS*)realloc(ptr,poz*sizeof(*ptr));
                *act = poz;
            }


    }



}




int compare (const void *a, const void *b) {
        if (((GOODS *)a)->appear > ((GOODS *)b)->appear) {
            return -1;
        } else if (((GOODS *)a)->appear < ((GOODS *)b)->appear) {
            return 1;
        } else {
            return 0;
        }
    }

int cmp_by_appear(const void *a, const void *b) { 
     GOODS *ia = ( GOODS *)a;
     GOODS *ib = ( GOODS *)b;
    return (int)(ia->appear - ib->appear);
	/* float comparison: returns negative if b > a 
	and positive if a > b. We multiplied result by 100.0
	to preserve decimal fraction */ 
 
} 


int cmp_by_name(const void *a, const void *b) { 
    GOODS *ia = (GOODS *)a;
    GOODS *ib = (GOODS *)b;
    return strcmp(ia->name, ib->name);
}


int main (void) {

    int cnt = 0, act = 1000;
    GOODS*arr = (GOODS*)malloc(act*sizeof(*arr));
    char frst;
    GOODS*ptr = NULL; 
    int cnt_ptr = 0;

    int  poz_act=100;
    //ptr=(GOODS*)malloc(act*sizeof(*ptr));

    int cnt_flwng;
    printf ("Pocet sledovanych:\n");
    if (scanf (" %d ", &cnt_flwng) != 1 || cnt_flwng < 1){
        free (arr);
        printf ("Nespravny vstup.\n");
        return 1;
    }

    printf ("Pozadavky:\n");

    //while ((frst = getchar()) != EOF){
    //while (scanf("%c", &frst) == 1){   
    while (1){

        do {
            frst = getchar();
        }
        while (isspace(frst));

        if (frst == '+'){
            if (InputData (arr, &cnt, &act) == 1){
                free (arr);
                printf ("Nespravny vstup.\n");
                return 1;
            }
            if (cnt==act-1){
                act*=1.5;
                arr=(GOODS*)realloc(arr,act*sizeof(*arr));
            }
            //printf ("!!!!!%s\n", arr[cnt-1].name);
        //printf ("%s, %d\n", arr[cnt-1].name, arr[cnt-1].appear);

        }

        else if (frst == '#'){
            if (cnt == 0){
                printf ("Nejprodavanejsi zbozi: prodano 0 kusu\n");
                continue;
            }
            
           
            ptr=(GOODS*)malloc(act*sizeof(*ptr));
            //qsort(arr, cnt, sizeof(*arr), compare);
            //qsort(arr, cnt, sizeof(*arr), cmp_by_appear);
            qsort(arr, cnt, sizeof(*arr), cmp_by_name);    

            cnt_ptr=0;
            int i = 0;

            while (i < cnt ){
            
                strcpy (ptr[cnt_ptr].name, arr[i].name);
                //printf("!!!!%s!!!!!\n", ptr[*cnt_ptr].name);
                ptr[cnt_ptr].appear=arr[i].appear;

                for (int j = i + 1; j < cnt; j++){
                    if (strcmp(arr[i].name, arr[j].name)==0){
                        ptr[cnt_ptr].appear++;
                        i = j;
                    }
                    else     break;
                }
                i++;
                (cnt_ptr)++;

                if (cnt_ptr==poz_act-1){
                        poz_act *=  1.8;
                        ptr=(GOODS*)realloc(ptr,poz_act*sizeof(*ptr));
                    }
            }


         /*   for (int i = 0; i < cnt; i++){
                 printf("2!!!!%s!!!!!\n", arr[i].name);
            }   */      
           
            qsort(ptr, cnt_ptr, sizeof(*ptr), compare);
            
            int popular = 0;
            //AlmostOutput(arr, cnt, cnt_flwng, &popular);
            AlmostOutput(ptr, cnt_ptr, cnt_flwng, &popular);
            printf ("Nejprodavanejsi zbozi: prodano %d kusu\n", popular);
          
            free(arr);
           arr=ptr;
           cnt = cnt_ptr;

        }  
        
        
        else if (frst == '?'){
            if (cnt == 0){
                printf ("Nejprodavanejsi zbozi: prodano 0 kusu\n");
                continue;
            }
           ptr=(GOODS*)malloc(act*sizeof(*ptr));
            //qsort(arr, cnt, sizeof(*arr), compare);
            //qsort(arr, cnt, sizeof(*arr), cmp_by_appear);
            qsort(arr, cnt, sizeof(*arr), cmp_by_name);    

            cnt_ptr=0;
            int i = 0;

            while (i < cnt ){
            
                strcpy (ptr[cnt_ptr].name, arr[i].name);
                //printf("!!!!%s!!!!!\n", ptr[*cnt_ptr].name);
                ptr[cnt_ptr].appear=arr[i].appear;

                for (int j = i + 1; j < cnt; j++){
                    if (strcmp(arr[i].name, arr[j].name)==0){
                        ptr[cnt_ptr].appear++;
                        i = j;
                    }
                    else     break;
                }
                i++;
                (cnt_ptr)++;

                if (cnt_ptr==poz_act-1){
                        poz_act *=  1.5;
                        ptr=(GOODS*)realloc(ptr,poz_act*sizeof(*ptr));
                    }
            }


         /*   for (int i = 0; i < cnt; i++){
                 printf("2!!!!%s!!!!!\n", arr[i].name);
            }   */      
           
            qsort(ptr, cnt_ptr, sizeof(*ptr), compare);
            


            int pop = ReturnPopular(ptr, cnt_flwng, cnt_ptr);
            printf ("Nejprodavanejsi zbozi: prodano %d kusu\n", pop);

            free(arr);
           arr=ptr;
           cnt = cnt_ptr;

        }

        else if (frst == EOF){
            break;
        }
       else if (frst != EOF){
            //printf ("|%c|\n", frst);
            free (arr);
            printf ("Nespravny vstup.\n");
            return 1;
        }


    }
    
    

    free(arr);
    return 0 ;
}