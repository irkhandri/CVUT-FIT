#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
      : m_X ( x ),
        m_Y ( y ),
        m_W ( w ),
        m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */



class CWindow
{
  public:
    CWindow (size_t id, const string & title, const CRect & absPos, const string typeObject = "Window")
    : m_id(id),
    m_title(title), m_absPos(absPos), m_actPos(absPos),
    m_typeObject(typeObject)
    {}
    virtual ~CWindow() {};
// for each elem in multimap windows
    /*void updateSizeWindow (double & x, double & y, double & wight, double & hight){
    m_actPos.m_X = m_absPos.m_X * wight + x;
    m_actPos.m_Y = m_absPos.m_Y * hight + y;
    m_actPos.m_W = m_absPos.m_W * wight;
    m_actPos.m_H = m_absPos.m_H * hight;  */
    virtual void updateSizeWindow (const CRect & x){
        m_actPos.m_X = m_absPos.m_X * x.m_W + x.m_X;
        m_actPos.m_Y = m_absPos.m_Y * x.m_H + x.m_Y;
        m_actPos.m_W = m_absPos.m_W * x.m_W;
        m_actPos.m_H = m_absPos.m_H * x.m_H;
    }

    // setPosition   WINDOW
    void setPosition (const CRect & position){
        m_absPos = position;
        m_actPos = position;
        for (auto i : windows){
            i.second->updateSizeWindow(position);
        }
    }

    virtual shared_ptr<CWindow> rePointered () const {
        return shared_ptr<CWindow>( new CWindow(*this));
    };

// output operators
    friend  ostream& operator<<(ostream & os, const CWindow & x) {
        return x.print(os);
    }
    // add
    CWindow & add (const CWindow & x){
        shared_ptr<CWindow> y = x.rePointered();

        y->updateSizeWindow(m_actPos);
        windows.insert({x.m_id, y});
        ///if (x.m_typeObject == "ComboBox")
            //this->cntBox += 1;

        return *this;
    }

    virtual ostream& print(ostream& os) const ;


    //    todo old operator <<
/*      friend ostream & operator<<(ostream & os, const CWindow & x )   {
        os << "["<< x.m_id << "] " << x.m_typeObject << " \"" << x.m_title << "\" " << x.m_actPos << endl;
         for (const auto & window : x.windows) {
             os << "+- " << *(window.second);
         }
         return os;
    }  */

    // search
    shared_ptr<CWindow> search (const size_t & id){
        auto res = windows.find(id);
        if (res == windows.end())
            return nullptr;
        return res->second;
    }

    CWindow(const CWindow &other)
            : m_id(other.m_id),
              m_title(other.m_title),
              m_absPos(other.m_absPos),
              m_actPos(other.m_actPos),
              m_typeObject(other.m_typeObject)
    {
        for (auto window : other.windows) {
            windows.insert({ window.first, window.second->rePointered() });
        }
    }







private:
    multimap <size_t , shared_ptr<CWindow>> windows;
    //size_t  cntBox = 0;

    protected:
    size_t m_id;
    string m_title;
    CRect m_absPos = CRect(0,0,0,0);
    CRect m_actPos = CRect(0,0,0,0);
    string m_typeObject;

};



class CButton : public CWindow
{
  public:
  CButton (int id, const CRect & relPos, const string & name)
  : CWindow(id, name, relPos, "Button")
  {}

    shared_ptr<CWindow> rePointered () const override{
        return shared_ptr<CWindow>( new CButton(*this));
    };

    ostream& print(ostream& os) const override {
        os << "[" << m_id << "] " << m_typeObject << " \"" << m_title << "\" " << m_actPos << endl;
        return os;
    }
    int w = 0;


};

class CInput : public CWindow
{
  public:
  CInput (int id,const CRect & relPos, const string & value)
  : CWindow(id,value,relPos, "Input")
  {}
    // setValue
    void setValue (const string & x){
        this->m_title = x;
    }
    // getValue
    const string getValue () const {
      return this->m_title;
  }

    ostream& print(ostream& os) const override {
        os << "[" << m_id << "] " << m_typeObject << " \"" << m_title << "\" " << m_actPos << endl;
        return os;
    }

     shared_ptr<CWindow> rePointered () const override{
        return shared_ptr<CWindow>( new CInput(*this));
    };

  int w = 0;

};
class CLabel : public CWindow
{
  public:
  CLabel ( int id, const CRect & relPos, const string & label )
  : CWindow (id, label, relPos, "Label") {}

     shared_ptr<CWindow> rePointered () const override{
        return shared_ptr<CWindow>( new CLabel(*this));
    };

    ostream& print(ostream& os) const override {
        os << "[" << m_id << "] " << m_typeObject << " \"" << m_title << "\" " << m_actPos << endl;
        return os;
    }
    int w = 0;


};
///////////////////////////////////////
class CComboBox : public CWindow
{
  public:
   CComboBox (int id, const CRect & relPos)
   : CWindow(id,"",relPos,"ComboBox")
   {}

   shared_ptr<CWindow> rePointered () const override{
        return shared_ptr<CWindow>( new CComboBox(*this));
    };

    // add
    CComboBox & add (const string & x){
        boxes.push_back(x);
        return *this;
    }

    ostream& print(ostream& os) const override {
        size_t i = 0;
        os << "[" << m_id << "] " << m_typeObject << " " << m_actPos << endl;
        for (const auto& box : boxes) {
            if (i == selected)
                os << "+->" << (box) << "<"<< endl;
            else
                os << "+- " << (box) << endl;
            ++i;
        }
        return os;
    }
    // setSelected
    void setSelected (size_t x) {
        this->selected = x;
    }
    // getSelected
    size_t getSelected () const{
        return selected;
    }

