#include<iostream>
#include<fstream>
#include<cstring>
#include<list>
using namespace std;

class Sportiv {
private:
	const int id;
	char* nume;
	bool dePerformanta;
	float venitAnual;
public:
	static int base_id; //id-ul de baza de la care se autogenereaza id-uri.
	
	//1
	Sportiv() :id(++base_id) {
		nume = new char[11];
		strcpy(nume, "NECUNOSCUT");//g++
		//strcpy_s(nume, 11, "NECUNOSCUT");
		dePerformanta = NULL;
		venitAnual = 0;
	}
	Sportiv(const char* _nume, bool _dePerformanta, float _venitAnual) :id(++base_id) {
		nume = new char[strlen(_nume) + 1];
		strcpy(nume, _nume);//g++
		//strcpy_s(nume, strlen(_nume) + 1, _nume);
		dePerformanta = _dePerformanta;
		venitAnual = _venitAnual;
	}
	Sportiv(const Sportiv& s):id(++base_id) {
		nume = new char[strlen(s.nume) + 1];
		strcpy(nume, s.nume);//g++
		//strcpy_s(nume, strlen(s.nume) + 1, s.nume);
		dePerformanta = s.dePerformanta;
		venitAnual = s.venitAnual;
	}
	friend istream& operator>>(istream& in, Sportiv& s) {
		cout << "Nume:";
		char buffer[100];
		in.getline(buffer, 99);
		delete[] s.nume;
		s.nume = new char[strlen(buffer) + 1];
		strcpy(s.nume, buffer);//g++
		//strcpy_s(s.nume, strlen(buffer) + 1, buffer);
		cout << "[0 sau 1] Performanta:"; 
		int test;
		in >> test;
		if ((test != 0) && (test != 1)) {
			s.dePerformanta = 0;
		}
		else {
			s.dePerformanta = test;
		}
		cout << "Venit:";
		in >> s.venitAnual;
		return in;
	}
	Sportiv& operator=(const Sportiv& s) {
		const int id = s.id;
		delete[] nume;
		nume = new char[strlen(s.nume) + 1];
		strcpy(nume, s.nume);//g++
		//strcpy_s(nume, strlen(s.nume) + 1, s.nume);
		dePerformanta = s.dePerformanta;
		venitAnual = s.venitAnual;
		return *this;
	}
	friend ostream& operator<<(ostream& out, Sportiv& s) {
		out << "ID:" << s.id<<endl;
		out << "Nume:" << s.nume << endl;
		if (s.dePerformanta)
			out << "Este de performanta." << endl;
		else
			out << "Nu este de performanta." << endl;
		out << "Venit Anual:" << s.venitAnual<<endl;
		out << endl;
		return out;
	}
	void setVenitAnual(float _venit) {
		venitAnual = _venit;
	}

	//2
	char* getNume() {
		return nume;
	}
	bool operator>(Sportiv& s) {
		if (venitAnual <= s.venitAnual)
			return false;
		else 
			return true;
	}
	~Sportiv() {
		if (nume != NULL) {
			delete[] nume;
		}
	}
};
int Sportiv::base_id = 1000; //Eu incep de la 1000 id-ul. Why not?

class Tenismen :public Sportiv {
	int locATP;
	int nrTurnee;
	int* locuriTurnee;
public:
	//3
	Tenismen() :Sportiv() {
		locATP = 100; //fie 100 de clasamente
		nrTurnee = 0;
		locuriTurnee = NULL;
	}
	Tenismen& operator+=(int loc) {
		nrTurnee++;
		if (locuriTurnee) {
			int* buffer = new int[nrTurnee - 1];//aici pun vechiul vector
			for (int i = 0; i < nrTurnee - 1; i++) {
				buffer[i] = locuriTurnee[i];
			}
			delete[] locuriTurnee;
			locuriTurnee = new int[nrTurnee];
			for (int i = 0; i < nrTurnee - 1; i++) {
				locuriTurnee[i] = buffer[i];
			}
			locuriTurnee[nrTurnee - 1] = loc;
		}
		else {
			locuriTurnee = new int[nrTurnee];
			locuriTurnee[0] = loc;
		}
		return *this;
	}
	void setLocATP(int loc) {
		locATP = loc;
	}
	
	//4
	Tenismen& operator()(int loc, int crestere) {
		*this+= loc; 
		(*this).setLocATP(locATP - crestere); //SAU this->setLocATP(locATP + crestere); Exact acelasi lucru.
		//este minus pentru ca o "crestere" in clasament inseamna un numar mai mic...
		return *this;
	}
	
