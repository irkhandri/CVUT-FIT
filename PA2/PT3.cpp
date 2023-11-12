#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRange
{
  public:
    CRange () {};
    CRange(long long lo, long long hi);
    ~CRange() {};

    bool operator == (const CRange & rhs) const;
    bool operator != (const CRange & rhs) const;
    long long int  getFirst () const {
        return a;
    };
    long long int  getSecond () const {
        return b;
    };

    void setFirst(long long int x){
        a = x;
    };
    void setSecond(long long int x){
        b = x;
    };

    string print() const ;
    ostream & operator << (ostream & out);
  private:
    long long a;
    long long b;
};

string CRange::print() const {
    if (a == b)
        return to_string(a);
    return '<' + to_string(a) + ".." + to_string(b) + '>';
}

CRange::CRange(long long lo, long long hi){
    if (lo > hi)
        throw std::logic_error("Neplatny vstup");
    a = lo;
    b = hi;
}

bool cmpRangeFirst (const CRange & lhs, const CRange & rhs) {
    return lhs.getFirst() < rhs.getFirst();
}
bool cmpRangeSecond (const CRange & lhs, const CRange & rhs) {
    return lhs.getSecond() < rhs.getSecond();
}
bool cmpRangeFirstSecond (const CRange & lhs, const CRange & rhs) {
    return lhs.getSecond() < rhs.getFirst();
}

class CRangeList
{
  public:
    CRangeList () {}
    ~CRangeList () {};

    CRangeList(const CRangeList & list) {
        ranges = list.ranges;
    }

    CRangeList & operator = (const CRange & range);
    CRangeList & operator = (const CRangeList & rangeL);
    CRangeList & operator += (const CRange & range);
    CRangeList & operator += (const CRangeList & rangeL);
    CRangeList & operator -= (const CRange & range);
    CRangeList & operator -= (const CRangeList & rangeL);

    const vector <CRange> & getRanges () const;
    friend ostream & operator<< (ostream &out, const CRangeList &list) {
        out << "{";
        for (size_t i = 0; i < list.ranges.size(); i++) {
            if (i != 0) out << ",";
            if (list.ranges[i].getFirst() == list.ranges[i].getSecond()) out << list.ranges[i].getFirst();
            else out << "<" << list.ranges[i].getFirst() << ".." << list.ranges[i].getSecond() << ">";
        }
        out << "}";
        return out;
    };
    friend CRangeList operator + (CRangeList & lhs, CRangeList & rhs);

    bool includes (const long long & number) const;
    bool includes (const CRange & range) const;

    bool operator == (const CRangeList & rangeL) const;
    bool operator != (const CRangeList & rangeL) const;

    int controlRange (const CRange & range);
  //private:
    vector <CRange> ranges;
};

bool CRangeList::includes (const long long & number) const{
    CRange tmp = CRange(number, number);
    auto lowHi = std::lower_bound(ranges.begin(), ranges.end(), tmp, cmpRangeFirstSecond);

    if (lowHi == ranges.end())
        return false;

    if (number >= lowHi->getFirst() && number <= lowHi->getSecond())
        return true;

    return false;
}

bool CRangeList::includes (const CRange & range) const{

    if (ranges.empty() )
        return false;

    auto lowHi = std::lower_bound(ranges.begin(), ranges.end(), range, cmpRangeFirstSecond);

    if (range.getFirst() >= lowHi->getFirst() && range.getSecond() <= lowHi->getSecond())
        return true;

    return false;
}

CRangeList & CRangeList::operator -= (const CRangeList & rangeL) {
    for(CRange i : rangeL.ranges)
        *this -= i;
    return *this;
}

