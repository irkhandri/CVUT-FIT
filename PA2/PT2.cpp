#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CEmployee {
public:
    string name;
    string surname;
    string email;
    unsigned int salary;

    CEmployee (string name, string surname, string email = "", unsigned int salary = 0)
    : name (name), surname(surname), email(email), salary(salary)
    {}
    /*{
        this->name = name;
        this->surname = surname;
        this-> email = email;
        this->salary = salary;
    }  */
    ~CEmployee() {};
};



class CPersonalAgenda
{

public:
    CPersonalAgenda  (void);
    ~CPersonalAgenda (void);
    bool          add               (const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary);
    bool          del               (const string    & name,
                                     const string    & surname);
    bool          del               (const string    & email);
    bool          changeName        (const string    & email,
                                     const string    & newName,
                                     const string    & newSurname);
    bool          changeEmail       (const string    & name,
                                     const string    & surname,
                                     const string    & newEmail);
    bool          setSalary         (const string    & name,
                                     const string    & surname,
                                     unsigned int      salary);
    bool          setSalary         (const string    & email,
                                     unsigned int      salary);
    unsigned int  getSalary         (const string    & name,
                                     const string    & surname) const;
    unsigned int  getSalary         (const string    & email) const;
    bool          getRank           (const string    & name,
                                     const string    & surname,
                                     int             & rankMin,
                                     int             & rankMax) const;
    bool          getRank           (const string    & email,
                                     int             & rankMin,
                                     int             & rankMax) const;
    bool          getFirst          (string          & outName,
                                     string          & outSurname) const;
    bool          getNext           (const string    & name,
                                     const string    & surname,
                                     string          & outName,
                                     string          & outSurname) const;

// for control actual condition
// _________________________________________________________________

    void printSurname () const {
        cout << "_______________________NAME_____________________" << endl;
        for (long unsigned int i = 0; i < sortedByName.size(); i++){
            cout << " I= " << i << " " << sortedByName[i].surname << " " << sortedByName[i].name << " EMAIL:  "   <<
            sortedByName[i].email << " $ :" << sortedByName[i].salary<< endl;
        }
        cout << endl;
    } ;
    void printEmail   () const {
        cout << "_______________________EMAIL_____________________" << endl;
        for (long unsigned int i = 0; i < sortedByEmail.size(); i++){
            cout << " I= " << i << " " << sortedByEmail[i].surname << " " << sortedByEmail[i].name << " EMAIL:  " << sortedByEmail[i].email << " $: "
            << sortedByEmail[i].salary << endl;
        }
        cout << endl;
    } ;
    void printSalary () const {
        for (long unsigned int i = 0; i < salaryList.size(); i++){
            cout << salaryList[i] << " " ;
        }
        cout << endl;


        auto low = lower_bound(salaryList.begin(), salaryList.end(), 35000);
        auto upe = upper_bound(salaryList.begin(), salaryList.end(), 35000);
        std::cout << "LOWER: " << low - salaryList.begin() << " UPPER: " << upe - salaryList.begin() << std::endl;
    }
    // _________________________________________________________________


private:
    // for searching by surname and then by name
    vector <CEmployee> sortedByName;
    // for searching by email
    vector <CEmployee> sortedByEmail;
    // only sorted salary
    vector <unsigned  int> salaryList;

    int checkName  ( CEmployee & in_surname, int & position) const;
    int checkEmail (const CEmployee & in_email, int & position) const ;

    int addSalary (unsigned int salary) const;

    // compare two object CEmployee
    static bool cmpE(const CEmployee &lhs, const CEmployee &rhs) ;
    static bool cmpS(const CEmployee &lhs, const CEmployee &rhs) ;
    static bool cmpN(const CEmployee &lhs, const CEmployee &rhs) ;


};




CPersonalAgenda::CPersonalAgenda(void)
{}

CPersonalAgenda::~CPersonalAgenda(void)
{}

bool CPersonalAgenda::cmpE(const CEmployee &lhs, const CEmployee &rhs)  {
    return (lhs.email.compare(rhs.email) < 0) ? true : false;
}

bool CPersonalAgenda::cmpS(const CEmployee &lhs, const CEmployee &rhs)  {
    return (lhs.surname.compare(rhs.surname ) < 0) ? true : false;
}

bool CPersonalAgenda::cmpN(const CEmployee &lhs, const CEmployee &rhs)  {
    return (lhs.name.compare(rhs.name ) < 0) ? true : false;
}

/*
 * Control if vector contains object in_email
 * return -1 when contains else position where input in sorted vector
 */