	//5
	//Pentru a functiona, avem nevoie de constructor de copiere :) 
	//Daca nu folosim, ce se intampla: Cand se copiaza intre functii Tenismenii, o sa se copieze prost, ptc
	//compilatorul nu stie sa copieze singur un vector dinamic (locurile). D-aia, trebuie sa specificam noi ce si cum:
	Tenismen(const Tenismen& t):Sportiv(t) {//Vreau sa apeleze si constructorul de copiere pt sportivi! Daca nu, daca avem un tenismen cu nume (nu avem, dar in fine) o sa dea eroare de executie.
		locATP = t.locATP;
		nrTurnee = t.nrTurnee;
		locuriTurnee = new int[nrTurnee];
		for (int i = 0; i < nrTurnee; i++)
			locuriTurnee[i] = t.locuriTurnee[i];
	}
	Tenismen& operator=(const Tenismen& t) {
		locATP = t.locATP;
		nrTurnee = t.nrTurnee;
		if (locuriTurnee)
			delete[] locuriTurnee;
		locuriTurnee = new int[nrTurnee];
		for (int i = 0; i < nrTurnee; i++)
			locuriTurnee[i] = t.locuriTurnee[i];
		return *this;
	}
	//De asemenea, pentru afisare corecta, trebuie sa avem operator<< si pt tenismen. Altfel, se v-a apela doar cel din Sportivi
	//care nu este relevant pentru noi, care cerem info despre tenismen!
	friend ostream& operator<<(ostream& out, Tenismen t) {
		out << "Tenismen:" <<t.getNume()<< endl;
		out << "Loc ATP:" << t.locATP << endl;
		out << "nr. Turnee:" << t.nrTurnee << endl;
		out << "Locuri in turnee:";
		for (int i = 0; i < t.nrTurnee; i++)
			out << t.locuriTurnee[i] << " ";
		out << endl<<endl;
		return out;
	}
	Tenismen operator++(int) { 
		Tenismen copie = *this;
		this->setLocATP(locATP - 1);
		return copie;
	}
	const Tenismen& operator++() {
		this->setLocATP(locATP - 1);
		return *this;
	}
	
	//6
	int operator[](int index) {
		if(index >=0 && index<nrTurnee)
			return locuriTurnee[index];
		throw runtime_error("Index gresit!");//Recomandat ar fi sa facem alta clasa de exceptii, derivata din exception. Dar... pentru o singura exceptie e okay sa folosim si clasa mare.
	}

	//7
	friend fstream& operator<<(fstream& fout, Tenismen t) {
		if (fout) {
			fout << t.locATP << endl;
			fout << t.nrTurnee << endl;
			for (int i = 0; i < t.nrTurnee; i++)
				fout << t.locuriTurnee[i] << " ";
			fout << endl;
		}
		return fout;
	}
	friend fstream& operator>>(fstream& fin, Tenismen &t) {
		if (fin) {
			fin >> t.locATP;
			fin >> t.nrTurnee;
			if (t.locuriTurnee)
				delete[] t.locuriTurnee;
			t.locuriTurnee = new int[t.nrTurnee];
			for (int i = 0; i < t.nrTurnee; i++)
				fin >> t.locuriTurnee[i];
		}
		return fin;
	}
	~Tenismen() {
		if (locuriTurnee!=NULL)
			delete[] locuriTurnee;
	}

	int getnrTurnee() {
		return nrTurnee;
	}
};
bool sorteazaNrTurnee(Tenismen &a, Tenismen &b) {
	return (a.getnrTurnee() < b.getnrTurnee());//am facut getter de turnee
}

int main() {

	//1
	Sportiv s1;
	Sportiv s2("Hagi", true, 1000); //presupun ca 1000 ala e salariul
	Sportiv s3 = s2;
	Sportiv s4("Nadia", true, 2000);
	//cin >> s4;
	s1 = s3;
	cout << s1;
	s2.setVenitAnual(3000);

	//2
	if (s2 > s1)
		cout << s2.getNume() << " are venitul mai mare decat " << s1.getNume() << endl;//numai Hagi poate sa aiba salariul mai mare decat Hagi.

	//3
	Tenismen t1;
	t1 += 3;
	t1 += 6;
	t1.setLocATP(6);

	//4
	Tenismen t2;
	t2(2, 1);//un turneu nou, cu locul 2, si ii mai si creste ATP. 
	
	cout << endl;
	//5
	t2 = t1++;
	cout << t2;//tenismenii sunt necunoscuti deoarece nu ne-a cerut sa fie atribuiti sportivilor deja prezenti. Astfel, ei cand sunt creati, apar default, adica necunoscut.
	t2=++t1;
	cout << t2;

	//6
	int loc = t1[0];//poza e proasta, presupun ca vrea locul de pe pozitia 0 nu?
	try {//arunca exceptie
		int loc2 = t1[-1];
	}
	catch (exception& e) {
		cout << e.what()<<endl;
	};//apropo, daca foloseste catch exception, e normal sa afiseze in consola exit code-1. Ca doar, asta si inseamna. Ca a prins exceptia, am avut o eroare!

	//7


	fstream fisTenismen;
	fisTenismen.open("output.txt", ios::out);
	fisTenismen << t1;
	fisTenismen.close();
	fisTenismen.open("output.txt", ios::in);
	fisTenismen >> t2;
	cout << t2;
	fisTenismen.close();

	//8 STL list cu 3 tenismeni, sortati dupa numar turnee.
	list<Tenismen> lt;
	Tenismen t3;
	Tenismen t4;
	t3 += 2;
	t3 += 1;
	t3 += 10;
	t4 += 12;//am pus de la mine inca 2.
	lt.push_front(t1);
	lt.push_front(t2);
	lt.push_front(t3);
	lt.push_front(t4);
	list<Tenismen>::iterator li;
	for (li = lt.begin(); li != lt.end(); li++) {
		cout << *li << endl;
	}//afisare nesortata
	//acum sortez:
	lt.sort(sorteazaNrTurnee);//vezi functia sorteazaNrTurnee in afara clasei
	for (li = lt.begin(); li != lt.end(); li++) {
		cout << *li << endl;
	}
	return 0;
}