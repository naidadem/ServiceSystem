#include<iostream>
using namespace std;
const char * crt = "\n----------------------------------------------\n";
char * AllocateCharArray(const char * content) {
	if (content == nullptr)
		return nullptr;
	int size = strlen(content) + 1;
	char *temp = new char[size];
	strcpy_s(temp, size, content);
	return temp;
}
class DateD {
	int *_day, *_month, *_year;
public:
	DateD(int day = 1, int month = 1, int year = 2000) {
		_day = new int(day);
		_month = new int(month);
		_year = new int(year);

	}
	DateD(const DateD & o) {
		_day = new int(*o._day);
		_month = new int(*o._month);
		_year = new int(*o._year);
	}
	DateD & operator=(const DateD & o) {
		if (this != &o) {
			delete _day;
			delete _month;
			delete _year;
			_day = new int(*o._day);
			_month = new int(*o._month);
			_year = new int(*o._year);
		}
		return *this;
	}
	~DateD() {
		delete _day;
		_day = nullptr;
		delete _month;
		_month = nullptr;
		delete _year;
		_year = nullptr;

	}
	friend ostream &operator <<(ostream &COUT, const DateD &obj) {
		COUT << *obj._day << "  " << *obj._month << "  " << *obj._year;
		return COUT;
	}
	int SumOfDays() {
		return *_day + *_month * 30 + *_year * 365;
	}
};
template <class T>
class Collection {
	T * _array;
	int _currently;
public:
	Collection() :_currently(0), _array(nullptr) {}
	~Collection() { delete[]_array; _array = nullptr; _currently = 0; }
	Collection(const Collection & o) {
		_currently = o._currently;
		_array = new T[o._currently];
		for (int i = 0; i < o._currently; i++)
		{
			_array[i] = o._array[i];
		}
	}
	Collection & operator=(const Collection & o) {
		if (this != o) {
			delete[] _array;
			_currently = o._currently;
			_array = new T[o._currently];
			for (int i = 0; i < o._currently; i++)
			{
				_array[i] = o._array[i];
			}
		}
		return *this;
	}
	T * GetArray() { return _array; }
	int GetCurrently() { return _currently; };

