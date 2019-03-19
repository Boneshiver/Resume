//Gestiune meniu restaurant 
#include<iostream>
#include<string>
#include<fstream>
#include<vector>

using namespace std;

class InterfataRestaurant {
	virtual void setPreturi(int,float*) = 0;
	virtual float* getPreturi() = 0;
	virtual float valoareMeniu() = 0;
	virtual float mediePreturi() = 0;
};

class Restaurant:public InterfataRestaurant {
protected:
	const int id_restaurant;
	static int id;
	string denumire;
	int meniu;
	float* preturi;
public:
	Restaurant():id_restaurant(id++) {
		denumire = "necunoscut";
		meniu = 0;
		preturi = NULL;
	}
	Restaurant(const string _denumire, int _meniu, float* _preturi) :id_restaurant(id++) {
		denumire = _denumire;
		meniu = _meniu;
		preturi = new float[meniu];
		for (int i = 0; i < meniu; i++) {
			preturi[i] = _preturi[i];
		}
	}
	Restaurant(const Restaurant& r):id_restaurant(r.id_restaurant) {
		denumire = r.denumire;
		meniu = r.meniu;
		if (preturi)
			delete[] preturi;
		preturi = new float[meniu];
		for (int i = 0; i < meniu; i++) {
			preturi[i] = r.preturi[i];
		}
	}
	~Restaurant() {
		if (preturi)
			delete[] preturi;
		id--;
	}

	void setPreturi(int _meniu, float* _preturi) {
		if (_meniu > 0) {
			if (preturi) {
				delete[] preturi;
			}
			meniu = _meniu;
			preturi = new float[meniu];
			for (int i = 0; i < meniu; i++) {
				preturi[i] = _preturi[i];
			}
		}
		else {
			throw runtime_error("Numarul de produse din meniu este invalid!\n");
		}
	}
	float* getPreturi() {
		return preturi;
	}
	float valoareMeniu() {
		if (preturi) {
			float val = preturi[0];
			for (int i = 1; i < meniu; i++) {
				val = val + preturi[i];
			}
			return val;
		}
		else {
			throw runtime_error("Restaurantul nu are un meniu!\n");
		}
	}
	float mediePreturi() {
		if (preturi) {
			return valoareMeniu() / meniu;
		}
		else {
			throw runtime_error("Restaurantul nu are un meniu!\n");
		}
	}

	Restaurant& operator=(const Restaurant& r) {
		denumire = r.denumire;
		meniu = r.meniu;
		if (preturi)
			delete[] preturi;
		preturi = new float[meniu];
		for (int i = 0; i < meniu; i++) {
			preturi[i] = r.preturi[i];
		}
		return *this;
	}
	friend ostream& operator<<(ostream& out, Restaurant& r) {
		out << r.denumire << endl;
		out << "Produse meniu:"<<r.meniu << endl;
		out << "Preturi produse:";
		for (int i = 0; i < r.meniu; i++) {
			out << r.preturi[i] << " ";
		}
		out << endl;
		return out;
	}
	float operator()() {//pret maxim
		if (preturi) {
			float max = preturi[0];
			for (int i = 1; i < meniu; i++) {
				if (max < preturi[i])
					max = preturi[i];
			}
			return max;
		}
		else {
			return 0;
		}
	}
	bool operator>(Restaurant& r) {
		return(valoareMeniu() <= r.valoareMeniu() ? false : true);
	}

	friend istream& operator>>(istream& in, Restaurant& r) {
		cout << "Denumire:";
		char buffer[100];
		in.getline(buffer, 99);
		r.denumire = buffer;
		cout << "Numar produse:";
		int _meniu;
		in >> _meniu;
		try {
			float* pp = new float[_meniu];
			for (int i = 0; i < _meniu; i++) {
				cout << "pret " << i + 1 << ":";
				in >> pp[i];
			}
			r.setPreturi(_meniu, pp);
		}
		catch (exception) {
			cout << "Numar invalid pentru meniu!\n";
		}
		in.get();
		return in;
	}

	friend fstream& operator<<(fstream& fout, Restaurant& r) {
		if (fout) {
			fout << r.denumire << endl;
			fout << "Produse meniu:" << r.meniu << endl;
			fout << "Preturi produse:";
			for (int i = 0; i < r.meniu; i++) {
				fout << r.preturi[i] << " ";
			}
			fout << endl;
		}
		return fout;
	}


};
int Restaurant::id = 100;