CRangeList & CRangeList::operator -= (const CRange & range){
    if (ranges.empty())
        return *this;

    auto low = lower_bound(ranges.begin(), ranges.end(), range, cmpRangeFirst);
    auto high = lower_bound(ranges.begin(), ranges.end(), range, cmpRangeSecond);

    if (low != ranges.end() && range.getFirst() == range.getSecond() && low->getFirst() == range.getFirst()){
        //cout << "ZAC " << endl;
        if (low->getFirst() == low->getSecond())
            ranges.erase(low);
        low->setFirst(range.getFirst()+1);
        return *this;
    }
/*
    if (low!=ranges.end())
        cout << "LOW: " << low->getFirst() << endl;
    if (high!=ranges.end())
        cout << "HIGH: " << high->getFirst() << endl;
*/
    if ((low == ranges.begin() && range.getSecond() < ranges.begin()->getFirst())
    ||  (low == ranges.end() && (low-1)->getSecond() < range.getFirst())
    ||  (low != ranges.begin() && low != ranges.end() && range.getSecond() < low->getFirst() && range.getFirst() > (low-1)->getSecond() ) ){
        //cout << " NOTHING " << endl;
        return *this;
    }

    if (low != ranges.begin() && range.getFirst() <= (low-1)->getSecond() && range.getSecond() >= (low-1)->getSecond()){
        //cout << "2  ZAC " << endl;

        if (low!=ranges.end() && low->getFirst() == range.getSecond()){
            low->setFirst(range.getSecond() + 1);
            //if ()
            (low-1)->setSecond(range.getFirst() - 1);
            return *this;
        }

        (low-1)->setSecond(range.getFirst() - 1);
        auto i = low;
        while (true){
            if (i == ranges.end() || i > high)
                break;
            if (i->getSecond() <= range.getSecond()){
                // cout << "DEL : " << i->getFirst() << " : " << i->getSecond() << endl;
                ranges.erase(i);
                --high;
                continue;
            }
            if (i == high && range.getSecond() > i->getFirst() && range.getSecond() < i->getSecond() ){
                i->setFirst(range.getSecond() + 1);
                //  cout << "       JSEM TADY " << i->getFirst() <<endl;
                break;
            }

            if (i+1 == ranges.end() ){
                // cout << "@@@@@@@@@@@@@@@" << endl;
                ranges.erase(i);
                --high;
            }
            ++i;
        }
        //cout << "  CROSS RIGHT " << endl;
    }
    else if ((low != ranges.begin() && (low-1)->getSecond() <= range.getFirst() && range.getSecond() >= low->getFirst())
        ||  (low == ranges.begin() && range.getSecond() >= low->getFirst()) ){
        // cout << "3   ZAC " << endl;

        if (low->getFirst() == low->getSecond() && low->getSecond() <= range.getSecond()){
            //  cout << "01   WWWWWWW" << endl;
            ranges.erase(low);
            return *this;

        }


        if (low!=ranges.end() && low->getFirst() == range.getSecond()){
            // cout << "WWWWWWW" << endl;
            low->setFirst(range.getSecond() +1);
            return *this;
        }

        auto i = low;

        while (true) {
            if (i == ranges.end() || i > high)
                break;
            if (i->getSecond() <= range.getSecond()) {
                //  cout << "DEL : " << i->getFirst() << " : " << i->getSecond() << endl;
                ranges.erase(i);
                --high;
                continue;
            }
            if (i == high && range.getSecond() > i->getFirst() && range.getSecond() < i->getSecond()) {
                i->setFirst(range.getSecond() + 1);
                // cout << "       JSEM TADY " << i->getFirst() << endl;
                return *this;
                break;
            }
            if (i + 1 == ranges.end()) {
                // cout << "3@@@@@@@@@@@@@@@   " << i->getFirst()  <<endl;
                ranges.erase(i);
                --high;
            }
            ++i;
        }
        //cout << "  CROSS LEFT " << endl;
    }
    // inside one range -> divorce and add
    else{
        //cout << "OLOLO " << low->getFirst() << endl;
        long long int tmp = (low-1)->getSecond();
        (low-1)->setSecond(range.getFirst() - 1);
        ranges.insert(low, CRange(range.getSecond() + 1, tmp) );
    }

/*
    if (low!=ranges.end())
        cout << "2 LOW: " << low->getFirst() << endl;
    if (high!=ranges.end())
        cout << "2 HIGH: " << high->getFirst() << endl;
    cout << "LEN " << ranges.size() << endl;
*/
    return *this;
}