int CPersonalAgenda::checkEmail (const CEmployee & in_email, int & position) const {
    auto lower = lower_bound(sortedByEmail.begin(), sortedByEmail.end(), in_email, cmpE);

    position = lower - sortedByEmail.begin();

    // dont contain
    if (lower == sortedByEmail.end())
        return sortedByEmail.size();

    // vector contains object with the same email
    if (!((*lower).email.compare(in_email.email)))
        return -1;
    else
        return lower - sortedByEmail.begin();
    //return true;
}

/*
 * Control if vector contains object in_surname
 * return -1 when contains else position where input iin sorted vector
 */
int CPersonalAgenda::checkName( CEmployee & in_surname, int & position) const {

/*
    auto lowerSurname = lower_bound(sortedByName.begin(), sortedByName.end(), in_surname, cmpS);
    position = lowerSurname - sortedByName.begin();

    // end
    if (lowerSurname == sortedByName.end())
        return sortedByName.size();

    while (!sortedByName[position].surname.compare(in_surname.surname)){
        // find same
        //if (!sortedByName[position].name.compare(in_surname.name)){
        if (sortedByName[position].name.compare(in_surname.name) == 0)
            //position++;
            return -1;

        if (sortedByName[position].name.compare(in_surname.name) > 0)
            break;

        position++;

        if ((sortedByName.begin() + position) == sortedByName.end())
            break;
    }
    return position;
*/


//__________________________________________



    auto lower = lower_bound(sortedByName.begin(), sortedByName.end(), in_surname, cmpS);

    position = lower - sortedByName.begin();
    if (lower == sortedByName.end())
        return sortedByName.size();

    auto upper = upper_bound(sortedByName.begin(), sortedByName.end(), in_surname, cmpS);

    auto lowerName = lower_bound(lower,upper, in_surname, cmpN);
    position = lowerName - sortedByName.begin();

    if (lowerName == sortedByName.end()) {
        return sortedByName.size();
    }

    position = lowerName - sortedByName.begin();

    if (sortedByName[position].name.compare(in_surname.name) == 0 && !sortedByName[position].surname.compare(in_surname.surname)) {
        return -1;
    }
    //cout << "!!!!!!!!!! " << endl;


    return position;
}

/*
 * Find position in sorted vector where input salary
 */
int CPersonalAgenda::addSalary (unsigned int salary) const {
    auto lower = lower_bound(salaryList.begin(), salaryList.end(), salary);
    if (lower == salaryList.end())
        return salaryList.size() ;
    else
        return lower - salaryList.begin();
}


bool CPersonalAgenda::add (const string & name,
                           const string & surname,
                           const string & email,
                           unsigned int   salary){

    CEmployee emp = CEmployee(name,surname,email,salary);
// control where vector is empty
    if (sortedByEmail.empty()){
        sortedByEmail.push_back(emp);
        sortedByName.push_back(emp);
        salaryList.push_back(salary);
        return true;
    }

    int posName = 0, posEmail = 0, resEmail, resName, resSalary;
    resEmail = checkEmail(emp, posEmail);
    resName  = checkName (emp, posName);;
    resSalary = addSalary(salary);

    if (resEmail == -1 || resName == -1)
        return false;

    sortedByEmail.insert(sortedByEmail.begin() + posEmail, emp);
    sortedByName. insert(sortedByName. begin() + posName,  emp);
    salaryList.insert(salaryList.begin() + resSalary, salary);

     return true;
 }


bool CPersonalAgenda::getFirst (string & outName, string & outSurname) const {
    if (sortedByName.empty())
        return false;
    outName = sortedByName[0].name;
    outSurname = sortedByName[0].surname;
    return true;
}

bool CPersonalAgenda::getNext (const string & name,
                               const string & surname,
                                     string & outName,
                                     string & outSurname) const {

    int position = 0;
    CEmployee emp = CEmployee(name, surname);
    int resName = checkName(emp, position);

    if (resName != -1 || (resName == -1 && position == (int)sortedByName.size() - 1))
        return false;

    outName = sortedByName[position + 1].name;
    outSurname = sortedByName[position + 1].surname;

    return true;
}


bool CPersonalAgenda::setSalary (const string & name,
                                 const string & surname,
                                 unsigned int   salary) {

    int position = 0;
    CEmployee emp = CEmployee(name, surname);
    if (checkName(emp, position) == -1){
        int posSalary = addSalary(sortedByName[position].salary);
        salaryList[posSalary] = salary;
        sortedByName[position].salary = salary;
        int posEmail = 0;
        checkEmail(CEmployee("","",sortedByName[position].email), posEmail);
        sortedByEmail[posEmail].salary = salary;
    }
    else
        return false;
    return true;
}