class Autoservire:public Restaurant{
	float reducere;
public:
	Autoservire() :Restaurant() {
		reducere = 0;
	}
	Autoservire(float _reducere, const string denumire, int meniu, float* preturi) :Restaurant(denumire, meniu, preturi) {
		reducere = _reducere;
	}
	Autoservire& operator=(const Autoservire& a) {
		reducere = a.reducere;
		Restaurant::operator=(a);
		return *this;
	}
	friend ostream& operator<<(ostream& out, Autoservire& a) {
		out << a.denumire << endl;
		out << "Produse meniu:" << a.meniu << endl;
		out << "Reducere: " << a.reducere << "%" << endl;
		out << "Preturi produse:";
		for (int i = 0; i < a.meniu; i++) {
			out << a.preturi[i] << " ";
		}
		
		out << endl;
		return out;
	}

};

template<class T>
class vRestaurant {
	T* v;
	int n;
public:
	vRestaurant(int _n) {
		v = new T[_n];
		n = _n;
	}
	friend ostream& operator<<(ostream& out, vRestaurant& vr) {
		for (int i = 0; i < vr.n; i++) {
			out << vr.v[i] << endl;
		}
		return out;
	}
	vRestaurant& operator=(const T& t) {
		v = &t;
	}
	T& operator[](int i) {
		return v[i];
	}
};
int main() {
	Restaurant r1, r2;
	float preturi3[3] = { 10, 15, 25.5 };
	float preturi4[4] = { 9, 12.5, 30, 25 };
	float preturi5[5] = { 10.50, 32.5, 18.5, 10, 50 };
	Restaurant r3("Restaurant Pirat",3,preturi3), r4("Restaurant Coltul vesel",4,preturi4), r5("Restaurant Cocosul Rosu",5,preturi5);
	Restaurant r0(r3);
	
	r1.setPreturi(3, preturi3);
	try {
		r1.setPreturi(-2, preturi3);
	}
	catch(exception& e){
		cout << e.what();
	}
	cout << r1.valoareMeniu()<<endl;
	cout << r1.mediePreturi()<<endl;
	try {
		r2.valoareMeniu();
	}
	catch (exception& e) {
		cout << e.what();
	}

	r1 = r2;
	cout << r1 << endl << r2 << endl;
	cout << r2() << endl;
	if (r5 > r4) {
		cout << r5.valoareMeniu() << " > " << r4.valoareMeniu() << endl;
	}
	else {
		cout << r4.valoareMeniu() << " > " << r5.valoareMeniu() << endl;
	}

	cin >> r3;

	Restaurant** vpr = new Restaurant*[6];
	vpr[0] = &r0;
	vpr[1] = &r1;
	vpr[2] = &r2; //fac vp ca sa nu treaca prin constructor sa strice id-ul
	vpr[3] = &r3;
	vpr[4] = &r4;
	vpr[5] = &r5;

	cout << "Nume fisier:";
	string nume;
	getline(cin, nume);
	nume = nume + ".txt";
	fstream fr;
	fr.open(nume, ios::out);
	for (int i = 0; i < 6; i++) {
		fr << *vpr[i];
	}
	fr.close();

	Autoservire a0(20, "Autoservire Eforie Nord", 5, preturi5);
	cout << a0 << endl;

	vRestaurant<Restaurant> vR(6);//am creat un vector de restaurante capabil de 6 restaurante
	vR[0] = r0;
	vR[1] = r1;
	vR[2] = r2;
	vR[3] = r3;
	vR[4] = r4;
	vR[5] = r5;
	for (int i = 0; i < 6; i++)
		cout << vR[i] << endl;


	vector<Restaurant*> vr;
	vector<Restaurant*>::iterator it;
	vr.push_back(&r0);
	vr.push_back(&r1);
	vr.push_back(&r2);
	vr.push_back(&r3);
	vr.push_back(&r4);
	vr.push_back(&r5);
	cout << "*\n*\n";
	for (it = vr.begin(); it != vr.end(); it++) {
		cout << **it << endl;
	}

	return 0;
}
