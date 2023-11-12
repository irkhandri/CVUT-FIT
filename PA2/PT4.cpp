#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */



// ===============================================     CMail
class CMail
{
  public:
    CMail ( const char * from, const char * to, const char * body );
    CMail ( const CMail & m );

    ~ CMail() ;

    CMail() {
        m_from = new char [100];
        m_to   = new char [100];
        m_body = new char [1024];
    };
    CMail & operator = (const CMail & m){

            if (strlen(m.m_from) > 99){
                delete [] m_from;
                m_from = new char [strlen(m.m_from) + 1];
                strcpy(m_from, m.m_from);
            }

            else
                strcpy(m_from, m.m_from);

            if (strlen(m.m_to) > 99){
                delete [] m_to;
                m_to = new char [strlen(m.m_to) + 1];
                strcpy(m_to, m.m_to);
            }
            else
                strcpy(m_to, m.m_to);

            if (strlen(m.m_body) > 1024){
                delete [] m_body;
                m_body = new char [strlen(m.m_body) + 1];
                strcpy(m_body, m.m_body);
            }
            else
                strcpy(m_body, m.m_body);

            return *this;
    }
    bool operator == (const CMail & x) const;
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CMail     & m );

    char * m_from ;
    char * m_to;
    char * m_body;
    private:
};
CMail::CMail( const CMail & m){
    m_from = new char [strlen(m.m_from) + 1];
    m_to   = new char [strlen(m.m_to) + 1];
    m_body = new char [strlen(m.m_body) +1];

    strcpy(m_from, m.m_from);
    strcpy(m_to, m.m_to);
    strcpy(m_body, m.m_body);
}

CMail::CMail(const char *from, const char *to, const char *body){
    m_from = new char [strlen(from) + 1];
    m_to   = new char [strlen(to) + 1];
    m_body = new char [strlen(body) +1];

    strcpy(m_from, from);
    strcpy(m_to, to);
    strcpy(m_body, body);
}
CMail::~CMail() {
    delete [] m_from;
    delete [] m_to;
    delete [] m_body;
}

bool CMail::operator == (const CMail & x) const {
    return !strcmp(x.m_body, m_body) && !strcmp(x.m_from, m_from) && !strcmp(x.m_to, m_to);
}

// ===============================================     Block

struct Block {
     char  *  name;
     char  box;
    CMail * ptr;
    Block * next;
    size_t cnt = 1;

    Block (char * name,  CMail * ptr  , char  box){
        this->name = new char [strlen(name) + 1];
        strcpy(this->name, name);
        this->box = box;
        this->ptr = ptr;
    }
    Block(const Block &other)
            : Block(other.name, other.ptr, other.box)
    {}
    Block (){
        name = new char [100];
    }

    Block& operator=(Block other) {
        swap(name, other.name);
        swap(ptr, other.ptr);
        swap(cnt, other.cnt);
        swap(box, other.box);
        return *this;
    }
    ~Block() {
        delete[] name;
    }
};

// ===============================================     CMailIterator

class CMailIterator 
{
  public:
    explicit                 operator bool                 ( void ) const{
        if (elem != nullptr && sizeMail != 0)
            return true;
        else
            return false;
    }

    bool                     operator !                    ( void ) const{
        if (actual == sizeMail - 1 || elem == nullptr || sizeMail == 0)
            return true;
        else
            return false;
    }

    const CMail            & operator *                    ( void ) const ;

    CMailIterator          & operator ++                   ( void ) ;

    CMailIterator (Block * m, Block * s, int l, int dif) {
        //cout << "JEDNOU " << endl;
        box = m->box;
        sizeMail = dif;

        mailArr = new CMail[dif];
        actual = 0;
        ptrdiff_t dist = static_cast<int>(m - s);

        for (int i = 0; ;++i){
            mailArr[i] = *s[dist].ptr;
            ++dist;
            if (i == sizeMail-1) {
                break;
            }
            //this->sizeMail = i;
        }

        elem = &mailArr[0];//m->ptr;


    }



    CMailIterator() {
        this->box = '0';
        mailArr = new CMail [1];
        actual = 0;
        sizeMail = 0;
        elem = nullptr;
    };

    CMailIterator& operator=(const CMailIterator& other) {
        if (this != &other) {
            // Deallocate any resources currently held by this object
            delete[] mailArr;

            // Copy over all data members from the other object
            elem = other.elem;
            box = other.box;
            sizeMail = other.sizeMail;
            actual = other.actual;

            // Allocate new memory for the mailArr and copy its contents
            mailArr = new CMail[sizeMail];
            for (int i = 0; i < sizeMail; ++i) {
                mailArr[i] = other.mailArr[i];
            }
        }
        return *this;
    }





/*
    CMailIterator& operator=(const CMailIterator& other) {
        if (this != &other) {
            elem = other.elem;
            len = other.len;
            box = other.box;
            sizeMail = other.sizeMail;
            actual = other.actual;
            flg = other.flg;

            // Delete and reallocate mailArr if necessary
            if (sizeMail != other.sizeMail) {
                delete[] mailArr;
                mailArr = new CMail[other.sizeMail];
            }
            // Copy elements from other.mailArr
            for (int i = 0; i < other.sizeMail; i++) {
                mailArr[i] = other.mailArr[i];
            }
        }
        return *this;
    }
*/