bool CPersonalAgenda::setSalary (const string & email,
                                 unsigned int salary) {
    int position = 0;
    CEmployee emp = CEmployee("","",email);
    if (checkEmail(emp,position) == -1){
        int posSalary = addSalary(sortedByEmail[position].salary);
        salaryList[posSalary] = salary;
        sortedByEmail[position].salary = salary;

        CEmployee emp2 = CEmployee(sortedByEmail[position].name, sortedByEmail[position].surname);
        int posName = 0;
        checkName(emp2, posName);
        //cout << "POSSSS  " << posName << " " <<  sortedByEmail[position].name << " " << sortedByEmail[position].surname << endl;
        sortedByName[posName].salary = salary;
    }
    else return false;

    return true;

}


unsigned int  CPersonalAgenda::getSalary (const string & name,
                                          const string & surname) const {
    int position = 0;

    CEmployee emp = CEmployee(name, surname);
    if (checkName(emp, position) == -1)
        return sortedByName[position].salary;

    else return 0;
}

unsigned int  CPersonalAgenda::getSalary        ( const string    & email ) const {
    int position = 0;

    CEmployee emp = CEmployee("", "", email);
    if (checkEmail(emp, position) == -1){
        return sortedByEmail[position].salary;
    }
    else return 0;
}

/*
 * TODO del from sortedNAme
 * TODO del from sortedEmail
 * TODO del from salary
 */

bool CPersonalAgenda::del (const string & name,
                           const string & surname) {

    CEmployee emp = CEmployee (name, surname);
    int posName = 0, posEmail = 0, posSalary = 0;
    int res = checkName(emp, posName);

    if (res != -1) return false;

    CEmployee emp2 = CEmployee ("", "", sortedByName[posName].email);
    checkEmail(emp2, posEmail);
    posSalary = addSalary(sortedByName[posName].salary);

    // salary
    salaryList.erase(salaryList.begin() + posSalary);

    //name
    sortedByName.erase(sortedByName.begin() + posName);

    //email
    sortedByEmail.erase(sortedByEmail.begin() + posEmail);

    return true;
}
bool CPersonalAgenda::del (const string & email) {

    int posEmail = 0, posName = 0, posSalary = 0;
    CEmployee emp = CEmployee ("", "", email);

    int resEmail = checkEmail(emp, posEmail);

    if (resEmail != -1) return false;

    CEmployee emp2 = CEmployee(sortedByEmail[posEmail].name, sortedByEmail[posEmail].surname);
    checkName(emp2, posName);
    posSalary = addSalary(sortedByEmail[posEmail].salary);

    salaryList.erase(salaryList.begin() + posSalary);

    //name
    sortedByName.erase(sortedByName.begin() + posName);

    //email
    sortedByEmail.erase(sortedByEmail.begin() + posEmail);

    return true;
}


bool CPersonalAgenda::changeName (const string & email,
                                  const string & newName,
                                  const string & newSurname) {

    CEmployee emp = CEmployee("", "", email);
    int posEmail = 0, posName = 0, posEmp3 = 0;
    int resEmail = checkEmail(emp,posEmail);
    // not found
    if (resEmail != -1)
        return false;

    CEmployee emp2 = CEmployee(sortedByEmail[posEmail].name, sortedByEmail[posEmail].surname);
    checkName(emp2, posName);

    CEmployee emp3 = CEmployee(newName, newSurname, email, sortedByEmail[posEmail].salary);


    int resName = checkName(emp3, posEmp3);

    if (resName == -1) return false;
    sortedByName.erase(sortedByName.begin() + posName);
    checkName(emp3, posEmp3);
/*
    cout << "_____________________________" << endl;
    printSurname();
    cout << "_______________" << endl;
    printEmail();
    cout << "_____________________________" << endl;
    cout << "POSIT: " << posEmp3 << endl;
*/

    sortedByName.insert(sortedByName.begin() + posEmp3, emp3);

    sortedByEmail[posEmail].name = newName;
    sortedByEmail[posEmail].surname = newSurname;

    return true;
}
bool CPersonalAgenda::changeEmail (const string & name,
                                   const string & surname,
                                   const string & newEmail) {

    CEmployee emp = CEmployee(name, surname);
    int posEmail = 0, posName = 0, posEmp3 = 0;
    int resName = checkName(emp, posName);

    if (resName != -1)
        return false;

    CEmployee emp2 = CEmployee("", "", sortedByName[posName].email);
    checkEmail(emp2, posEmail);

    CEmployee emp3 = CEmployee(name, surname, newEmail, sortedByName[posName].salary);
    int resEmail = checkEmail(emp3, posEmp3);
    if (resEmail == -1) return false;

    sortedByEmail.erase(sortedByEmail.begin() + posEmail);
    sortedByEmail.insert(sortedByEmail.begin() + posEmp3, emp3);

    sortedByName[posName].email = newEmail;

    return true;
}