CRangeList &  CRangeList::operator += (const CRangeList & rangeL){
    for (CRange i : rangeL.ranges)
        *this += i;
    return *this;
}

CRangeList operator + (CRangeList  lhs, CRangeList  rhs) {
    for (CRange i : rhs.ranges)
        lhs += i;
    return lhs;
}

CRangeList operator - (CRangeList  lhs, CRangeList  rhs){
    for (CRange i : rhs.ranges)
        lhs -= i;
    return lhs;
}

CRangeList operator + (CRange  lhs, CRange  rhs){
    CRangeList tmp;
    tmp = lhs;
    tmp += rhs;
    return tmp;
}

CRangeList operator - (CRange  lhs, CRange  rhs){
    CRangeList tmp;
    tmp = lhs;
    tmp -= rhs;
    return tmp;
}

CRangeList operator + (CRangeList  lhs, const CRange & rhs){
    lhs += rhs;
    return lhs;
}
CRangeList operator - (CRangeList  lhs, const CRange & rhs){
    lhs -= rhs;
    return lhs;
}


bool CRangeList::operator == (const CRangeList & rangeL) const{
    if (rangeL.ranges.size() != ranges.size())
        return false;

    for (long unsigned int i = 0; i < ranges.size(); i++){
        if (rangeL.ranges[i].getFirst() != ranges[i].getFirst()
        &&  rangeL.ranges[i].getSecond() != ranges[i].getSecond() )
            return false;
    }
    return true;
}

bool CRangeList::operator != (const CRangeList & rangeL) const{
    return !(*this == rangeL);
}

CRangeList & CRangeList::operator = (const CRangeList & list){
    this->ranges.clear();
    for (CRange i : list.ranges)
        this->ranges.push_back(i);
    return *this;
}


CRangeList & CRangeList::operator = (const CRange & range){
    if (!ranges.empty())
        ranges.clear();
    ranges.emplace_back(range);
    return *this;
}


