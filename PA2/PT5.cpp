#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
  public:
    //---------------------------------------------------------------------------------------------
                             CDate                         ( int               y,
                                                             int               m,
                                                             int               d )
      : m_Year ( y ),
        m_Month ( m ),
        m_Day ( d )
    {
    }
    //---------------------------------------------------------------------------------------------
    int                      compare                       ( const CDate     & x ) const
    {
      if ( m_Year != x . m_Year )
        return m_Year - x . m_Year;
      if ( m_Month != x . m_Month )
        return m_Month - x . m_Month;
      return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int                      year                          ( void ) const 
    {
      return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int                      month                         ( void ) const 
    {
      return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int                      day                           ( void ) const 
    {
      return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CDate     & x )
    {
      char oldFill = os . fill ();
      return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Day ) 
                << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
  private:
    int16_t                  m_Year;
    int8_t                   m_Month;
    int8_t                   m_Day;
};
#endif /* __PROGTEST__ */

//-----------------  CInvoice ---------------------//
/** Class represents invoice. */
class CInvoice
{
  public:
    /**
     *Constructor that initializes the member variables of the CInvoice.
     * @param date object of CDate, represent invoices date.
     * @param seller name of company, which sell.
     * @param buyer name of company, which buy.
     * @param amount how much money paid buyer.
     * @param vat value added tax.
     */
    CInvoice (const CDate & date, const string & seller, const string & buyer, unsigned int amount, double vat);
    /**
     * Operator that compares two CInvoice objects,
     * based on their date, seller name, buyer name, invoice amount, and VAT rate.
     * @param oth second CInvoice object.
     * @return true if all of these fields are equal, otherwise false.
     */
    bool operator == (const CInvoice & oth) const;
    /**
     * Overloaded < operator that compares two CInvoice objects.
     * The comparison is based on the invoice date, seller name, buyer name, invoice amount, and VAT rate.
     * @param other second CInvoice object for comparing.
     * @return true if the left operand is less than the right operand, otherwise false.
     */
    bool operator < (const CInvoice & other) const;
    /**
     * Overloaded << operator that is used to output the contents of an CInvoice object to an output stream.
     * It prints the invoice date, seller name, buyer name, invoice amount, and VAT rate.
     * For self control.
     * @param os output stream.
     * @param inv object for streaming.
     * @return output stream.
     */
    friend ostream & operator << (ostream & os, const CInvoice & inv);
    /**
     * Getter method.
     * @return the invoice date.
     */
    CDate date (void) const;
    /**
     *  Getter method.
     * @return the name of the buyer
     */
    string buyer (void) const;
    /**
     *  Getter method.
     * @return the name of the seller
     */
    string seller (void) const;
    /**
     * Getter method.
     * @return the invoice amount.
     */
    unsigned int amount (void) const;
    /**
     * Getter method.
     * @return  the VAT rate.
     */
    double vat (void) const;
    /**
     * Setter method that sets the seller name.
     * @param name new name of seller.
     */
    void setSeller (string & name);
    /**
     * Setter method that sets the buyer name.
     * @param name new name of byuer.
     */
    void setBuyer (string & name);
    /**
     * Setter method that sets the counter of the CInvoice object.
     * @param srs counter
     */
    void setCounter (size_t srs);
    /**
     * Getter method.
     * @return the counter of the CInvoice object
     */
    size_t getCounter () const;

  private:
    /** object of invoices date  */
    CDate m_date = CDate (6,6,6);
    /** the name of the seller. */
    string m_seller;
    /** the name of the buyer.  */
    string m_buyer;
    /** the amount of the invoice. */
    unsigned int m_amount;
    /** the VAT amount of the invoice.  */
    double m_vat;
    /** A counter used for internal purposes, check order */
    size_t m_counter;
};

 ostream & operator << (ostream & os, const CInvoice & inv){
    os << "Date: " << inv.date() << ", Seller: " << inv.seller()  << ", Buyer: " << inv.buyer()
       << ", Amount: " << inv.amount() << ", VAT: " << inv.vat();
    return os;
}

bool CInvoice::operator < (const CInvoice & other) const{
    int date_cmp = m_date.compare(other.m_date);
    if (date_cmp < 0)
        return true;
    else if (date_cmp == 0){
        if (m_seller < other.m_seller)
            return true;
        else if (m_seller == other.m_seller){
            if (m_buyer < other.m_buyer)
                return true;
            else if (m_buyer == other.m_buyer){
                if (m_amount < other.m_amount)
                    return true;
                else if (m_amount == other.m_amount)
                    return m_vat < other.m_vat;
            }
        }
    }
    return false;
}

CInvoice::CInvoice (const CDate & date, const string & seller, const string & buyer, unsigned int amount, double vat){
    this->m_date = date;
    this->m_seller = seller;
    this->m_buyer = buyer;
    this->m_amount = amount;
    this->m_vat = vat;
    this->m_counter = 0;
}

bool CInvoice::operator == (const CInvoice & oth) const{
    if (m_date.compare(oth.m_date) != 0)
        return false;
    if (m_seller != oth.seller() || m_buyer != oth.buyer() || m_amount != oth.amount() || m_vat != oth.vat())
        return false;
    return true;

}

void CInvoice::setSeller (string & name){
     m_seller = name;
 }
void CInvoice::setBuyer (string & name){
     m_buyer = name;
 }

CDate CInvoice::date (void) const{
    return m_date;
}
string CInvoice::buyer (void) const{
    return m_buyer;
}
string CInvoice::seller (void) const{
    return m_seller;
}
unsigned int CInvoice::amount (void) const{
    return m_amount;
}
double CInvoice::vat (void) const{
    return m_vat;
}
void CInvoice::setCounter (size_t src){
     m_counter = src;
 }
size_t CInvoice::getCounter () const{
    return m_counter;
}


// ---------------------------   CSortOpt   ------------------------------//
/** The CSortOpt class specifies the sorting criteria. */
class CSortOpt
{
  public:
    /** @var BY_DATE, @var BY_BUYER, @var BY_SELLER, @var BY_AMOUNT, @var  BY_VAT
     * are five static integer constants representing different sorting options. */
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    /** The default constructor for the class. */
    CSortOpt();
    /**
     *This will add two keys to the m_keys vector
     * @param key representing the sorting option that should be added to the list of keys to sort by.
     * @param ascending is an optional boolean value that specifies whether the sorting should be done
     * in ascending order or descending order (default value is true).
     * @return reference to the CSortOpt
     */
    CSortOpt & addKey (int key, bool ascending);
    /** Is a vector that holds the list of sorting keys. */
    vector <pair <int, bool>> m_keys;
};

CSortOpt::CSortOpt (){}

CSortOpt & CSortOpt::addKey (int key, bool ascending = true){
    m_keys.push_back(make_pair( key, ascending));
    return *this;
}

/**
 * This is a hash function for the CInvoice class.
 *  It takes an instance of CInvoice as input
 *  and returns a size_t hash value based on the contents of the CInvoice object.
 */
struct CInvoiceHash{
    size_t operator () (const CInvoice & src) const{
        size_t h1 = hash <int>() (src.date().year());
        size_t h2 = hash <int>() (src.date().month());
        size_t h3 = hash <int>() (src.date().day());
        size_t h4 = hash <string>() (src.buyer());
        size_t h5 = hash <string>() (src.seller());
        size_t h6 = hash <unsigned int>() (src.amount());
        size_t h7 = hash <double>() (src.vat());
        return h1 ^ h2 ^ h3 ^ h4 ^ h5 ^ h6 ^ (h7 << 1);
    }
};


//-------------------- CVATRegister --------------------//
/**  Class manages VAT invoices issued and accepted by different companies. */
class CVATRegister
{
  public:
    /** Default constructor that initializes the c_cvatOrder member to 0. */
    CVATRegister ();
    /**
     * Registers a company with the given name in the system.
     * @param name name of company.
     * @return true if registration completed successfully.
     */
    bool registerCompany (const string & name);
    /**
     * Adds an issued invoice to the system.
     * @param x object CInvoice for adding.
     * @return true if the invoice was added successfully and false if it already exists in the system( issued invoice).
     */
    bool addIssued (const CInvoice & x);
    /**
     * Adds an accepted invoice to the system.
     * @param x object CInvoice for adding.
     * @return true if the invoice was added successfully and false if it already exists in the system(accepted invoice).
     */
    bool addAccepted (const CInvoice & x);
    /**
     * Removes an issued invoice from the system.
     * @param x invoice for removing from accepted system.
     * @return true if the invoice was removed successfully and false if it does not exist in the system.
     */
    bool delIssued (const CInvoice & x);
    /**
     * Removes an accepted invoice from the system.
     * @param x invoice for removing from issued system.
     * @return true if the invoice was removed successfully and false if it does not exist in the system.
     */
    bool delAccepted (const CInvoice & x);
    /**
     * Find unmatched invoices and sort by specification.
     * @param company for which company find unmatched invoices.
     * @param sortBy specification for sorting.
     * @return a list of unmatched invoices for the given company, sorted according to the specified sort options.
     */
    list<CInvoice> unmatched (const string & company, const CSortOpt & sortBy) const;
    //private:
    /**  Keeps track of the order in which invoices were added to the system. */
    size_t c_cvatOrder;
    /**
     * Corrects the name of a company, delete whitespaces and transform chars to lowercase.
     * @param src name of company for transforming.
     * @return transformed name of company.
    */
    string ccorreccttName (const string & src) const;
    /** An unordered map that maps the corrected name of a company to its default name. */
    unordered_map <string, string> registerCorrectDefault;
    /** A map that maps the corrected name of a company to a set of its unmatched invoices. */
    map <string, set<CInvoice>> unmatchedCompanies;
    /** An unordered set that stores the issued invoices in the system. */
    unordered_set <CInvoice, CInvoiceHash> invoicesIssued;
    /** An unordered set that stores the accepted invoices in the system. */
    unordered_set <CInvoice, CInvoiceHash> invoicesAccepted;

    /**
     * Add invoices from the system.
     * @param x invoice for adding to system.
     * @param invoices1 for adding issued invoice use @var invoicesIssued, otherwise @var invoicesAccepted.
     * @param invoices2 for adding accepted invoice use @var invoicesAccepted, otherwise @var invoicesIssued.
     * @return true if added successfully, otherwise false.
     */
    bool addInvoiceHelper (const CInvoice & x, unordered_set <CInvoice, CInvoiceHash> & invoices1,
                                               unordered_set <CInvoice, CInvoiceHash> & invoices2);
    /**
     * Remove invoices from the system.
     * @param x invoice for removing to system.
     * @param invoices1 for removing issued invoice use @var invoicesIssued, otherwise @var invoicesAccepted.
     * @param invoices2 for removing accepted invoice use @var invoicesAccepted, otherwise @var invoicesIssued.
     * @return
     */
    bool delInvoiceHelper (const CInvoice & x, unordered_set <CInvoice, CInvoiceHash> & invoices1,
                                               unordered_set <CInvoice, CInvoiceHash> & invoices2);

};

CVATRegister::CVATRegister (){
    c_cvatOrder = 0;
}

list<CInvoice> CVATRegister::unmatched (const string & company, const CSortOpt  & sortBy ) const{
    list<CInvoice> result;

    // check if the company is registered and get the corrected name
    string correctedName = ccorreccttName(company);
    if (registerCorrectDefault.find(correctedName) == registerCorrectDefault.end())
        return result;

    // get the set of unmatched invoices for the company
    const set<CInvoice> & unmatchedInvoices = unmatchedCompanies.at(correctedName);

    // copy the set of unmatched invoices to a vector to allow sorting
    vector<CInvoice> unmatchedVector(unmatchedInvoices.begin(), unmatchedInvoices.end());


    for ( CInvoice & inv : unmatchedVector) {
        string resSel = registerCorrectDefault.find(inv.seller())->second ;
        inv.setSeller(resSel);
        string resBuy = registerCorrectDefault.find(inv.buyer())->second;
        inv.setBuyer(resBuy);
    }


    // inspiration from https://stackoverflow.com;
    // sort the vector according to the keys in the sortBy object
    sort(unmatchedVector.begin(), unmatchedVector.end(),
         [&](const CInvoice & inv1, const CInvoice & inv2)->bool{
             if (sortBy.m_keys.empty())
                return (inv1.getCounter() < inv2.getCounter());

             for (auto [key, ascending] : sortBy.m_keys){
                 switch (key) {
                     case CSortOpt::BY_DATE:
                         if (inv1.date().compare(inv2.date()) < 0)
                             return ascending;
                         else if (inv1.date().compare(inv2.date()) > 0)
                             return !ascending;
                         break;
                     case CSortOpt::BY_SELLER: {
                         string s1 = inv1.seller();
                         string s2 = inv2.seller();
                         std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
                         std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
                         if (s1 < s2)
                             return ascending;
                         else if (s1 > s2)
                             return !ascending;
                         break;
                     }case CSortOpt::BY_BUYER:{
                         string s1 = inv1.buyer();
                         string s2 = inv2.buyer();
                         std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
                         std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
                         if (s1 < s2)
                             return ascending;
                         else if (s1 > s2)
                             return !ascending;
                         break;
                     }case CSortOpt::BY_AMOUNT:
                         if (inv1.amount() < inv2.amount())
                             return ascending;
                         else if (inv1.amount() > inv2.amount())
                             return !ascending;
                         break;
                     case CSortOpt::BY_VAT:
                         if (inv1.vat() < inv2.vat())
                             return ascending;
                         else if (inv1.vat() > inv2.vat())
                             return !ascending;
                         break;
                     default:
                         if (inv1.getCounter() < inv2.getCounter())
                             return ascending;
                         else if (inv1.getCounter() > inv2.getCounter())
                             return !ascending;
                         break;
                 }
             }
             //return false;
             return inv1.getCounter() < inv2.getCounter();
         });

    // copy the sorted vector back to a list and return it
    for ( CInvoice & inv : unmatchedVector) {
       /* string resSel = registerCorrectDefault.find(inv.seller())->second ;
        inv.setSeller(resSel);
        string resBuy = registerCorrectDefault.find(inv.buyer())->second;
        inv.setBuyer(resBuy); */
        result.push_back(inv);
    }

    return result;
}

bool CVATRegister::delInvoiceHelper (const CInvoice & x, unordered_set <CInvoice, CInvoiceHash> & invoices1,
                                                         unordered_set <CInvoice, CInvoiceHash> & invoices2){

    string correctedSeller = ccorreccttName(x.seller());
    string correctedBuyer  = ccorreccttName(x.buyer());

    // check if companies are registered
    if (registerCorrectDefault.find(correctedSeller) == registerCorrectDefault.end()
     || registerCorrectDefault.find(correctedBuyer)  == registerCorrectDefault.end())
        return false;

    CInvoice tmpInvoice = CInvoice (x.date(), correctedSeller, correctedBuyer, x.amount(), x.vat());

    // check if invoice is registered in current system.
    auto resSel = invoices1.find(tmpInvoice);
    if (resSel == invoices1.end())
        return false;
    else
        invoices1.erase(resSel);

    // find iterators to position in system and trying to insert current invoice as unmatched

    auto itrS = unmatchedCompanies.find(correctedSeller);
    auto itrB = unmatchedCompanies.find(correctedBuyer);


    if (itrS != unmatchedCompanies.end() && !itrS->second.count(tmpInvoice) )
        itrS->second.insert(tmpInvoice);
    if (itrB != unmatchedCompanies.end() &&   !itrB->second.count(tmpInvoice) )
        itrB->second.insert(tmpInvoice);

    if (!invoices2.count(tmpInvoice)){
        unmatchedCompanies[correctedSeller].erase(tmpInvoice);
        unmatchedCompanies[correctedBuyer].erase(tmpInvoice);
    }
    return true;
}


bool CVATRegister::delAccepted (const CInvoice & x){
    return delInvoiceHelper(x, invoicesAccepted, invoicesIssued);
}

bool CVATRegister::delIssued (const CInvoice & x){
    return delInvoiceHelper(x, invoicesIssued, invoicesAccepted);
}

bool CVATRegister::addInvoiceHelper(const CInvoice & x, unordered_set <CInvoice, CInvoiceHash> & invoices1,
                                                        unordered_set <CInvoice, CInvoiceHash> & invoices2){

    string correctedSeller = ccorreccttName(x.seller());
    string correctedBuyer  = ccorreccttName(x.buyer());

    CInvoice tmpInvoice = CInvoice (x.date(), correctedSeller,
                                    correctedBuyer, x.amount(), x.vat());
    ++c_cvatOrder;
    tmpInvoice.setCounter(c_cvatOrder);

    if (registerCorrectDefault.find(correctedSeller) == registerCorrectDefault.end() ||
        registerCorrectDefault.find(correctedBuyer) == registerCorrectDefault.end() ||
        correctedSeller == correctedBuyer || invoices1.count(tmpInvoice))
        return false;

    auto itS = unmatchedCompanies.find(correctedSeller);
    auto itB = unmatchedCompanies.find(correctedBuyer);

    if (!invoices2.count(tmpInvoice)){
        (itS != unmatchedCompanies.end() ? itS->second : unmatchedCompanies[correctedSeller]).insert(tmpInvoice);
        (itB != unmatchedCompanies.end() ? itB->second : unmatchedCompanies[correctedBuyer]).insert(tmpInvoice);
    } else{
        if (itS != unmatchedCompanies.end())  itS->second.erase(tmpInvoice);
        if (itB != unmatchedCompanies.end()) itB->second.erase(tmpInvoice);
    }
    invoices1.insert(tmpInvoice);
    return true;
}

bool CVATRegister::addAccepted (const CInvoice & x){
    return addInvoiceHelper(x, invoicesAccepted, invoicesIssued);
}
bool  CVATRegister::addIssued  (const CInvoice & x){
    return addInvoiceHelper(x, invoicesIssued, invoicesAccepted);
}

bool CVATRegister::registerCompany (const string & name){
    string correctedName = ccorreccttName(name);
    // Control if register contains name of company
    if (!registerCorrectDefault.insert({correctedName, name}).second)
        return false;

    map <string, set<CInvoice>> unmatchedCompanies;

    //unmatchedCompanies.insert ( { correctedName, map<string ,  set<CInvoice>>() } );
    return true;
}

// takes a string as input and returns a transformed version of it
string CVATRegister::ccorreccttName(const string & src) const {
    string tmpN;
    // Reserve memory for the new string to avoid multiple reallocations
    tmpN.reserve(src.size());
    bool flg = false;
    // iterate over each character in the input string
    for (char c : src){
        // if the previous character was not a space, add a single space to the new string and set the flag
        if (isspace(c)){
            if (!flg ) {
                tmpN.push_back(' ');
                flg = true;
            }
        } else{
            // if the character is not a space, add it to the new string and reset the flag
            tmpN.push_back(c);
            flg = false;
        }
    }
    // find the first non-space character in the new string
    size_t frst = 0;
    while (frst < tmpN.size() && isspace(tmpN[frst]))
        ++frst;
    // find the last non-space character in the new string
    size_t last = tmpN.size() - 1;
    while (last > 0 && isspace(tmpN[last]))
        --last;
    // Extract the substring from the first non-space character to the last non-space character and update the new string
    tmpN = tmpN.substr(frst, last - frst + 1);
    // Convert all characters in the new string to lowercase
    for (char& c : tmpN)
        c = tolower(c);
    return tmpN;
}



#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{

    if ( a.size() != b.size() )
        return false;

    std::vector<CInvoice> v1;
    std::vector<CInvoice> v2;

    for ( auto it = a.begin(); it != a.end(); ++it )
        v1.push_back ((*it));
    for ( auto it = b.begin(); it != b.end(); ++it )
        v2.push_back ((*it));

    for ( size_t i = 0; i < a.size(); i++ ) {
        if ( ! ( v1[i].date().year() == v2[i].date().year() &&
                 v1[i].date().month() == v2[i].date().month() &&
                 v1[i].date().day() == v2[i].date().day() &&
                 v1[i].buyer() == v2[i].buyer() && v1[i].seller() == v2[i].seller() &&
                 v1[i].amount() == v2[i].amount() && v1[i].vat() == v2[i].vat() ) )
            return false;
    }
    return true;

}

int main ( void )
{

    //string qwe = "    JHKH jjkkKL      OIergLJknkalK      " ;
  CVATRegister r;

  assert ( r . registerCompany ( "first Company" ) );

  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );

  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );



    assert (equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true )
   . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );

  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );


    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
             } ) );

    assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
             list<CInvoice>
             {
             } ) );


  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );




  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );

  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );

  /*
    for (const auto& pair : r.unmatchedCompanies) {
        std::cout << "Company: " << pair.first << std::endl;
        std::cout << "Invoices: " << endl;

        for (const auto& invoice : pair.second) {
            std::cout << invoice << ", " << endl;
        }
        std::cout << std::endl<< endl;
    }
*/


  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );

  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );

  /*
    for (const auto& pair : r.unmatchedCompanies) {
        std::cout << "Company: " << pair.first << std::endl;
        std::cout << "Invoices: " << endl;

        for (const auto& invoice : pair.second) {
            std::cout << invoice << ", " << endl;
        }
        std::cout << std::endl<< endl;
    }
    cout << "____________________________________________________" << endl;
*/



    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );

  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );


  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );


  CVATRegister q;
    assert ( q . registerCompany ( "first Company" ) );
    assert ( q . registerCompany ( "Second     Company" ) );
    assert ( q . registerCompany ( "ThirdCompany, Ltd." ) );
    assert ( q . registerCompany ( "Third Company, Ltd." ) );
    assert ( !q . registerCompany ( "Third Company, Ltd." ) );
    assert ( !q . registerCompany ( " Third  Company,  Ltd.  " ) );
    assert ( q . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
    assert ( q . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );

    assert ( q . addAccepted( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );



   // assert ( q . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Second Company ", 100, 20 ) ) );
    //assert ( q . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", " Second Company ", 200, 30 ) ) );
/*
    for (const auto& pair : q.unmatchedCompanies) {
        std::cout << "Company: " << pair.first << std::endl;
        std::cout << "Invoices: " << endl;

        for (const auto& invoice : pair.second) {
            std::cout << invoice << ", " << endl;
        }
        std::cout << std::endl<< endl;
    }
*/
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