	void Add(T e) {
		T * temp = new T[_currently + 1];
		for (int i = 0; i < _currently; i++)
		{
			temp[i] = _array[i];
		}
		temp[_currently] = e;
		delete[] _array;
		_array = temp;
		_currently++;
	}
	friend ostream& operator<<<>(ostream &COUT, const Collection<T> &obj);
	T & operator[](int i) {
		return _array[i];
	}
};
template <class T>
ostream& operator<<<>(ostream &COUT, const Collection<T> &obj) {
	for (int i = 0; i < obj._currently; i++)
	{
		COUT << obj._array[i] << endl;
	}
	return COUT;
}
class Service {
	char * _name;
	TOuble _price;//service price expressed by day of usage
public:
	Service() :_name(nullptr), _price(0) {}
	Service(const char *name, TOuble price) :_price(price) {
		_name = AllocateCharArray(name);
	}
	Service(const Service & o) {
		_name = new char[strlen(o._name) + 1];
		strcpy_s(_name, strlen(o._name) + 1, o._name);
		_price = o._price;
	}
	Service & operator=(const Service & o) {
		if (this != &o) {
			delete[] _name;
			_name = new char[strlen(o._name) + 1];
			strcpy_s(_name, strlen(o._name) + 1, o._name);
			_price = o._price;
		}
		return *this;
	}
	Service(Service && o) {
		_name = o._name;
		_price = o._price;
		o._name = nullptr;
	}
	~Service() {
		delete _name;
		_name = nullptr;
	}
	TOuble GetPrice() { return _price; }
	const char *GetName() { return _name; }
	friend ostream& operator<<(ostream &COUT, const Service &obj);
	bool operator==(const Service & u) {
		if (strcmp(_name, u._name) == 0 && _price == u._price)
			return true;
		return false;
	}
};
ostream& operator<<(ostream &COUT, const Service &obj) {
	COUT << obj._name << " -> \t\t" << obj._price << "KM" << endl;
	return COUT;
}
class Reservations {
	static int _ReservationsIDCounter;
	const int _reservationID;//reservationID starts from 160000
	DateD _reservationFROM;
	DateD _reservationTO;
	TOuble _pricePerDay;//price of bacis services TOesn't include additional services
	Collection<Service>_additionalServices;//additional services can be: sauna, sports grounds, bowling, massage
public:
	Reservations(DateD reservationFROM, DateD reservationTO, TOuble pricePerDay) :
		_reservationFROM(reservationFROM), _reservationTO(reservationTO), _reservationID(_ReservationsIDCounter++) {
		_pricePerDay = pricePerDay;
	}
	bool operator+=(const Service & u) {
		for (int i = 0; i < _additionalServices.GetCurrently(); i++)
		{
			if (_additionalServices[i] == u)
				return false;
		}
		_additionalServices.Add(u);
	}
	void PrintRacun() {
		cout << crt;
		cout << "R.ID:" << _reservationID << endl;
		cout << "FROM: " << _reservationFROM << endl;
		cout << "TO: " << _reservationTO;
		cout << crt;
		cout << "ADDITIONAL SERVICES :";
		cout << crt;

		int numberOfDays = _reservationTO.SumOfDays() - _reservationFROM.SumOfDays();
		TOuble basic = numberOfDays * _pricePerDay;
		TOuble additional = 0;

		for (int i = 0; i < _additionalServices.GetCurrently(); i++)
		{
			TOuble price = _additionalServices[i].GetPrice();
			cout << _additionalServices[i].GetName() << "->\t\t\t" << price << " KM" << endl;
			additional += price;
		}
		additional *= numberOfDays;

		cout << crt;
		cout << "Basic services->\t\t" << basic << " KM" << endl;
		cout << crt;
		cout << "Additional services->\t\t" << additional << " KM" << endl;
		cout << crt;
		cout << "Total->\t\t\t" << basic + additional << " KM" << endl;
		cout << crt;
	}
	Service * operator[](const char * name) {
		for (int i = 0; i < _additionalServices.GetCurrently(); i++)
		{
			if (strcmp(name, _additionalServices[i].GetName()) == 0)
				return &_additionalServices[i];
		}
		return nullptr;
	}
	Collection<Service> operator()(int FROM, int TO) {
		Collection<Service> k;
		for (int i = 0; i < _additionalServices.GetCurrently(); i++)
		{
			if (_additionalServices[i].GetPrice() >= FROM && _additionalServices[i].GetPrice() <= TO)
				k.Add(_additionalServices[i]);
		}
		return k;
	}
};

int Reservations::_ReservationsIDCounter = 160000;

void main() {

	cout << crt << "EXAM DURATION -> 90 MINUTES" << crt;
	Collection<int>numbers;
	for (int i = 0; i < 15; i++)
	{
		numbers.Add(i);
	}
	cout << numbers << crt;
	Service Parking("Parking", 5), Breakfast("Breakfast", 10), Lunch("Lunch", 15), Laundry("Laundry", 10);

	//reservationFROM,reservationTO,pricePerDay
	Reservations reservation(DateD(25, 10, 2018), DateD(30, 10, 2018), 59.9);

	//adding new service to reservation, disable repetition of services
	if (reservation += Parking)
		cout << "Service added successfully!" << endl;
	if (reservation += Breakfast)
		cout << "Service added successfully!" << endl;
	if (reservation += Lunch)
		cout << "Service added successfully!" << endl;
	if (reservation += Laundry)
		cout << "Service added successfully!" << endl;
	if (reservation += Breakfast)
		cout << "Service already exists!" << endl;
	/*
		displays a bill formatted as shown in the picture. when calculating the amount
	use the prices of basic and additional services, as well as the number of days for which the reservation was made
	(note: length of the reservation is maximum 30 days)
	*/
	reservation.PrintBill();
	//returns pointer to the reservation with forwarded name
	Service *p = reservation["Breakfast"];
	if (p != nullptr)
		cout << *p << crt;
	//returns all additional services that have price in range of the value of parameter
	Collection<Service>servicesPerPrice = reservation(8, 12);
	for (int i = 0; i < servicesPerPrice.GetCurrently(); i++)
		cout << servicesPerPrice[i] << crt;
	cin.get();

	system("pause");
}

/*
--------------------------------------
R.ID:160000
FROM:25 10 2018
TO:30 10 2018
--------------------------------------
Additional services:
--------------------------------------
Parking->                   5 KM
Breakfast->                  10 KM
Lunch->                    15 KM
Laundry->                  10 KM
--------------------------------------
basic services->           299.5 KM
--------------------------------------
additional services->           200 KM
--------------------------------------
Total->                   499.5 KM
--------------------------------------

*/