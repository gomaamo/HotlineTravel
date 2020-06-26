/*******************************************************
** Aufgabe Nr.: H5                                    **
** Nachname: Gomaa                                    **
** Vorname: Mohamed                                   **
** Matrikelnummer: 3072697                            **
** Uni-Email: mohamed.gomaa@stud.uni-due.de           **
** Studiengang: ISE CE (SWE)                          **
*******************************************************/

#include<iostream>
#include<iomanip>
#include<string>

using namespace std;

class Date{
private:
    short unsigned int day, month, year;
    int daysInMonth[12]={31,28,31,30,31,30,31,31,30,31,30,31};
public:
    Date(){
        day = month = year = 0;
    }

    Date(int d, int m, int y){
        day=d;
        month=m;
        year=y;
    }

    Date operator+(int n){
        int n_day;
        Date newDate;
        n_day=n+day;
        if(n_day>daysInMonth[month-1] && month!=12){
            newDate.day=n_day-daysInMonth[month-1];
            newDate.month=month+1;
            newDate.year=year;
        }
        else if(n_day>daysInMonth[month-1] && month==12){
            newDate.day=n_day-daysInMonth[month-1];
            newDate.month=1;
            newDate.year=year+1;
        }
        else{
            newDate.day=n_day;
            newDate.month=month;
            newDate.year=year;
        }
        return newDate;
    }

    friend istream &operator>>(istream &input, Date &D);
    friend ostream &operator<<(ostream &output, Date &D);
};

istream &operator>>(istream &input, Date &D){
         int d,m,y;
         char sep;
         input >> d >> sep >> m >> sep >> y;
         if(d<=31 && m<=12){
            D.day=d;
            D.month=m;
            D.year=y;
            return input;
         }
         else if(d>31 && m<=12){
            D.day=31;
            D.month=m;
            D.year=y;
            return input;
         }
         else if(m>12 && d<=31){
            D.day=d;
            D.month=12;
            D.year=y;
            return input;
         }
         else if(d>31 && m>12){
            D.day=31;
            D.month=12;
            D.year=y;
            return input;
         }
      }

ostream &operator<<(ostream &output, Date &D){
    if(D.day<10 && D.month<10){
        return output << "0" << D.day << ".0" << D.month << "." << D.year;
    }
    else if(D.day<10){
        return output << "0" << D.day << "." << D.month << "." << D.year;
    }
    else if(D.month<10){
        return output << D.day << ".0" << D.month << "." << D.year;
    }
    else{
        return output << D.day << "." << D.month << "." << D.year;
    }
}

enum Board{all, breakfast, halfBoard, noMeals};

istream &operator>>(istream &input, Board &B){
    char c;
    input >> c;
    switch(c){
        case 'a': B=all; break;
        case 'b': B=breakfast; break;
        case 'h': B=halfBoard; break;
        case 'w': B=noMeals; break;
        default: B=noMeals;
    }
    return input;
}

ostream &operator<<(ostream &output, Board &B){
    switch(B){
        case all: output << "all inclusive"; break;
        case breakfast: output << "with breakfast"; break;
        case halfBoard: output << "half board"; break;
        case noMeals: output << "without meals"; break;
    }
    return output;
}

class Hotel{
private:
    string name;
    Date arrival;
    int nights, singles, doubles;
    Board boardtype;
    double priceNightSingle, priceNightDouble;
    bool parking;
public:
    Hotel(){
        cout << "name of hotel: ";
        cin.sync();
        getline(cin, name);
        cout << "arrival on: ";
        cin >> arrival;
        int n;
        cout << "how many nights: ";
        cin >> n;
        if(n<=28){ nights=n; }
        else{ nights=28; }
        cout << "how many single bed rooms: ";
        cin >> singles;
        cout << "how many double bed rooms: ";
        cin >> doubles;
        cout << "(a) all inclusive \n(b) breakfast \n(h) half board \n(w) without meals \n>>";
        cin >> boardtype;
        cout << "price of one night in single bed room: ";
        cin >> priceNightSingle;
        cout << "price one night in double bed room: ";
        cin >> priceNightDouble;
        char p;
        cout << "with parking (y(es) or n(o)): ";
        cin >> p;
        if(p=='y'){ parking=true; }
        else{ parking=false; }
    }

    ~Hotel(){
        cout << "destructor hotel " << name << " at " << arrival
             << " for " << get_guests() << " guest(s) done." << endl;
    }

    double get_price(void){
        if(parking==true){
            return nights*(singles*priceNightSingle + doubles*priceNightDouble + 10);
        }
        else{
            return nights*(singles*priceNightSingle + doubles*priceNightDouble);
        }
    }

    Date get_arrival(void){
        return arrival;
    }

    Date get_checkout(void){
        return arrival+nights;
    }

    int get_guests(void){
        return singles+2*doubles;
    }