    CMailIterator(const CMailIterator& other) {
        elem = other.elem;
        box = other.box;
        sizeMail = other.sizeMail;
        actual = other.actual;

        mailArr = new CMail[sizeMail];
        for (int i = 0; i < sizeMail; i++) {
            mailArr[i] = other.mailArr[i];
        }
    }

    CMail * elem  ;
//private:

    CMail * mailArr;
    int sizeMail ;
    int actual ;

    char box;
    //int pos;
    ~CMailIterator() {
         delete[] mailArr;
    }
};
const CMail &  CMailIterator::operator * ( void ) const{
    return mailArr[actual];//*elem;
}

CMailIterator & CMailIterator::operator ++ (void) {


   if (actual != sizeMail-1){
       ++actual;
       elem = &(mailArr[actual]);
       return *this;
   }
   else {
       elem = nullptr;
       sizeMail = 0;
       return *this;
   }


   /* if ( elem == nullptr)
        return *this;
    if (ptr->next != nullptr){
        ptr = ptr->next;
        elem = ptr->ptr;
    }
    else{
        ptr = nullptr;
        elem = nullptr;
    }
    return *this;  */
}

struct MailCnt {
    CMail mail;
    int cnt;
   MailCnt(const CMail& m, int cnt ) : mail(m), cnt(cnt) {}
    MailCnt() : mail(), cnt() {}
};
// ===============================================      CMailServer
class CMailServer 
{
  public:
                             CMailServer                   ( void );
                             CMailServer                   ( const CMailServer & src );
    CMailServer            & operator =                    ( const CMailServer & src );
                             ~CMailServer                  ( void );
    void                     sendMail                      ( const CMail     & m );
    CMailIterator            outbox                        ( const char      * email ) const;
    CMailIterator            inbox                         (const char      * email ) const;
    Block* findBlockOut(const char * name) const;

    Block* findBlock    (const char* name,  char box) const;
    Block* findLastBlock(const char* name, char box) const;

    void insertIntoSorted(Block &toInsert);

    CMail * mailPtr (int elem){
        return &mails[elem];
    }
  //private:
    // todo
    CMail * mails;
    Block * blocks;

    int lenMails;
    int sizeMails;
    int lenBlock;
    int sizeBlock;
    void addBlock( Block & block);
    void insertBlock( Block & block) ;
};
// +++++++++++++++++++++==+++++++
CMailIterator  CMailServer::outbox  (const char * email) const {
    Block *tmpB;
    tmpB = findBlock(email, 'o');
    if (tmpB == nullptr) {
        CMailIterator ret;
        ret.elem = nullptr;
        ret.sizeMail = 0;
        return ret;
    }
    Block * tmpLast = findLastBlock(email, 'o');
    ptrdiff_t dif = static_cast<int>(tmpLast - tmpB);
    CMailIterator iter = CMailIterator(tmpB, blocks, lenBlock, dif+1);
    return iter;
}

CMailIterator  CMailServer::inbox  (const char * email) const{
    Block * tmpB ;
    tmpB = findBlock(email, 'i');
    if (tmpB == nullptr){
        // CMail * smpl ;
        CMailIterator ret ;
        ret.elem = nullptr;
        ret.sizeMail = 0;
        return ret;
    }
    Block * tmpLast = findLastBlock(email, 'i');
    ptrdiff_t dif = static_cast<int>(tmpLast - tmpB);
    CMailIterator iter = CMailIterator(tmpB, blocks, lenBlock, dif +1);
    return iter;
}

Block* CMailServer::findBlock(const char* name, char box) const {
    int left = 0;
    int right = lenBlock - 1;
    Block* result = nullptr;

    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(blocks[mid].name, name);

        if (cmp < 0) {
            left = mid + 1;
        } else if (cmp > 0) {
            right = mid - 1;
        } else {
            if (blocks[mid].box == box) {
                result = &blocks[mid];
                right = mid - 1; // continue search for first mathing item
            } else if (blocks[mid].box < box) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }

    return result;
}