int CRangeList::controlRange (const CRange & range) {

    auto low = lower_bound(ranges.begin(), ranges.end(), range, cmpRangeFirst);
    auto high  = lower_bound(ranges.begin(), ranges.end(), range, cmpRangeSecond);
    auto lowHi = lower_bound(ranges.begin(), ranges.end(), range, cmpRangeFirstSecond);
/*
    cout << "________" << endl;
    cout << "RANGE : " << range.getFirst() << " : " << range.getSecond() << endl;

    if (low != ranges.end())
        cout << "LOW:    " << low->getFirst() << " : " << low->getSecond() << endl;
    if (lowHi != ranges.end())
        cout << "LOW HI: " << lowHi->getFirst() << " : " << lowHi->getSecond() << endl;
    if (high != ranges.end())
        cout << "HIGH:    " << high->getFirst() << " : " << high->getSecond() << endl;

    cout << "____________" << endl << endl;
*/
    if (low == ranges.begin() && high == ranges.end()){
        //cout << "STTTT" << endl;
        ranges.clear();
        ranges.emplace_back(range);
        return 1;
    }

    if (low == ranges.begin() && range.getSecond() >= high->getFirst() && high+1 == ranges.end() ){
        // cout << "2STTTT" << endl;

        auto ttmmpp = high->getSecond();
        ranges.clear();
        ranges.emplace_back(CRange(range.getFirst(), ttmmpp));
        return 1;
    }
/*
    cout << "________" << endl;
    cout << "RANGE : " << range.getFirst() << " : " << range.getSecond() << endl;

    if (low != ranges.end())
        cout << "LOW:    " << low->getFirst() << " : " << low->getSecond() << endl;
    if (lowHi != ranges.end())
        cout << "LOW HI: " << lowHi->getFirst() << " : " << lowHi->getSecond() << endl;
    if (high != ranges.end())
        cout << "HIGH:    " << high->getFirst() << " : " << high->getSecond() << endl;

    cout << "____________" << endl << endl;

*/
    bool flgL = false, flgR = false;

    if (low == ranges.begin() && range.getSecond() < low->getFirst() && low->getFirst() - range.getSecond() != 1)
        return -1;

    if (lowHi == ranges.end()){
        if (range.getFirst() - (lowHi-1)->getSecond() == 1){
            (lowHi-1)->setSecond(range.getSecond());
            //  cout << " ENDDNDNDNDNDNDN" << endl;
            return 1;
        }
        return 0;
    }

    // left cross
    if (low != ranges.end() && (
            (low->getFirst() <= range.getSecond())
    || low->getFirst() - range.getSecond() == 1)){
        // cout << "LEFT CROSS" << endl;
        low->setFirst(range.getFirst());
        flgL = true;
    }
    //right cross
    if ((lowHi != ranges.end() && lowHi->getFirst() <= range.getFirst())
    || (lowHi != ranges.begin() &&  range.getFirst() - (lowHi-1)->getSecond() == 1 ) ){
        // cout << "RIGHT CROSS" << endl;
        if (lowHi->getSecond() < range.getSecond()
        || (range.getFirst() - lowHi->getSecond() == 1 && range.getFirst() > lowHi->getSecond() ) )
            lowHi->setSecond(range.getSecond());
        if (lowHi != ranges.begin() &&  range.getFirst() - (lowHi-1)->getSecond() == 1 )
            (lowHi-1)->setSecond(range.getSecond());
        flgR = true;
    }
    auto tmpPos = lowHi - ranges.begin();
    if (range.getSecond() < lowHi->getFirst()){
        // cout << "IN MIDDLE " << endl;
        ranges.insert(lowHi,range);

    }

    lowHi = ranges.begin() + tmpPos;

/*
    cout << "___m_____" << endl;
    cout << "RANGE : " << range.getFirst() << " : " << range.getSecond() << endl;

    if (low != ranges.end())
        cout << "LOW:    " << low->getFirst() << " : " << low->getSecond() << endl;
    if (lowHi != ranges.end())
        cout << "LOW HI: " << lowHi->getFirst() << " : " << lowHi->getSecond() << endl;
    if (high != ranges.end())
        cout << "HIGH:    " << high->getFirst() << " : " << high->getSecond() << endl;

    cout << "_____m_______" << endl << endl;
*/
    auto i = lowHi;
    auto tmpHi = lowHi->getSecond();
    //  cout << "TMP HI SECOND " << tmpHi << endl;

    if ((lowHi) != ranges.begin() && (lowHi-1)->getSecond() >= lowHi->getFirst()){
        --i;
        lowHi = i;
        tmpHi = i->getSecond();
        // cout << "!!!!!!!!!!!!!!!!!!!!!!!! TADY " << i->getFirst() << " : " << i->getSecond() << endl;
    }
int cnt = 0;
    while (flgL || flgR){

        if (i-1 == high || i == ranges.end())
            break;

        if (i->getSecond() <= tmpHi && cnt!=0){
            // cout << "|TRY DELL I " << endl;
            ranges.erase(i);
            cnt++;
            continue;
        }
        if ((i->getFirst() <= tmpHi &&  cnt!=0)
        ||  ( i->getFirst() - tmpHi == 1 && i->getFirst() >  tmpHi  ))
        {
            //  cout << "OPAAA" << endl;
            lowHi->setSecond(i->getSecond());
            ranges.erase(i);
            //++i;
            cnt++;
            continue;
        }
        cnt++;
        ++i;
    }
    return 1;
}

