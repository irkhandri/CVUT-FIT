#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int daysInMonth (int y, int m){
    int days = 28;
    if ( m == 2 )
  {
    y % 4    == 0 ? days = 29 : 28;
    y % 100  == 0 ? days = 28 : 28;
    y % 400  == 0 ? days = 29 : 28;
    y % 4000 == 0 ? days = 28 : 28;
  }
  else 
    days = 28 + ( m + m / 8 ) % 2 + 2 % m + 1 / m * 2 ;
  return days;  
}

int checkInput (int y, int m, int d, int h, int i){
    if (y < 1600 || m < 1 || m > 12 || h < 0 || h > 23 || i < 0 || i > 59 ) 
        return 0;
    if (daysInMonth(y, m) < d || d < 1)
        return 0;
    return 1;
}

//count min to the end of hour
int countMin (int vid, int po){
    int cnt = 0;
    int arr [10] = {4,5,2,3,3,1,5,4,1,2};

    int diff = po - vid;
    cnt += diff * 200;
    //printf ("cntMin = %d\n", cnt);

    for (int i = vid; i < po; i++){
        
        cnt += arr[i % 10];
       
        
        if (i % 10 == 0 && i != 0){
            cnt +=  arr[i / 10 - 1];
            
        }
        
    }
      // printf ("cntMin = %d\n", cnt);

    
    if (po == 60){
        cnt += 3;
       
        return cnt;
    }
    if (po % 10 == 0)
        cnt += arr [po / 10 - 1];
   
    return cnt;
}

int countHour (int h, int po){
    int cnt = 0;
    int arr [10] = {4,5,2,3,3,1,5,4,1,2};
    int vid = h;
    if (vid > 23){
        vid = 0;
        cnt += 6;
        return cnt;
    }
    else if (h == 0)
        cnt = 0;
    else
        cnt += arr[h % 10 - 1];

    int diff = po - vid;
    if (po == 0) return 0;
    cnt += diff * 12200;
    
    for (int i = vid; i < po; i ++){
        cnt += arr[i % 10];
      
        if (i % 10 == 0 && i != 0)
            cnt += arr[i / 10 - 1];
        
    }
    //printf ("hod = %d\n", diff);

    if (po == 24){
        cnt += 3;
        //printf ("cnt = %d\n", cnt);
        return cnt;
    }

    if (po % 10 == 0)
        cnt += arr[po / 10 - 1];
    //printf ("end cnt = %d\n", cnt);
    return cnt;
}

//count days to the end of month
int countDays  (int y_vid, int m_vid, int d, int po, int isTheEnd){
    int cnt = 0;
    int m = m_vid, y = y_vid, vid = d;

//printf ("m = %d, d = %d\n", m, d);
    if (d > daysInMonth(y_vid, m_vid) ){
        vid = 1;
        m = m_vid + 1;
        if (m > 12){
            m = 1;
            y = y_vid + 1;
        }
        return 0;
    }
       //printf ("vid = %d, m = %d, y = %d\n", vid , m, y);


    if (po == vid && isTheEnd) return 292886;
    int diff = po - vid;
    if (isTheEnd )
        cnt +=  (daysInMonth(y,m) - vid + 1) * 292886;
    else    
        cnt += diff * 292886;
    

    //if (po == daysInMonth(y,m)) cnt += 292883;
    //printf ("cnt(%d) , diff = %d \n", cnt, diff);
    return cnt;
}


int countMonth (int y_vid, int m, int po) {
    int cnt = 0;
    int y = y_vid, vid = m;

    if (vid > 12){
        vid = 1;
        y = y_vid + 1;
        return 0;
    }
    //printf ("vid = %d, cntM = %d\n", vid, cnt);
    if (vid == po ) return 0;

    for (int i = vid; i < po; i++){
        cnt += 292886 * daysInMonth (y, i);
    }
    if (po == 12) cnt += 31 * 292886;
    //printf ("vid = %d, cntM = %d\n", vid, cnt);

    return cnt;
}


long long int countYear  (int vid, int po){
    long long int cnt = 0;
   
     if (vid == po) return 0;
     //printf ("vid = %d, do = %d\n", vid, po);
    for (int i = vid; i < po; i ++){
        //printf ("i = %d, cntY = %lld\n", i, cnt);
        if (daysInMonth(i, 2) == 29) cnt += 366 * 292886;
        else                           cnt += 365 * 292886;
    }
    //printf ("cntY = %lld\n", cnt);
    return cnt;
}