Block* CMailServer::findLastBlock(const char* name, char box) const {
    int left = 0;
    int right = lenBlock - 1;
    Block* result = nullptr;

    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(blocks[mid].name, name);

        if (cmp < 0) {
            left = mid + 1;
        } else if (cmp > 0) {
            right = mid - 1;
        } else {
            if (blocks[mid].box == box) {
                result = &blocks[mid];
                left = mid + 1; // continue search for last matching item
            } else if (blocks[mid].box < box) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }

    return result;
}



bool cmpq(const Block& a, const Block& b) {
    int name_cmp = strcmp(a.name, b.name);
    if (name_cmp != 0) {
        return name_cmp < 0;
    } else {
        int box_cmp = a.box > b.box;
        if (box_cmp != 0) {
            return box_cmp < 0;
        } else {
            //  names and boxes are equal sort by pointer value
            return &a < &b;
        }
    }
}

void CMailServer::insertIntoSorted(Block & toInsert) {
    int insertIndex = 0;
    while (insertIndex < lenBlock && cmpq(blocks[insertIndex], toInsert)) {
        insertIndex++;
    }
    for (int i = lenBlock - 1; i >= insertIndex; i--) {
        blocks[i+1] = blocks[i];
    }
    blocks[insertIndex] = toInsert;
    ++lenBlock;
}

void CMailServer::sendMail (const CMail & m){
    if (sizeMails - lenMails == 1){
        int newSize =  sizeMails * 1.5;
        CMail *tmp = new CMail[newSize];
        std::copy(mails, mails + lenMails, tmp);
        for (int i = 0; i < lenBlock; ++i){
            if (blocks[i].ptr != nullptr){
                ptrdiff_t dif = static_cast<int>(blocks[i].ptr - mails);
                blocks[i].ptr = &tmp[dif];
            }
        }
        delete[] mails;
        sizeMails = newSize;
        mails = tmp;
    }
    mails[lenMails] = m;
    lenMails++;
    if (sizeBlock - lenBlock <= 3){
        int newSizeB =  sizeBlock * 1.5;
        Block *tmpB = new Block[newSizeB];
        std::copy(blocks, blocks + lenBlock, tmpB);
        delete[] blocks;
        sizeBlock = newSizeB;
        blocks = tmpB;
    }
    Block tmpBlockOut = Block (m.m_from, &mails[lenMails-1], 'o');
    Block tmpBlockIn  = Block (m.m_to,   &mails[lenMails-1], 'i');
    insertIntoSorted(tmpBlockOut);
    insertIntoSorted(tmpBlockIn);

}

CMailServer::CMailServer (const CMailServer & src) {
    sizeMails = src.sizeMails;
    lenMails = src.lenMails;
    sizeBlock = src.sizeBlock;
    lenBlock = src.lenBlock;

    mails = new CMail[sizeMails];
    blocks = new Block[sizeBlock];

    std::copy(src.mails, src.mails + src.lenMails, mails);
    std::copy(src.blocks, src.blocks + src.lenBlock, blocks);

    for (int i = 0; i < lenBlock; ++i){
        if (blocks[i].ptr != nullptr){
            ptrdiff_t dif = blocks[i].ptr - src.mails;
            blocks[i].ptr = &mails[dif];
        }
    }
}

CMailServer::CMailServer (void){
    lenMails = 0;
    sizeMails = 30;
    lenBlock = 0;
    sizeBlock = 30;
    mails  = new CMail [sizeMails];
    blocks = new Block   [sizeBlock];
}

CMailServer::~CMailServer() {
    delete []  (mails);
    delete [] blocks;
}