CRangeList & CRangeList::operator += (const CRange & range) {
    if (ranges.empty()){
        ranges.emplace_back(range);
        return *this;
    }

    int res = controlRange(range);
    if (!res)
        ranges.push_back(range);
    else if (res == -1)
        ranges.insert(ranges.begin(), range);
    return *this;
}

#ifndef __PROGTEST__

string             toString                                ( const CRangeList& x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int                main                                    ( void )
{


  CRangeList a, b;

  assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );

  a = CRange ( 5, 10 );
    a += CRange ( 25, 100 );
    assert ( toString ( a ) == "{<5..10>,<25..100>}" );
    a += CRange ( -5, 0 );
    a += CRange ( 8, 50 );
   assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
    a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
    assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
    a += CRange ( 106, 119 ) + CRange ( 152, 158 );
    assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
    a += CRange ( -3, 170 );
    a += CRange ( -30, 1000 );
    assert ( toString ( a ) == "{<-30..1000>}" );
    b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
     a += b;
    assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
    a -= CRange ( -400, -400 );
    assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
     a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
     assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );

      try
      {
        a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
        assert ( "Exception not thrown" == nullptr );
      }
      catch ( const std::logic_error & e )
      {
      }
      catch ( ... )
      {
        assert ( "Invalid exception thrown" == nullptr );
      }

      assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
      b = a;
      assert ( a == b );
    assert ( !( a != b ) );
    b += CRange ( 2600, 2700 );
    assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
    assert ( a == b );
    assert ( !( a != b ) );
  b += CRange ( 15, 15 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
  assert ( !( a == b ) );
  assert ( a != b );
  assert ( b . includes ( 15 ) );
  assert ( b . includes ( 2900 ) );
  b.includes ( CRange ( 15, 15 ) );
    b.includes ( CRange ( -350, -350 ) );
    b.includes ( CRange ( 100, 200 ) );
    assert ( b . includes ( CRange ( 15, 15 ) ) );
    assert ( b . includes ( CRange ( -350, -350 ) ) );
    assert ( b . includes ( CRange ( 100, 200 ) ) );
    assert ( !b . includes ( CRange ( 800, 900 ) ) );
    assert ( !b . includes ( CRange ( -1000, -450 ) ) );
    assert ( !b . includes ( CRange ( 0, 500 ) ) );
    a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
    assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
    b += a;
    assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
    b -= a;
    assert ( toString ( b ) == "{}" );
    b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
    assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
    b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
    assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
    b += CRange (38, 39);
    //cout << toString(b) << endl;

    CRangeList c;
    c += CRange(-10000, 10000);
    c += CRange(10000000, 1000000000);
    c += CRange(LLONG_MAX - 1, LLONG_MAX);
    assert(toString(c) == "{<-10000..10000>,<10000000..1000000000>,<9223372036854775806..9223372036854775807>}");


    c += CRange(LLONG_MAX, LLONG_MAX) + CRange(LLONG_MIN, LLONG_MAX);

    //cout << LLONG_MIN << " LLONG_MIN" << endl;
    //cout << toString(c) << "---------------------------" << endl;
    //c+= CRange(LLONG_MIN, LLONG_MAX);
    //cout << toString(c) << endl;


    assert(toString(c) == "{<-9223372036854775808..9223372036854775807>}");

    c -= c;
    assert(toString(c) == "{}");
    c += CRange(-10000, 10000) + CRange(10000000, 1000000000) + CRange(LLONG_MIN, LLONG_MIN + 1);
    assert(toString(c) == "{<-9223372036854775808..-9223372036854775807>,<-10000..10000>,<10000000..1000000000>}");
    c += CRange(LLONG_MIN, LLONG_MAX);
    assert(toString(c) == "{<-9223372036854775808..9223372036854775807>}");


    CRangeList d;
    d = CRange(LLONG_MIN, LLONG_MAX);
    d -= CRange(10, 12);
    d += d;

    //cout << toString(d) << endl;



#ifdef EXTENDED_SYNTAX
  CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */



  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