int energyConsumption ( int y1, int m1, int d1, int h1, int i1,
                        int y2, int m2, int d2, int h2, int i2, long long int * consumption )
{
    //*consumption = 0;
    if (checkInput(y1,m1,d1,h1,i1) == 0 || checkInput(y2,m2,d2,h2,i2) == 0)
        return 0;

    if ( y1 == y2 ){
        if ( m1 == m2 ){
            if ( d1 == d2 ){
              if ( h1 == h2 ){
                if ( i1 > i2) return 0; }
              else if ( h1 > h2 ) return 0;}
            else if ( d1 > d2 ) return 0;}
          else if ( m1 > m2 ) return 0; }
    else if ( y2 < y1 )
        return 0;

    //int y_1 = y1, m_1 = m1, d_1 = d1, h_1 = h1, i_1 = i1; 
    //int y_2 = y2, m_2 = m2, d_2 = d2, h_2 = h2, i_2 = i2; 



    if      (y1 == y2 && m1 == m2 && d1 == d2 && h1 == h2 && i1 == i2){
        *consumption = 0;
        return 1;
  }

    if      (y1 == y2 && m1 == m2 && d1 == d2 && h1 == h2)
        *consumption = countMin (i1, i2);
    else if (y1 == y2 && m1 == m2 && d1 == d2)
        *consumption = countMin(i1, 60) + countHour (h1+1, h2) + countMin(0, i2);
    else if (y1 == y2 && m1 == m2)
        *consumption = countMin(i1, 60) + countHour (h1+1, 24) + countDays(y1, m1, d1+1, d2, 0) + countHour(0, h2) + countMin(0, i2);
    else if (y1 == y2)                                                                                                                                          //// add if the end of month
        *consumption = countMin(i1, 60) + countHour (h1+1, 24)
                     + countDays(y1, m1, d1+1, daysInMonth(y1, m1), 1) + 
                        countMonth(y1, m1+1, m2) + countDays (y2, m2, 1, d2, 0)    + countHour(0, h2) + countMin(0, i2);
    else {
        *consumption = countMin(i1, 60) + countHour (h1+1, 24) + countDays(y1, m1, d1+1, daysInMonth(y1, m1), 1) +
        countMonth(y1, m1+1, 12) +  countYear(y1+1, y2) + countMonth(y2, 1, m2) + 
                            countDays (y2, m2, 1, d2, 0)    + countHour(0, h2) + countMin(0, i2) ;
        
    
    
    }
//printf ("cons = %d\n", countMin(i1, 60));


    return 1;
}






#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int consumption;

 // int qwe = energyConsumption (      2020, 11,  30, 23, 59,
       //                        2020, 12,  1, 0, 0,  &consumption ) ;
    //printf ("%lld\n", consumption   );
    //printf ("%d\n", qwe);

   //printf ("!!!%d\n", daysInMonth(2021, 2));

//////// kradene



//printf ("\nSRAN\n");

    ///////

  assert ( energyConsumption ( 2021, 10,  1,  0, 15,
                               2021, 10,  1,  0, 25, &consumption ) == 1
           && consumption == 2035LL );
    assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 0LL );
assert ( energyConsumption ( 2021, 11,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2021, 10, 32, 12,  0,
                               2021, 11, 10, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2100,  2, 29, 12,  0,
                               2100,  2, 29, 12,  0, &consumption ) == 0 );
assert ( energyConsumption ( 2400,  2, 29, 12,  0,
                               2400,  2, 29, 12,  0, &consumption ) == 1
           && consumption == 0LL );
assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  1, 18, 45, &consumption ) == 1
           && consumption == 67116LL );
assert ( energyConsumption ( 2021, 10,  1,  0,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 146443LL );

assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  2, 11, 20, &consumption ) == 1
           && consumption == 269497LL );

assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  1, &consumption ) == 1
           && consumption == 204LL );
          

  
assert ( energyConsumption ( 1900,  1,  1, 13, 15,
                               1900, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );


assert ( energyConsumption ( 2021,  1,  1, 13, 15,
                               2021, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
 
 
 assert ( energyConsumption ( 2024,  1,  1, 13, 15,
                               2024, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81398919LL );
             

             assert ( energyConsumption ( 2100, 11, 29, 0, 0, 2100, 12, 28, 0, 0, &consumption ) == 1
           && consumption == 8493694LL );
             
  

  
  
  
  
  
  
  
  return 0;
}
#endif /* __PROGTEST__ */