CMailServer & CMailServer::operator = (const CMailServer & src){
    if (this != &src) {
        delete[] mails;
        delete[] blocks;
        sizeMails = src.sizeMails;
        lenMails = src.lenMails;
        sizeBlock = src.sizeBlock;
        lenBlock = src.lenBlock;
        mails = new CMail[sizeMails];
        blocks = new Block[sizeBlock];

        std::copy(src.mails, src.mails + src.lenMails, mails);
        std::copy(src.blocks, src.blocks + src.lenBlock, blocks);

        for (int i = 0; i < lenBlock; ++i){
            if (blocks[i].ptr != nullptr){
                ptrdiff_t dif = blocks[i].ptr - src.mails;
                blocks[i].ptr = &mails[dif];
            }
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const CMail& mail) {
    os << "From: " << mail.m_from << ", ";
    os << "To: " << mail.m_to << ", ";
    os << "Body: " << mail.m_body;
    return os;
}

#ifndef __PROGTEST__


bool                         matchOutput                   ( const CMail     & m,
                                                             const char      * str )
{
  ostringstream oss;
  oss << m;
  return oss . str () == str;
}






int main ( void ) {







    assert(CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
    assert(!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));
    char from[100], to[100], body[1024];

    CMail qw = CMail("john", "peter", "some important mail");
    CMail qa = qw;
    CMailServer s0;
    s0.sendMail(qa);
    strncpy(from, "john", sizeof(from));
    strncpy(to, "thomas", sizeof(to));
    strncpy(body, "another important mail", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "alice", sizeof(to));
    strncpy(body, "deadline notice", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    s0.sendMail(CMail("alice", "john", "deadline confirmation"));
    s0.sendMail(CMail("peter", "alice", "PR bullshit"));


// << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    CMailIterator i0 = s0.inbox("alice");

    for (int i = 0; i < s0.lenBlock; ++i ) {
        // cout << "NAME :" << s0.blocks[i].box << " " << s0.blocks[i].name << " Body: "<< s0.blocks[i].ptr->m_body << endl;
    }



    assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );

   assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );

     assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
     assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
     assert ( ! ++i0 );

     CMailIterator i1 = s0 . inbox ( "john" );
     assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
     assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
    assert ( ! ++i1 );

     CMailIterator i2 = s0 . outbox ( "john" );
    assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );

    assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
    assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
    assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ! ++i2 );


    CMailIterator i3 = s0 . outbox ( "thomas" );
    assert ( ! i3 );

    CMailIterator i4 = s0 . outbox ( "steve" );
    assert ( ! i4 );

    CMailIterator i5 = s0 . outbox ( "thomas" );
    s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
    assert ( ! i5 );

    for (int i = 0; i < s0.lenBlock; ++i ) {
       // cout << "NAME :" << s0.blocks[i].box << " " << s0.blocks[i].name << " Body: "<< s0.blocks[i].ptr->m_body << endl;

    }

    CMailIterator i6 = s0 . outbox ( "thomas" );
    assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
    assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
    assert ( ! ++i6 );

    CMailIterator i7 = s0 . inbox ( "alice" );
    s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
    assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
   assert ( ! ++i7 );

    CMailIterator i8 = s0 . inbox ( "alice" );
    assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ! ++i8 );

    for (int i = 0; i < s0.lenBlock; ++i ) {
     //   cout << "NAME :" << s0.blocks[i].box << " " << s0.blocks[i].name << " Body: "<< s0.blocks[i].ptr->m_body << endl;

    }


    CMailServer s1 ( s0 );
    s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
    s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );

    CMailIterator i9 = s0 . inbox ( "alice" );
    assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ! ++i9 );

    CMailIterator i10 = s1 . inbox ( "alice" );
    assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
    assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
    assert ( ! ++i10 );

    CMailServer s2;
    s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
    CMailIterator i11 = s2 . inbox ( "alice" );
    assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
    assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
    assert ( ! ++i11 );

    s2 = s0;
    s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
    s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
    CMailIterator i12 = s0 . inbox ( "alice" );
    assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
    assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
    assert ( ! ++i12 );

    CMailIterator i13 = s2 . inbox ( "alice" );
    assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
    assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
    assert ( ! ++i13 );
    assert ( !  ++i13 );

//__________________________________________

    CMailIterator i15 = s2 . inbox ( "alice" );

    CMailIterator i14 = i15;
    assert ( i14 && *i14 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( matchOutput ( *i14,  "From: john, To: alice, Body: deadline notice" ) );
    assert ( ++i14 && *i14 == CMail ( "peter", "alice", "PR bullshit" ) );
    assert ( matchOutput ( *i14,  "From: peter, To: alice, Body: PR bullshit" ) );
    assert ( ++i14 && *i14 == CMail ( "thomas", "alice", "meeting details" ) );
    assert ( matchOutput ( *i14,  "From: thomas, To: alice, Body: meeting details" ) );
    assert ( ++i14 && *i14 == CMail ( "joe", "alice", "delivery details" ) );
    assert ( matchOutput ( *i14,  "From: joe, To: alice, Body: delivery details" ) );
    assert ( ++i14 && *i14 == CMail ( "paul", "alice", "invalid invoice" ) );
    assert ( matchOutput ( *i14,  "From: paul, To: alice, Body: invalid invoice" ) );
    assert (  !++i14 );
    assert (  !++i14 );

    i13 = s2.inbox("alice");
    s2 = s1;
    assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );


cout << "____________________________" << endl;
    CMailIterator i23 = s2.inbox("alice");

    cout << "1 " << i23.mailArr[i23.actual] << endl;
    s2 = s1;
    cout << "2 " << i23.mailArr[i23.actual] << endl;



    assert ( i23 && *i23 == CMail ( "john", "alice", "deadline notice" ) );
    assert ( ++i23 && *i23 == CMail ( "peter", "alice", "PR bullshit" ) );



    CMailIterator q ;
    assert (!++q);
    assert (!++q);
    q = s2.inbox("alice");
    assert (++q);
    q = s2.outbox("alice");



    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