    void print(void){
            cout << "check-in: " << arrival << " hotel " << name
                 << " for " << nights << " night(s) ";
                 if(doubles!=0){
                    cout << doubles << " double bed room(s)";
                 }
                 else{ cout << ""; }
                 if(doubles!=0 && singles!=0){ cout << " and "; }
                 else{ cout << ""; }
                 if(singles!=0){
                    cout << singles << " single bed room(s)\n";
                 }
                 else{ cout << endl; }
                 cout << "\t\t\t\t" << boardtype;
                 if(parking!=false){
                    cout << ", parking included" << endl;
                 }
                 else{ cout << endl; }
    }
};

class Transport{
public:
    virtual ~Transport(){
        cout << "destructor Transport done." << endl; }

    virtual bool withTransfer(void)=0;

    virtual double get_price(void)=0;

    virtual void print(void)=0;
};

class SelfOrganised:public Transport{
public:
    SelfOrganised(){}

    virtual ~SelfOrganised(){
        cout << "destructor SelfOrganised done." << endl; }

    virtual bool withTransfer(void){
        return false; }

    virtual double get_price(void){
        return 0; }

    virtual void print(void){
        cout << "self organised transport" << endl; }
};

class PublicTransport:public Transport{
protected:
    Date departure;
    string code, from, to;
    double priceOneSeat;
    bool firstClass;
public:
    PublicTransport(Date dep, string c, string f, string t, double pr, bool fc=false){
        departure=dep;
        code=c;
        from=f;
        to=t;
        priceOneSeat=pr;
        firstClass=fc;
    }

    virtual ~PublicTransport(){
        cout << "destructor PublicTransport " << code << " at " << departure << " done." << endl;
    }

    virtual void print(void){
        cout << departure << " " << code << " from " << from << " to " << to << endl;
    }
};

class Flight:public PublicTransport{
private:
    bool transfer;
public:
    Flight(Date dep, string c, string f, string t, double pr,
           bool fc=false, bool tran=true):PublicTransport(dep,c,f,t,pr,fc){
        transfer=tran;
    }

    virtual ~Flight(){
        cout << "destructor Flight done." << endl;
    }

    virtual bool withTransfer(void){
        return transfer;
    }

    virtual double get_price(void){
        if(firstClass==false){
            return priceOneSeat;
        }
        else{ return 2*priceOneSeat; }
    }

    virtual void print(void){
        cout << "flight ";
        PublicTransport::print();
    }
};

class Train:public PublicTransport{
public:
    Train(Date dep, string c, string f, string t, double pr,
           bool fc=false):PublicTransport(dep,c,f,t,pr,fc){}

    virtual ~Train(){
        cout << "destructor Train done." << endl;
    }

    virtual bool withTransfer(void){
        return false;
    }

    virtual double get_price(void){
        if(firstClass==false){
            return priceOneSeat;
        }
        else{ return 1.5*priceOneSeat; }
    }

    virtual void print(void){
        cout << "train ";
        PublicTransport::print();
    }
};

class Trip{
private:
    unsigned int no;
    static unsigned int lastNo;
    unsigned int travellers;
    Hotel *hotel;
    Transport *transportOutward, *transportBack;
    Trip* nxtrip;
public:
    Trip(int trav, Hotel *h, Transport *tOut, Transport *tBack, Trip *nx=nullptr){
        travellers=trav;
        hotel=h;
        transportOutward=tOut;
        transportBack=tBack;
        nxtrip=nx;
        ++lastNo;
        no=lastNo;
    }

    virtual ~Trip(){
        delete hotel;
        delete transportOutward;
        delete transportBack;
        cout << "destructor Trip done." << endl;
    }

    int get_no(void){
        return no;
    }

    Trip* get_next(void){
        return nxtrip;
    }

    void set_next(Trip* nx){
        nxtrip=nx;
    }

    double get_price(void){
        return (hotel->get_price()
                + travellers*(transportOutward->get_price()+transportBack->get_price()));
    }

    void print(void){
        cout << "trip inquiry " << get_no() << " for " << travellers << " person(s)" <<endl;
        hotel->print();
        cout << "outward journey: ";
        transportOutward->print();
        cout << "journey back: ";
        transportBack->print();
        if(transportOutward->withTransfer()==true || transportBack->withTransfer()==true){
            cout << "transfer included" << endl;
        }
        else{
            cout << "no transfer" << endl;
        }
        cout << "\t\t\t price: " << fixed << setprecision(2) << get_price() << " EUR" << endl << endl;
    }
};

unsigned int Trip::lastNo=0;

class TravelAgency{
private:
    Trip* trips;
public:
    TravelAgency(){
        trips=nullptr;
    }

    void add(Trip* t){
        if (trips == nullptr){
        trips = t;
        trips->set_next(nullptr);
        }
        else{
            Trip *temp = trips;
            while (temp->get_next() != nullptr){
                temp = temp->get_next();
            }
            temp->set_next(t);
            t->set_next(nullptr);
        }
    }