    vector<string> boxes;
    size_t selected = 0;
private:



};

ostream& CWindow::print(ostream& os) const {
    // !!!!!!!!
    size_t cntb = 1;
    os << "[" << m_id << "] " << m_typeObject << " \"" << m_title << "\" " << m_actPos << "\n";
    for (const auto& window : windows) {
        if (window.second->m_typeObject == "ComboBox"){
            //cout << "CNT " << cntBox << endl;
            //CComboBox & b = dynamic_cast <CComboBox &> (*window.second);
            CComboBox *b = dynamic_cast<CComboBox*>(&(*window.second));
            //os <<"+- " << "[" << window.second->m_id << "] " << window.second->m_typeObject << " " << window.second->m_actPos << endl;
            os <<"+- " << "[" << b->m_id << "] " << b->m_typeObject << " " << b->m_actPos << endl;
            size_t i = 0;
            //os << "SIZEEEE " << windows.size() << " cntb " << cntb << endl;

            if (cntb != windows.size()){
            //if (window != windows.end()-1){
                for (auto box : b->boxes){
                    if (i == b->selected)
                        os << "|  +->" << (box) << "<"<< endl;
                    else
                        os << "|  +- " << (box) << endl;
                    ++i;
                }
            }
            else {
                for (auto box : b->boxes){
                    if (i == b->selected)
                        os << "   +->" << (box) << "<"<< endl;
                    else
                        os << "   +- " << (box) << endl;
                    ++i;
                }
            }


        }
        else
            os << "+- " << *(window.second);
        cntb ++;
    }
    return os;
}

// !!!!!_________FOR EXAMPLE dont todo
/*
class Cmain{
    int id;
    map <int, Cmain *> mapsCmain;
};

class CElem : Cmain{

    int t;
};

class CElem2 : Cmain{
    int y;
};


CElem qw ;
CElem2 wq;
*/


#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}




int main ( void )
{
/*
    //CWindow b;
    CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
   // a.setPosition(CRect ( 10, 130, 6030, 43380 ));
    a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
    a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
    a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
    //cout << "ELEM " << endl << toString(a) << endl;


    cout << toString(*a.search(2)) << endl;
    //cout <<"[10] Label \"Username:\" (70,58,120,48)\n"<< endl;


    a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );



    cout << toString ( a );
*/
cout << "SIZEE " << sizeof (CWindow) << " | " <<  sizeof (CButton) << endl;
   // assert (sizeof (CWindow) < sizeof (CInput));
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );


    a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
    cout << toString(a) << endl;

    assert ( toString ( a ) ==
      "[0] Window \"Sample window\" (10,10,600,480)\n"
      "+- [1] Button \"Ok\" (70,394,180,48)\n"
      "+- [2] Button \"Cancel\" (370,394,180,48)\n"
      "+- [10] Label \"Username:\" (70,58,120,48)\n"
      "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
      "+- [20] ComboBox (70,154,480,48)\n"
      "   +->Karate<\n"
      "   +- Judo\n"
      "   +- Box\n"
      "   +- Progtest\n" );

    CWindow b = a;
    assert ( toString ( *b . search ( 20 ) ) ==
      "[20] ComboBox (70,154,480,48)\n"
      "+->Karate<\n"
      "+- Judo\n"
      "+- Box\n"
      "+- Progtest\n" );

    assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
    dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
    assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
    dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );


    b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );


    assert ( toString ( b ) ==
      "[0] Window \"Sample window\" (10,10,600,480)\n"
      "+- [1] Button \"Ok\" (70,394,180,48)\n"
      "+- [2] Button \"Cancel\" (370,394,180,48)\n"
      "+- [10] Label \"Username:\" (70,58,120,48)\n"
      "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
      "+- [20] ComboBox (70,154,480,48)\n"
      "|  +- Karate\n"
      "|  +- Judo\n"
      "|  +- Box\n"
      "|  +->Progtest<\n"
      "+- [21] ComboBox (70,250,480,48)\n"
      "   +->PA2<\n"
      "   +- OSY\n"
      "   +- Both\n" );

    assert ( toString ( a ) ==
      "[0] Window \"Sample window\" (10,10,600,480)\n"
      "+- [1] Button \"Ok\" (70,394,180,48)\n"
      "+- [2] Button \"Cancel\" (370,394,180,48)\n"
      "+- [10] Label \"Username:\" (70,58,120,48)\n"
      "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
      "+- [20] ComboBox (70,154,480,48)\n"
      "   +->Karate<\n"
      "   +- Judo\n"
      "   +- Box\n"
      "   +- Progtest\n" );


    b . setPosition ( CRect ( 20, 30, 640, 520 ) );
    assert ( toString ( b ) ==
      "[0] Window \"Sample window\" (20,30,640,520)\n"
      "+- [1] Button \"Ok\" (84,446,192,52)\n"
      "+- [2] Button \"Cancel\" (404,446,192,52)\n"
      "+- [10] Label \"Username:\" (84,82,128,52)\n"
      "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
      "+- [20] ComboBox (84,186,512,52)\n"
      "|  +- Karate\n"
      "|  +- Judo\n"
      "|  +- Box\n"
      "|  +->Progtest<\n"
      "+- [21] ComboBox (84,290,512,52)\n"
      "   +->PA2<\n"
      "   +- OSY\n"
      "   +- Both\n" );

/*
       */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
