#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <sstream> // do getline z pliku
#include <unordered_set> //hasztablice, ignoruje jak beda 2 takie same numery rejestracyjne
#include <conio.h> //zatrzymanie konsoli zeby nie znikalo
#include <cstdlib> //CZYSZCZENIE KONSOLI
#include "tinyxml2.h" //biblioteka zewnetrzna

using namespace tinyxml2;

class Pojazd {
protected:
    std::string numerRejestracyjny;
    std::string marka;
    std::string model;
    int rokProdukcji;

public:
    Pojazd(const std::string& nr, const std::string& m, const std::string& mod, int rok)
        : numerRejestracyjny(nr), marka(m), model(mod), rokProdukcji(rok) {}
    virtual ~Pojazd() {}

    virtual void wyswietlDane() const = 0;
    //virtual void zapiszDoPlikuTekstowego(std::ofstream& out) const = 0;
    virtual void zapiszDoXml(XMLElement* parent, XMLDocument& doc) const = 0; //funkcja ktora zapisuje do xml

    const std::string& getNumerRejestracyjny() const { return numerRejestracyjny; }
};

class Osobowy : public Pojazd {
private:
    std::string typNadwozia; // Zmiana z liczbaDrzwi na typNadwozia

public:
    Osobowy(const std::string& nr, const std::string& m, const std::string& mod, int rok, const std::string& nadwozie)
        : Pojazd(nr, m, mod, rok), typNadwozia(nadwozie) {}

    void wyswietlDane() const override {
        std::cout << numerRejestracyjny
                  << "\t\t\tOsobowy \t" << marka << "\t" << model
                  << "\t\t" << rokProdukcji
                  << "\t\t - \t\t" << typNadwozia << std::endl;
    }

    /*void zapiszDoPlikuTekstowego(std::ofstream& out) const override {
        out << numerRejestracyjny << "\tOsobowy\t" << marka << "\t" << model
            << "\t" << rokProdukcji << "\t" << typNadwozia << "\n";
    }*/

       void zapiszDoXml(XMLElement* parent, XMLDocument& doc) const override{
        XMLElement* pojazdElement = doc.NewElement("Pojazd");
        pojazdElement->SetAttribute("typ", "Osobowy");
        pojazdElement->SetAttribute("numerRejestracyjny", numerRejestracyjny.c_str());
        pojazdElement->SetAttribute("marka", marka.c_str());
        pojazdElement->SetAttribute("model", model.c_str());
        pojazdElement->SetAttribute("rokProdukcji", rokProdukcji);
        pojazdElement->SetAttribute("typNadwozia", typNadwozia.c_str());
        parent->InsertEndChild(pojazdElement);
    }
};

class Motocykl : public Pojazd {
private:
    int pojemnoscSilnika;

public:
    Motocykl(const std::string& nr, const std::string& m, const std::string& mod, int rok, int pojemnosc)
        : Pojazd(nr, m, mod, rok), pojemnoscSilnika(pojemnosc) {}

    void wyswietlDane() const override {
        std::cout << numerRejestracyjny
                  << "\t\t\tMotocykl \t" << marka << "\t" << model
                  << "\t\t" << rokProdukcji
                  << "\t " << pojemnoscSilnika << " cm3" << " \t\t -"<< std::endl;
    }

 /*   void zapiszDoPlikuTekstowego(std::ofstream& out) const override {
        out << numerRejestracyjny << "\tMotocykl\t" << marka << "\t" << model
            << "\t" << rokProdukcji << "\t" << pojemnoscSilnika << "\n";
    }*/

       void zapiszDoXml(XMLElement* parent, XMLDocument& doc) const override { // ZMIANA
        XMLElement* pojazdElement = doc.NewElement("Pojazd");
        pojazdElement->SetAttribute("typ", "Motocykl");
        pojazdElement->SetAttribute("numerRejestracyjny", numerRejestracyjny.c_str());
        pojazdElement->SetAttribute("marka", marka.c_str());
        pojazdElement->SetAttribute("model", model.c_str());
        pojazdElement->SetAttribute("rokProdukcji", rokProdukcji);
        pojazdElement->SetAttribute("pojemnoscSilnika", pojemnoscSilnika);
        parent->InsertEndChild(pojazdElement);
    }
};

