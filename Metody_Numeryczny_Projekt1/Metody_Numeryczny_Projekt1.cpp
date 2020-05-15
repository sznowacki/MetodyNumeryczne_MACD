// Metody_Numeryczny_Projekt1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

double EMA(vector<double>* samples, int interval, int day)
{
	double scoreEMA = 0.0;
	double alpha = 2.0/(double(interval) - 1.0);
	double delta = (1.0 - alpha);
	double nominator = (*samples)[day];
	double denominator = 1.0;
	double tmp = delta;
	double tmp2 = 0.0;

	//Zabezpieczenie dla dni z mniejsza licza danych poprzedzajacych niz wartosc okresu
	if (day < interval)
	{
		interval = day;
	}

	for(int i = 0; i < interval; i++)
	{
		tmp2 = (*samples)[(day - 1 - i)] * tmp;
		nominator = nominator + tmp2;
		denominator = denominator + tmp;
		tmp = tmp * delta;
	}
	
	scoreEMA = nominator / denominator;
	return scoreEMA;
}

double moneyMoneyMoney(vector<double>* samples, int akcje)
{
	double stanKonta = 0;
	int posiadaneAkcje = akcje;
	double najnizszaWartosc = (*samples)[0];
	double wartoscPrzyZakupie = 0;
	bool przeciecieZGory = false;
	bool przeciecieZDolu = false;
	vector <double>* MACD = new vector<double>;
	vector <double>* SIGNAL = new vector<double>;

	for (int i = 0; i < samples->size(); i++)
	{
		//Tworzenie Wskaznika MACD w czasie rzeczywistym
		MACD->push_back(EMA(samples, 12, i) - EMA(samples, 26, i));
		SIGNAL->push_back((EMA(MACD, 9, i)));

		//Wyczekiwanie na przeciecie z dolu(MACD przecina SIGNAL z dolu, czyli jego wartosci zmieniaja wartosc z mniejszych wzgledem SIGNAL na wieksze)
		if( i > 1)
		{
			//Jezeli wykres MACD rosnie
			if((*MACD)[i-1] < (*MACD)[i])
			{
				if ((*SIGNAL)[i] > (*MACD)[i])
				{
					//nastapilo przeciecie z gory
					przeciecieZGory = true;
					przeciecieZDolu = false;
					//sprzedajemy
					printf("Mam pieniazkow: %f i akcji: %d\n", stanKonta, posiadaneAkcje);
					double sprzedajAkcje = posiadaneAkcje*(*samples)[i];
					stanKonta = stanKonta + sprzedajAkcje;
					posiadaneAkcje = 0;
					printf("Po sprzedazy  mam pieniazkow: %f i akcji: %d\n", stanKonta, posiadaneAkcje);
				}
							
			}
			else //MACD maleje
			{
				//Jezeli SIGNAL < MACD -> zarabiamy
				if ((*SIGNAL)[i] < (*MACD)[i])
				{
					//nastapilo przeciecie z dolu
					przeciecieZGory = false;
					przeciecieZDolu = true;
					//kupujemy
					printf("Mam pieniazkow: %f i akcji: %d\n", stanKonta, posiadaneAkcje);
					int kupAkcje = stanKonta / (*samples)[i];
					wartoscPrzyZakupie = (*samples)[i];
					posiadaneAkcje = posiadaneAkcje + kupAkcje;
					stanKonta = stanKonta - kupAkcje*(*samples)[i];
					printf("Po zakupie mam pieniazkow: %f i akcji: %d\n", stanKonta, posiadaneAkcje);
				}
			}
		}
	}
	printf("Na koniec mam pieniazkow: %f i akcji: %d\n", stanKonta, posiadaneAkcje);
	double cenaAkcji = (*samples)[samples->size() - 1];
	stanKonta = stanKonta + posiadaneAkcje*cenaAkcji;
	delete MACD;
	delete SIGNAL;

	return stanKonta;
}

int main()
{
	vector <double>* samples = new vector<double>;
	vector <double>* MACD = new vector<double>;
	vector <double>* SIGNAL = new vector<double>;

	string fileName;
	
	cout << "Podaj nazwe pliku z probkami: ";
	cin >> fileName;
	fstream file;	

	//odczyt z pliku
	file.open(fileName, ios::in);

	if(file.good() == false)
	{
		cout << "Plik nie istnieje";
		exit(0);
	}

	string line;
	int lineNr = 1;

	while(getline(file, line))
	{
		samples->push_back(stod(line.c_str()));
		lineNr++;
	}

	file.close();

	//Tu zarabiam pieniazki
	int iloscAkcji = 1000;
	double pinionzki = moneyMoneyMoney(samples, iloscAkcji);
	cout << endl << "Mialem " << iloscAkcji << " akcji po: " << (*samples)[0] << " pieniazkow, czyli: " << iloscAkcji* (*samples)[0] << " pieniazkow" << endl;
	cout << "Teraz mam: " << pinionzki << " pieniazkow"<< endl;
	
//	//Towrzenie MACD
//	for(int i = 0; i < samples->size();i++)
//	{
//		MACD->push_back(EMA(samples, 12, i) - EMA(samples, 26, i));
//	}
//
//	//Tworzenie SIGNAL
//	for (int i = 0; i < MACD->size(); i++)
//	{
//		SIGNAL->push_back((EMA(MACD, 9, i)));
//	}
//
//	//Zapis do pliku.csv
//	file.open("WynikiKoncowe.csv", ios::out | ios::app);
//	file << "price , MACD , SIGNAL \n";
//	for(int i =0; i < samples->size();i++)
//	{
//		file << (*samples)[i] << "," << (*MACD)[i] << "," << (*SIGNAL)[i] << "\n";		
//	}
//	
//	file.close();

	delete samples;
	delete MACD;
	delete SIGNAL;

	return 0;
}