bool CPersonalAgenda::getRank (const string & name,
                               const string & surname,
                               int          & rankMin,
                               int          & rankMax) const {

    CEmployee emp = CEmployee(name, surname);
    int posName = 0;
    int resName = checkName(emp, posName);

    if (resName != -1) return false;

    //int low, upp;
    auto low = lower_bound(salaryList.begin(), salaryList.end(), sortedByName[posName].salary);
    auto upp = upper_bound(salaryList.begin(), salaryList.end(), sortedByName[posName].salary);

    rankMin = low - salaryList.begin();
    rankMax = upp - salaryList.begin() - 1;


    return true;
}


bool CPersonalAgenda::getRank (const string & email,
                                     int    & rankMin,
                                    int     & rankMax) const {
    CEmployee emp = CEmployee("", "", email);
    int posEmail = 0;
    int resEmail = checkEmail(emp, posEmail);

    if (resEmail != -1) return false;

    //int low, upp;
    auto low = lower_bound(salaryList.begin(), salaryList.end(), sortedByEmail[posEmail].salary);
    auto upp = upper_bound(salaryList.begin(), salaryList.end(), sortedByEmail[posEmail].salary);

    rankMin = low - salaryList.begin();
    rankMax = upp - salaryList.begin() - 1;

    return true;
}




#ifndef __PROGTEST__
int main ( void )
{
    string outName, outSurname;
    int lo, hi;
    CPersonalAgenda b1;

    assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
    /*
    b1.printEmail();
    b1.printSurname();
    b1.printSalary();
     */
    //assert ( b1 . add ( "John", "qSmith", "ajoeehn", 30000 ) );

    assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
    assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );

    assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );

    b1 . getNext ( "Peter", "Smith", outName, outSurname );

    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
             && outName == "Peter"
             && outSurname == "Smith" );

    assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );

    //cout << b1.addSalary(35000) << " !!!!!!!!!!!!" << endl;
    //assert(!b1 . setSalary ( "Peter", "Smithq", 122121 ));

    assert ( b1 . setSalary ( "john", 32000 ) );

    b1 . getSalary ( "John", "Smith" );
     assert ( b1 . getSalary ( "john" ) ==  32000 );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );

    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 2
             && hi == 2 );

    assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . getSalary ( "john" ) ==  35000 );




    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 1
             && hi == 2 );

    assert ( b1 . changeName ( "peter", "James", "Bond" ) );
    //assert ( b1 . changeName ( "peter", "James1", "Bond2" ) );



    //
    assert ( !b1 . add ( "John", "Smith", "johnkm", 35000 ) );
    //assert ( b1 . add ( "Joh", "Smithq", "qaws", 35000 ) );





    assert ( b1 . getSalary ( "peter" ) ==  23000 );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );

    assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "James"
             && outSurname == "Bond" );
    assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "james" ) ==  23000 );
    assert ( b1 . getSalary ( "peter" ) ==  0 );
    assert ( b1 . del ( "james" ) );

    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 1 );

    assert ( b1 . del ( "John", "Miller" ) );

    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 0 );

    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . del ( "john" ) );
    assert ( ! b1 . getFirst ( outName, outSurname ) );
    assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );


    CPersonalAgenda b2;
    assert ( ! b2 . getFirst ( outName, outSurname ) );
    assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );

    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . setSalary ( "joe", 90000 ) );
    assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . getSalary ( "joe" ) ==  0 );



    assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . getRank ( "joe", lo, hi ) );

    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );

    assert ( ! b2 . del ( "Joe", "Black" ) );
    assert ( ! b2 . del ( "joe" ) );
/*
    b2.printEmail();
    b2.printSurname();
    b2.printSalary();

    cout << "++++++++++++++++++++++++" << endl;
    b2.printEmail();
    b2.printSurname();
    b2.printSalary();
*/
        assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );

    assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );



    assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );





    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . setSalary ( "peter", 37000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  0 );
    assert ( ! b2 . getRank ( "peter", lo, hi ) );
    assert ( ! b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . del ( "peter" ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  40000 );



    CPersonalAgenda b3;
    assert ( b3 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b3 . add ( "AB", "ab", "AAA", 42000 ) );
    assert ( b3 . add ( "wc", "w", "we", 40000 ) );
    assert ( b3 . add ( "AA", "ab", "aa", 45600 ) );
/*
    b3.printEmail();
    b3.printSurname();
    b3.printSalary();


    b3.printEmail();
    b3.printSurname();
    b3.printSalary();
*/
    assert ( b3 . add ( "wb", "w", "war", 40000 ) );
    assert ( b3 . add ( "wa", "w", "wae", 40000 ) );
    assert ( b3 . changeName ( "war", "wwww", "wwww" ) );



/*

    cout << "++++++++++++++++++++++++" << endl;
    b3.printEmail();
    b3.printSurname();
    b3.printSalary();
*/

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
