//autoturism personal -- gestiune revizii
#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

class Abstract{
public:
	virtual float totalRevizie() = 0;
};
class Revizie:public Abstract{
	const int id_revizie;
	static int id;
	string data;
	int numarReparatii;
	float* pretReparatii;
public:
	Revizie() :id_revizie(id++) {
		data = "DD/MM/YYY";
		numarReparatii = 0;
		pretReparatii = NULL;
	}
	Revizie(const string _data, int _numarReparatii, float* _pretReparatii) :id_revizie(id++) {
		data = _data;
		numarReparatii = _numarReparatii;
		pretReparatii = new float[numarReparatii];
		for (int i = 0; i < numarReparatii; i++) {
			pretReparatii[i] = _pretReparatii[i];
		}
	}
	Revizie(const Revizie& r):id_revizie(id++) {
		data = r.data;
		numarReparatii = r.numarReparatii;
		pretReparatii = new float[numarReparatii];
		for (int i = 0; i < numarReparatii; i++) {
			pretReparatii[i] = r.pretReparatii[i];
		}
	}
	Revizie& operator=(const Revizie& r) {
		data = r.data;
		numarReparatii = r.numarReparatii;
		if (pretReparatii)
			delete[] pretReparatii;
		pretReparatii = new float[numarReparatii];
		for (int i = 0; i < numarReparatii; i++) {
			pretReparatii[i] = r.pretReparatii[i];
		}
		return *this;
	}
	~Revizie() {
		if (pretReparatii)
			delete[] pretReparatii;
		id--;
	}
	void setPreturi(int _numarReparatii, float* _pretReparatii) {
		if (_numarReparatii > 0) {
			if (pretReparatii)
				delete[] pretReparatii;
			numarReparatii = _numarReparatii;
			pretReparatii = new float[numarReparatii];
			for (int i = 0; i < numarReparatii; i++) {
				pretReparatii[i] = _pretReparatii[i];
			}
		}
		else {
			throw runtime_error("Numar Reparatii invalid!");
		}
	}
	float getPretReparatie(int index) {
		if(index-1 >= 0 && index-1<numarReparatii){
			return pretReparatii[index-1];
		}
		else {
			//
		}
	}
	float totalRevizie() {
		if (pretReparatii) {
			float total = pretReparatii[0];
			for (int i = 1; i < numarReparatii; i++)
				total = total + pretReparatii[i];
			return total;
		}
	}
	float medieReparatie() {
		return totalRevizie() / numarReparatii;
	}
	friend ostream& operator<<(ostream& out, Revizie& r) {
		cout << "ID:" << r.id_revizie << endl;
		cout << "DATA:" << r.data << endl;
		cout << "NR. REPARATII:" << r.numarReparatii << endl;
		cout << "PRETURI REPARATII:";
		for (int i = 0; i < r.numarReparatii; i++) {
			out << r.pretReparatii[i] << " ";
		}
		out << endl;
		return out;
	}
	friend fstream& operator<<(fstream& fout, Revizie& r) {
		if (fout) {
			fout << r.data << endl;
			fout << r.numarReparatii << endl;
			for (int i = 0; i < r.numarReparatii; i++) {
				fout << r.pretReparatii[i] <<endl;
			}
		}
		return fout;
	}
	friend fstream& operator>>(fstream& fin, Revizie& r) {
		if (fin) {
			char buffer[100];
			fin.getline(buffer, 99);
			r.data = buffer;
			fin >> r.numarReparatii;
			r.pretReparatii = new float[r.numarReparatii];
			for (int i = 0; i < r.numarReparatii; i++) {
				fin >> r.pretReparatii[i];
			}
			fin.get();
		}
		
		return fin;
	}
	bool operator!() {
		return(pretReparatii ? false : true);
	}
	bool operator>(Revizie& r) {
		return(totalRevizie() <= r.totalRevizie() ? false : true);
	}
	friend istream& operator>>(istream& in, Revizie& r) {
		cout << "DATA(DD/MM/YYYY):";
		char buffer[100];
		in.getline(buffer,99);
		r.data = buffer;
		cout << "NR. REPARATII:";
		int n;
		in >> n;
		float* v = new float[n];
		for (int i = 0; i < n; i++) {
			cout << "Reparatia " << i + 1 << ":";
			in >> v[i];
		}
		r.numarReparatii = n;
		r.setPreturi(n, v);//si in cadrul ei, se trateaza problemele
		delete[] v;
		return in;
	}
	

};
int Revizie::id = 0;
template <class T>
class Tclass:public T {
	T r;
	float reducere;
public:
	//const si meto<<
	Tclass():T(r) {
		reducere = 0;
	}
	Tclass(const Tclass& tc) {
		reducere = tc.reducere;
		r = tc.r;
	}
	friend ostream& operator<<(ostream& out, Tclass& tc) {
		out << tc.r <<"cu reducerea ";
		out << tc.reducere;
		return out;
	}
};
int main() {
	Revizie r1;
	float preturi1[3] = { 150, 250, 400 };
	Revizie r2("01/02/2018", 3, preturi1);
	float preturi2[4] = { 50, 150, 300, 100 };
	Revizie r3("22/06/2017", 4, preturi2);
	Revizie r4 = r2;
	Revizie r5;
	r5 = r3;

	cout << "Testare functii:" << endl;
	float preturi3[2] = { 35.5, 34.5 };
	r1.setPreturi(2, preturi3);
	cout << "Pretul reparatiei " << 2 << " este " << r1.getPretReparatie(2) << endl;
	cout << "Total Revizie: " << r1.totalRevizie() << endl;
	cout << "Medie preturi reparatii: " << r1.medieReparatie() << endl;
	cout << endl;
	if (!r1) {
		cout << "Nu are preturi de reparatii!" << endl;
	}
	else cout << "Are preturi de reparatii!" << endl;
	cout << endl;
	if (r2 > r1) {
		cout << r2.totalRevizie() << " este mai mare ca " << r1.totalRevizie() << endl;
	}else
		cout << r1.totalRevizie() << " este mai mare ca " << r2.totalRevizie() << endl;
	cout << endl;
	
	/*try {
		cin >> r1;
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
	cout << r1 << endl;*/

	//cout << r1 << r2 << r3 << r4 << r5;
	Revizie* vr1 = new Revizie[10];
	fstream fr;
	/*fr.open("frevizie.txt", ios::out);
	fr<< r1 << r2 << r3 << r4 << r5;
	fr.close();*/
	fr.open("frevizie.txt", ios::in);
	for (int i = 0; i < 5; i++) {
		Revizie cr;
		fr >>cr;
		if (cr.totalRevizie() > 100) {
			vr1[i] = cr;
		}
	}
	for (int i = 0; i < 10; i++) {
		if(vr1[i].totalRevizie()>0)
			cout << vr1[i];
	}
	cout << endl;
	
	Abstract* a1;
	a1 = &r2;
	cout << a1->totalRevizie() << endl;
	cout << r2.totalRevizie()<<endl;

	Tclass<Revizie> tclass;
	cout << tclass << endl;
	system("cls");

	vector<Revizie> vr;
	vector<Revizie>::iterator it;
	vr.push_back(r1);
	vr.push_back(r2);
	vr.push_back(r3);
	vr.push_back(r4);
	vr.push_back(r5);
	for (it = vr.begin(); it != vr.end(); it++) {
		cout << *it;
	}

	


	return 0;
}