class BazaDanychPojazdow {
private:
    std::vector<std::shared_ptr<Pojazd>> pojazdy;
    std::unordered_set<std::string> numeryRejestracyjne;

    bool czyPojazdIstnieje(const std::string& nrRejestracyjny) const {
        return numeryRejestracyjne.count(nrRejestracyjny) > 0;
    }

public:
    void dodajPojazd(const std::shared_ptr<Pojazd>& pojazd) {
        if (!czyPojazdIstnieje(pojazd->getNumerRejestracyjny())) {
            pojazdy.push_back(pojazd);
            numeryRejestracyjne.insert(pojazd->getNumerRejestracyjny());
        } else {
            std::cout << "Pojazd z tym numerem rejestracyjnym już istnieje w bazie!" << std::endl;
        }
    }

    void usunPojazd(size_t index) {
        if (index < pojazdy.size()) {
            numeryRejestracyjne.erase(pojazdy[index]->getNumerRejestracyjny());
            pojazdy.erase(pojazdy.begin() + index);
        } else {
            std::cout << "Nieprawidlowy indeks!" << std::endl;
        }
    }

    void wyswietlWszystkiePojazdy() const {
        for (size_t i = 0; i < pojazdy.size(); ++i) {
            std::cout << "\t"<<  i + 1 << ". \t";
            pojazdy[i]->wyswietlDane();
        }
    }

/*    void zapiszDoPlikuTekstowego(const std::string& nazwaPliku) const {
        std::ofstream out(nazwaPliku);
        if (!out) {
            std::cerr << "Nie udało się otworzyć pliku do zapisu." << std::endl;
            return;
        }
        for (const auto& pojazd : pojazdy) {
            pojazd->zapiszDoPlikuTekstowego(out);
        }
    }
*/
    void zapiszDoXmlPliku(const std::string& nazwaPliku) const {
        XMLDocument doc;
        XMLElement* root = doc.NewElement("Pojazdy");
        doc.InsertFirstChild(root);

        for (const auto& pojazd : pojazdy) {
            pojazd->zapiszDoXml(root, doc);
        }

        if (doc.SaveFile(nazwaPliku.c_str()) != XML_SUCCESS) {
            std::cerr << "Nie udało się zapisać pliku XML." << std::endl;
        }
    }


/*    void wczytajZPlikuTekstowego(const std::string& nazwaPliku) {
        std::ifstream in(nazwaPliku);
        if (!in) {
            std::cerr << "Nie udalo sie otworzyc pliku do odczytu." << std::endl;
            return;
        }

        std::string linia; //wczytanie z klawiatury danych
        while (std::getline(in, linia)) {
            std::istringstream iss(linia);
            std::string typ, marka, model, numerRejestracyjny, dodatkowyParametr;
            int rok;

            std::getline(iss, numerRejestracyjny, '\t');
            std::getline(iss, typ, '\t');
            std::getline(iss, marka, '\t');
            std::getline(iss, model, '\t');
            iss >> rok;
            iss.ignore(1, '\t');
            std::getline(iss, dodatkowyParametr, '\t');

            if (!czyPojazdIstnieje(numerRejestracyjny)) {
                if (typ == "Osobowy") {
                    dodajPojazd(std::make_shared<Osobowy>(numerRejestracyjny, marka, model, rok, dodatkowyParametr));
                } else if (typ == "Motocykl") {
                    dodajPojazd(std::make_shared<Motocykl>(numerRejestracyjny, marka, model, rok, std::stoi(dodatkowyParametr))); //stoi - string to int
                }
            }
        }
    }
    */
       void wczytajZXmlPliku(const std::string& nazwaPliku) {
        XMLDocument doc;
        if (doc.LoadFile(nazwaPliku.c_str()) != XML_SUCCESS) {
            std::cerr << "Nie udało się otworzyć pliku XML." << std::endl;
            return;
        }

        XMLElement* root = doc.FirstChildElement("Pojazdy");
        if (!root) return;

        for (XMLElement* element = root->FirstChildElement("Pojazd"); element; element = element->NextSiblingElement("Pojazd")) {
            std::string typ = element->Attribute("typ");
            std::string numerRejestracyjny = element->Attribute("numerRejestracyjny");
            std::string marka = element->Attribute("marka");
            std::string model = element->Attribute("model");
            int rok;
            element->QueryIntAttribute("rokProdukcji", &rok);

            if (typ == "Osobowy") {
                std::string typNadwozia = element->Attribute("typNadwozia");
                dodajPojazd(std::make_shared<Osobowy>(numerRejestracyjny, marka, model, rok, typNadwozia));
            } else if (typ == "Motocykl") {
                int pojemnoscSilnika;
                element->QueryIntAttribute("pojemnoscSilnika", &pojemnoscSilnika);
                dodajPojazd(std::make_shared<Motocykl>(numerRejestracyjny, marka, model, rok, pojemnoscSilnika));
            }
        }
    }

