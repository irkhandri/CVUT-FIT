#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */


/**
 * Create 2d arr with pointers to strings
 *  
 * Realloc memory when read @param row words or length of word equal @param col
 * 
 *  @param[in] str     array of char 
 *  @param[in,out] wrd     array of char where will be read words
 *  @param[in,out] len_all size of array str
 *  @return wrd when read all str  
*/
char **  createArr (char * str, char ** wrd, int * len_all){
    int  row = 100;  
    wrd = (char**)malloc(row*sizeof(char*)); 
    while (*str!='\0'){
        // ignoring non-alpha characters
        while (!isalpha(*str) && *str!='\0' )
            str++;

        // realloc first dim
        if (*len_all==row-1){
                row*=1.5;  
                wrd = (char**)realloc(wrd,row*sizeof(char*));
        }  

        int len_wrd=0, col=30;                          
        wrd[*len_all]=(char *)malloc(1+col*sizeof(char));

        if (*str=='\0' && *len_all==0){
            wrd[0][0]='\0';
            return wrd;
        }     

        while (isalpha(*str)!=0){
            // realloc second dim
            if (len_wrd==col){
                col*=1.5;
                wrd[*len_all]=(char*)realloc(wrd[*len_all], 1+col*sizeof(char));
            }
            wrd[*len_all][len_wrd]=*str;
            str++;
            len_wrd++;
        }
        wrd[*len_all][len_wrd]='\0';
        while (!isalpha(*str) && *str!='\0') str++;
        (*len_all)++;
    }
    wrd[*len_all]=(char *)malloc(2);
    wrd[*len_all][0]='\0';
    return wrd;
}


/**
 *  Comparing each word in 2d arrays
 * 
 *  Ignoring repeated words in sorted arrays
 * 
 * @param[in] wrd1  sorted 2d array of char 
 * @param[in] wrd2  sorted 2d array of char 
 * @param[in] size1 size of array @param wrd1
 * @param[in] size2 size of array @param wrd2
 * @returns 0 if both arrays contain the same words, 0 otherwise
*/
int compareWordsLoop(char ** wrd1, char ** wrd2, int size1, int size2){

    int first=1,second=1;
    while (first<=size1 && second<=size2){
        while  (strcasecmp(wrd1[first-1],  wrd1[first])==0)  first++;
        while  (strcasecmp(wrd2[second-1], wrd2[second])==0) second++;
        
        // in case when words are different
        if (strcasecmp(wrd1[first-1], wrd2[second-1])!=0){
            return 1;
        }
        if (wrd1[first][0]=='\0' || wrd2[second][0]=='\0') return 0;
        first++;
        second++;  
    }
    return 0;
}


// Comparing strings for sorting 
int compare(const void * a, const void * b ){
  return strcasecmp(*(char **)a, *(char **)b);
}


// Free memory from array
void freeArr(char ** str, int len){
    for (int i=0; i<=len; i++){
        free(str[i]);
    }
    free(str);
}
/**
 * Compare two array with strings
 * Create 2d array for each string
 * 
 * @param[in] a array with characters
 * @param[in] b array with characters
 * @returns 1 if one array contains same words as second, 0 otherwise
*/
int sameWords(const char * a, const char * b){
    int len1=0, len2=0;
    int size1=(int)strlen(a)+1;
    int size2=(int)strlen(b)+1;

    char * str_a=(char *)malloc(size1*sizeof(char));
    char * str_b=(char *)malloc(size2*sizeof(char));
    strcpy(str_a, a);
    strcpy(str_b, b);
    
    char ** wrd1=NULL;
    wrd1=createArr(str_a, wrd1, &len1);
    char ** wrd2=NULL;
    wrd2=createArr(str_b, wrd2, &len2);

    qsort(wrd1, len1, sizeof(char*), compare);
    qsort(wrd2, len2, sizeof(char*), compare);

    int ret=compareWordsLoop(wrd1, wrd2, len1, len2);

    if (ret==1){
        freeArr(wrd1, len1);
        freeArr(wrd2, len2);
        free(str_a);
        free(str_b);
        return 0;
    }

    freeArr(wrd1, len1);
    freeArr(wrd2, len2);
    free(str_a);
    free(str_b);

    return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  sameWords ( "One TwO ONE TWO two two twO", "one ONE oNe OnE TWO two" ) ;
assert(  sameWords ( "One TwO ONE TWO two two twO", "one ONE oNe OnE TWO two" )  == 1);

assert ( sameWords ( "  qwe QWDQ QWEQWE QWDQWD Qwefwe wefwef wfw32 ewf wefw 3 ef w wfw efw efw f23 fewf w3 r3w fwe f23 r \
    ", "           qwe QWDQ QWEQWE QWDQWD Qwefwe wefwef wfw32 ewf wefw 3 ef w wfw efw efw f23 fewf w3 r3w fwe f23 r       '" ) == 1 );
assert ( sameWords ( "  qwe    ", "           qwe      '" ) == 1 );
	
assert ( sameWords ( "                                       23423               ", "3" ) == 1 );
   assert ( sameWords ( "thisisalongwordaverylongwordbutthelengthisstillsmallerthanthelengthofthelongestwordintheworld", \
   "thisisalongwordaverylongwordbutthelengthisstillsmallerthanthelengthofthelongestwordintheworld thisisalongwordaverylongwordbutthelengthisstillsmallerthanthelengthofthelongestwordintheworld" )==1);
    assert ( sameWords ( "He said he would do it.", "IT said: 'He would do it.'" ) == 1 );
  assert ( sameWords ( "Hello students.", "HELLO studEnts!" ) == 1 );
  assert ( sameWords ( " He said 'hello!'", "'Hello.' he   said." ) == 1 );
  assert ( sameWords ( "one two three", "one two five" ) == 0 );    
  return 0;
}

#endif /* __PROGTEST__ */
