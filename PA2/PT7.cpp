#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


int computeNumVisits (map<string, vector<string>> & graph) {
    set<int> numVisits;
    queue<string> fronta;

    for (const auto& entry : graph) {
        int cnt = 0;
        const string& start = entry.first;
        fronta.push(start);
        set<string> visited;
        visited.insert(start);

        while (!fronta.empty()) {
            string act = fronta.front();
            fronta.pop();
            for (auto & neighbor : graph[act] ) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    fronta.push(neighbor);
                    ++cnt;
                }
            }
        }
        auto res = numVisits.insert(cnt);
        if (!res.second)
            return -1;

    }
    return numVisits.size();
}


template <typename M_>
class CContest
{
  public:

    CContest() {};
    ~CContest() {};

    CContest &  addMatch (const string & lhs, const string  & rhs ,const M_ & src){


        auto res1 = inventor.find(make_pair(lhs, rhs));
        auto res2 = inventor.find(make_pair(rhs, lhs));
        if (res1 != inventor.end() || res2 != inventor.end()){
            throw logic_error("ojoj");
        }
        inventor.insert(make_pair(make_pair(lhs, rhs), src));
        return *this;
    }

    template<typename Comparator>
    bool isOrdered(Comparator comparator) const {
        map <string, vector <string>> winner;
        map <string, int > cntNeighbours;

        for (auto & i : inventor){
            winner.insert(std::make_pair(i.first.first, std::vector<std::string>()));
            winner.insert(std::make_pair(i.first.second, std::vector<std::string>()));
            if (comparator(i.second) > 0)
                winner[i.first.first].push_back(i.first.second);
            else if (comparator(i.second) < 0)
                winner[i.first.second].push_back(i.first.first);
        }

        int r = computeNumVisits(winner);
        if (r == -1)
            return false;
        return r ;

    }


    // results ( comparator )
    template<typename Comparator>
    list<string> results (Comparator comparator) const {

        map <string, vector <string>> winner;

        for (auto & i : inventor){
            winner.insert(std::make_pair(i.first.first, std::vector<std::string>()));
            winner.insert(std::make_pair(i.first.second, std::vector<std::string>()));
            if (comparator(i.second) > 0)
                winner[i.first.first].push_back(i.first.second);
            else if (comparator(i.second) < 0)
                winner[i.first.second].push_back(i.first.first);
        }

/*
        cout << "______________IS_______________" << endl;
        for (auto i : winner){
            cout << "{  WINNER " << i.first << " : "  << endl;
            for (auto j : i.second){
                cout << "ELEM " << j << " "  <<  endl;
            }
            cout << "        }  " << endl;
        }
*/
        map<int, string> numVisits;
        queue<string> fronta;
        list <string> resList ;

        for (const auto& entry : winner) {
            int cnt = 0;
            const string& start = entry.first;

            fronta.push(start);
            set<string> visited;
            visited.insert(start);
            while (!fronta.empty()) {
                string act = fronta.front();
                fronta.pop();
                for (auto & neighbor : winner[act] ) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        fronta.push(neighbor);
                        ++cnt;
                    }
                }
            }
            auto res = numVisits.insert(make_pair(cnt,start));
            if (!res.second){
                throw logic_error("aaaaa");
            }

        }
        for (auto rit = numVisits.rbegin(); rit != numVisits.rend(); ++rit) {
            resList.push_back(rit->second);
        }
        return resList;
    }

  private:
    map <pair<string, string>, M_> inventor;
};






#ifndef __PROGTEST__
struct CMatch
{
  public:
                             CMatch                        ( int               a, 
                                                             int               b )
      : m_A ( a ), 
        m_B ( b )
    {
    }
    
    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
  public:
                             HigherScoreThreshold          ( int diffAtLeast )
      : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int            m_DiffAtLeast;    
};

int                HigherScore                             ( const CMatch    & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

struct CMatchBool {
public:
    CMatchBool( bool a, bool b )
            : m_A ( a ),m_B ( b )
    {}
    bool   m_A;
    bool   m_B;
};
int                HigherScoreBool                             ( const CMatchBool    & x )
{
    return (x.m_A == true) - (x.m_B == true);
}


int                main                                    ( void )
{

    CContest<CMatchBool>  aqa;
    aqa . addMatch ( "Python", "PHP", CMatchBool ( true, false ) )
            . addMatch ( "PHP", "Perl", CMatchBool (true, false ) );
    assert ( aqa . isOrdered (HigherScoreBool ) );
    CContest<CMatchBool>  aa;
    aa . addMatch ( "Python", "PHP", CMatchBool ( true, false ) )
            . addMatch ( "PHP", "Perl", CMatchBool ( true, false ) )
            . addMatch ( "Perl", "Bash", CMatchBool ( true, false ) )
            . addMatch ( "Bash", "JavaScript", CMatchBool ( true, false ) )
            . addMatch ( "JavaScript", "VBScript", CMatchBool ( true, false ) );
    assert ( aa . isOrdered (HigherScoreBool ) );


    CContest<CMatchBool>  q;
    q . addMatch ( "C++", "Pascal", CMatchBool ( true, false ) )
            . addMatch ( "C++", "Java", CMatchBool (  true, false ) )
            . addMatch ( "Pascal", "Basic", CMatchBool (  true, false) )
            . addMatch ( "Java", "PHP", CMatchBool (  true, false ) )
            . addMatch ( "Java", "Pascal", CMatchBool (  true, false ) )
            . addMatch ( "PHP", "Basic", CMatchBool (  true, false) );
    assert ( ! q . isOrdered (HigherScoreBool ) );




  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
   assert ( ! x . isOrdered ( HigherScore ) );

    try
    {
      list<string> res = x . results ( HigherScore );
      assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
      assert ( "Invalid exception thrown!" == nullptr );
    }

    x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) );
    assert ( x . isOrdered ( HigherScore ) );

    try
    {
      list<string> res = x . results ( HigherScore );
      assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
    }
    catch ( ... )
    {
      assert ( "Unexpected exception!" == nullptr );
    }


    assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );


    try
    {
      list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
      assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
      assert ( "Invalid exception thrown!" == nullptr );
    }

    assert ( x . isOrdered ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
    } ) );

    try
    {
      list<string> res = x . results ( [] ( const CMatch & x )
      {
        return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A );
      } );
      assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
    }
    catch ( ... )
    {
      assert ( "Unexpected exception!" == nullptr );
    }

    CContest<bool>  y;

    y . addMatch ( "Python", "PHP", true )
      . addMatch ( "PHP", "Perl", true )
      . addMatch ( "Perl", "Bash", true )
      . addMatch ( "Bash", "JavaScript", true )
      . addMatch ( "JavaScript", "VBScript", true );

    assert ( y . isOrdered ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } ) );


    try
    {
      list<string> res = y . results ( [] ( bool v )
      {
        return v ? 10 : - 10;
      });
      assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
    }
    catch ( ... )
    {
      assert ( "Unexpected exception!" == nullptr );
    }

    y . addMatch ( "PHP", "JavaScript", false );
    assert ( !y . isOrdered ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } ) );


    try
    {
      list<string> res = y . results ( [] ( bool v )
      {
        return v ? 10 : - 10;
      } );
      assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
      assert ( "Invalid exception thrown!" == nullptr );
    }

    try
    {
      y . addMatch ( "PHP", "JavaScript", false );
      assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
      assert ( "Invalid exception thrown!" == nullptr );
    }

    try
    {
      y . addMatch ( "JavaScript", "PHP", true );
      assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e )
    {
    }
    catch ( ... )
    {
      assert ( "Invalid exception thrown!" == nullptr );
    }


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