    void removetrip(Trip* r){
        if(r == trips){
            if(trips->get_next() == nullptr){
                delete r;
                trips=nullptr;
            }
            else{
                trips = trips->get_next();
                delete r;
            }
        }
        else{
            Trip *prev = trips;
            while(prev->get_next() != nullptr && prev->get_next() != r){
                prev = prev->get_next();
            }
            prev->set_next(prev->get_next()->get_next());
            delete r;
        }
    }

    Trip* searchTrip(int n){
        if (trips != nullptr){
            if (trips->get_no() == n){
                return trips;
            }
            else{
                Trip *i = trips->get_next();
                while (i != nullptr){
                    if (i->get_no() == n){
                        return i;
                    }
                    i = i->get_next();
                }
                return nullptr;
            }
        }
        else{
            return nullptr;
        }
    }
    void printAllTrips(void){
        if(trips!=nullptr){
            Trip* curr;
            curr=trips;
            while(curr != nullptr){
                curr->print();
                curr=curr->get_next();
            }
        }
        else{
            cout << "There are no trips." << endl;
        }
    }
};

int main(void){
    int press;
    TravelAgency TA;
    do{
        cout << "HOTLINE TRAVEL AGENCY" << endl
             << "(0) end" << endl
             << "(1) new trip" << endl
             << "(2) search trip" << endl
             << "(3) show all trip offers" << endl
             << ">> ";
        cin >> press;

        switch(press){
            case 1: {
                    Hotel *h=new Hotel();
                    Date arrdate = h->Hotel::get_arrival();
                    Date backdate= h->Hotel::get_checkout();
                    int guests= h->Hotel::get_guests();
                    Transport *tOut,*tBack;
                    Trip* newtrip;
                    int choice;
                    string c,f,t;
                    double pr;
                    cout << "please choose transport for outward journey" << endl
                         << "(0) self organised" << endl << "(1) by flight" << endl
                         << "(2) by train" << endl << ">> ";
                    cin >> choice;
                    switch(choice){
                        case 0: { tOut= new SelfOrganised(); break;}
                        case 1: {
                                cout << "code of flight: ";
                                cin.sync();
                                getline(cin, c);
                                cout << "airport of departure: ";
                                cin.sync();
                                getline(cin, f);
                                cout << "airport of arrival: ";
                                cin.sync();
                                getline(cin, t);
                                cout << "price for one passenger: ";
                                cin >> pr;
                                tOut = new Flight(arrdate,c,f,t,pr);
                                break;
                                }
                        case 2: {
                                cout << "code of train: ";
                                cin.sync();
                                getline(cin, c);
                                cout << "main train station of departure: ";
                                cin.sync();
                                getline(cin, f);
                                cout << "main train station of arrival: ";
                                cin.sync();
                                getline(cin, t);
                                cout << "price for one passenger: ";
                                cin >> pr;
                                tOut= new Train(arrdate,c,f,t,pr);
                                break;
                                }
                    }

                   cout << "please choose transport for journey back" << endl
                         << "(0) self organised" << endl << "(1) by flight" << endl
                         << "(2) by train" << endl << ">> ";
                    cin >> choice;
                    switch(choice){
                        case 0: {tBack = new SelfOrganised(); break;}
                        case 1: {
                                cout << "code of flight: ";
                                cin.sync();
                                getline(cin, c);
                                cout << "airport of departure: ";
                                cin.sync();
                                getline(cin, f);
                                cout << "airport of arrival: ";
                                cin.sync();
                                getline(cin, t);
                                cout << "price for one passenger: ";
                                cin >> pr;
                                tBack = new Flight(backdate,c,f,t,pr);
                                break;
                                }
                        case 2: {
                                cout << "code of train: ";
                                cin.sync();
                                getline(cin, c);
                                cout << "main train station of departure: ";
                                cin.sync();
                                getline(cin, f);
                                cout << "main train station of arrival: ";
                                cin.sync();
                                getline(cin, t);
                                cout << "price for one passenger: ";
                                cin >> pr;
                                tBack = new Train(backdate,c,f,t,pr);
                                break;
                                }
                    }
                    newtrip = new Trip(guests,h,tOut,tBack);
                    TA.add(newtrip);
                    cout << endl << endl;
                    break;
                    }


            case 2: {
                    int n;
                    char c;
                    cout << "number of trip: ";
                    cin >> n;
                    if(TA.searchTrip(n)==nullptr){
                        cout << "trip not found" << endl << endl;
                    }
                    else{
                        TA.searchTrip(n)->print();
                        cout << "(d)elete or (n)ot: ";
                        cin >> c;
                        if(c=='d' || c=='D'){
                            TA.removetrip(TA.searchTrip(n));
                        }
                        cout << endl;
                    }
                    }
                    break;

            case 3: {
                    TA.printAllTrips();
                    cout << endl;
                    }
                    break;

            default: { cout << endl; } break;
        }
    }while(press != 0);

    return 0;
}