    void interfejsUzytkownika() {
        int opcja;
        do {
            std::cout << "\nMenu:\n1. Wyswietl pojazdy\n2. Dodaj pojazd\n3. Usun pojazd\n4. Zapisz do pliku\n5. Wyjdz\nWybierz opcje: ";
            std::cin >> opcja;

            switch (opcja) {
                case 1: {
                case1:
                    system("cls");
                    std::cout<< "\tLP. \tNr Rejestracyjny \tRodzaj \t\tMarka \tModel \tRok produkcji \tPojemnosc \t Typ nadwozia" <<std::endl;
                    wyswietlWszystkiePojazdy();
                    break;
                    getch();
                }
                case 2: {
                    int typ;
                    std::cout << "Typ pojazdu (1 - Osobowy, 2 - Motocykl): ";
                    std::cin >> typ;

                    std::string marka, model, numerRejestracyjny;
                    int rok;
                    std::string parametr;
                    std::cout << "Marka: ";
                    std::cin >> marka;
                    std::cout << "Model: ";
                    std::cin >> model;
                    std::cout << "Numer rejestracyjny: ";
                    std::cin >> numerRejestracyjny;
                    std::cout << "Rok produkcji: ";
                    std::cin >> rok;

                    if (typ == 1) {
                        std::cout << "Typ nadwozia: ";
                        std::cin >> parametr;
                        dodajPojazd(std::make_shared<Osobowy>(numerRejestracyjny, marka, model, rok, parametr));
                    } else if (typ == 2) {
                        std::cout << "Pojemnosc silnika: ";
                        std::cin >> parametr;
                        dodajPojazd(std::make_shared<Motocykl>(numerRejestracyjny, marka, model, rok, std::stoi(parametr)));
                    } else {
                        std::cout << "Nieznany typ pojazdu!" << std::endl;
                    }
                    goto case1;
                    break;
                }
                case 3: {
                    size_t index;
                    std::cout << "Indeks pojazdu do usuniecia: ";
                    std::cin >> index;
                    usunPojazd(index - 1);
                    goto case1;
                    break;
                }
                case 4:
                    zapiszDoXmlPliku("../data/baza_danych.xml");
                    break;
                case 5:
                    std::cout << "Koniec programu." << std::endl;
                    break;
                default:
                    std::cout << "Nieznana opcja!" << std::endl;
            }
        } while (opcja != 5);
    }
};

int main() {
    BazaDanychPojazdow baza;
    //baza.wczytajZPlikuTekstowego("../data/baza_danych.txt");
    baza.wczytajZXmlPliku("../data/baza_danych.xml");
    baza.interfejsUzytkownika();
    return 0;
}