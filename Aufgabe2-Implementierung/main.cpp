#include <bits/stdc++.h>
using namespace std;

int indexObstsorte(string sorte, map<string, int>* indizesObstsorten, map<int, string>* namenObstsorten)
{
    //Diese Funktion liefert den Index einer Obstsorte zurueck.
    //Ist diese Sorte bisher noch unbekannt, aktualisiert sie zusaetzlich die via Zeiger uebergebenen Container
    int groesse = (int) indizesObstsorten->size(); //Groesse der Container vor dem Aufruf dieser Funktion
    int index = (*indizesObstsorten)[sorte];
    if((int) indizesObstsorten->size() == groesse)
    {
        //Der Obstsorte ist schon ein Index zugeordnet
        return index;
    }
    else
    {
        //Der Obstsorte muss noch ein Index zugeordnet werden!
        (*indizesObstsorten)[sorte] = groesse;
        (*namenObstsorten)[groesse] = sorte;
        //Die Container sind aktualisiert
    }
    return groesse;
}

int StringToInt(string input) //Konvertiert einen String in eine Zahl
{
    stringstream iss;
    iss.clear();
    int output;
    iss<<input;
    iss>>output;
    return output;
}

bool StringIstZahl(string input) //Prueft, ob der uebergebene String eine Zahl zwischen 1 und 26 ist
{
    if(input == "1" || input == "2" || input == "3" || input == "4" || input == "5" || input == "6" || input == "7" || input == "8" ||
       input == "9" || input == "10" || input == "11" || input == "12" || input == "13" || input == "14" || input == "15" || input == "16" ||
       input == "17" || input == "18" || input == "19" || input == "20" || input == "21" || input == "22" ||
       input == "23" || input == "24" || input == "25" || input == "26")
    {
        return true;
    }
    return false;
}

void datenEinlesen(map<string, int>* indizesObstsorten, map<int, string>* namenObstsorten, int* anzahlObstsorten,
                   vector<vector<int>>* SMengen, vector<int>* ziehungen, vector<int>* wunschsorten, string* dateinameAusgabedatei)
{
    //Liest die Eingabedaten ein
    //Oeffnen der Eingabedatei:
    char dateiname[1000];
    cout<<"Geben Sie den Namen der Eingabedatei an: ";
    cin>>dateiname;

    cout<<"Geben Sie den Namen der Ausgabedatei an: ";
    cin>>*dateinameAusgabedatei;
    freopen(dateiname, "r", stdin);

    //Anzahl der Obstsorten:
    cin>>*anzahlObstsorten;
    //Vorbereiten der Container:
    for(int i = 0; i < *anzahlObstsorten; i++)
    {
        ziehungen->push_back(0);
    }
    for(int i = 0; i < *anzahlObstsorten; i++)
    {
        SMengen->push_back(*ziehungen);
    }

    string input; //Variable fuer das Speichern eines Worts
    cin>>input;
    //Einlesen der Wunschliste:
    while(!StringIstZahl(input))
    {
        wunschsorten->push_back(indexObstsorte(input, indizesObstsorten, namenObstsorten));
        cin>>input;
    }

    //An dieser Stelle enthaelt input die Anzahl der Obstspiesse als String
    int spiesse = StringToInt(input);

    //Einlesen der Spiesse:
    for(int i = 0; i < spiesse; i++)
    {
        vector<int> schuesseln (0); //Menge der Schuesseln
        cin>>input;
        while(StringIstZahl(input))
        {
            schuesseln.push_back(StringToInt(input)-1);
            cin>>input;
        }
        //Die Menge der Schuesseln ist eingelesen!
        //An dieser Stelle enthaelt input eine der Obstsorten in diesen Schuesseln
        vector<int> sorten (0); //Menge der Sorten aus diesen Schuesseln
        sorten.push_back(indexObstsorte(input, indizesObstsorten, namenObstsorten));
        while(sorten.size() < schuesseln.size())
        {
            cin>>input;
            sorten.push_back(indexObstsorte(input, indizesObstsorten, namenObstsorten));
        }

        //Initialisieren des Containers SMengen
        for(int i : schuesseln)
        {
            (*ziehungen)[i]++;
            for(int j : sorten)
            {
                (*SMengen)[i][j]++;
            }
        }
    }
    fclose(stdin);
}

int main()
{
    //Variablen zum Speichern der Eingabedatei:
    map<string, int> indizesObstsorten; //Ordnet den Namen einer Obstsorte deren Index zu
    map<int, string> namenObstsorten; //Ordnet den Index einer Obstsorte deren Name zu
    int anzahlObstsorten; //Anzahl der Obstsorten
    vector<vector<int>> SMengen (0, vector<int> (0)); //Container fuer Mengen des Typs S_a (s. Dokumentation)
    vector<int> ziehungen (0); //Speichert fuer jede Schuessel a die Zahl z_a (s. Dokumentation)
    vector<int> wunschsorten (0); //Donalds Wunschsorten
    string dateinameAusgabedatei; //Name der Ausgabedatei
    //Einlesen der Eingabedaten:
    datenEinlesen(&indizesObstsorten, &namenObstsorten, &anzahlObstsorten, &SMengen, &ziehungen, &wunschsorten, &dateinameAusgabedatei);

    //Der erste Schritt des Schlussverfahrens:
    for(int i = 0; i < anzahlObstsorten; i++) //Durch alle Obstsorten gehen
    {
        //Maximum der Ziehungen dieser Obstsorte bestimmen:
        int maximaleZiehungszahl = 0;
        for(int j = 0; j < anzahlObstsorten; j++)
        {
            if(SMengen[j][i] > maximaleZiehungszahl)
            {
                maximaleZiehungszahl = SMengen[j][i];
            }
        }

        //Nicht brauchbare Zahlen auf -1 setzten:_
        for(int j = 0; j < anzahlObstsorten; j++)
        {
            if(SMengen[j][i] < maximaleZiehungszahl)
            {
                SMengen[j][i] = -1;
            }
        }
    }

    vector<vector<int>> MMengen (anzahlObstsorten, vector<int> (0)); //Container fuer Mengen des Typs M_a (s. Dokumentation)
    //Initialisieren der Containers MMengen:
    for(int i = 0; i < anzahlObstsorten; i++)
    {
        for(int j = 0; j < anzahlObstsorten; j++)
        {
            if(SMengen[i][j] == ziehungen[i])
            {
                //Wird in diesen Anweisungsblock gesprungen, war die Obstsorte j auf jedem Obstspiess aus der Schuessel i!
                MMengen[i].push_back(j);
            }
        }
    }

    //Der zweite Schritt des Schlussverfahrens:
    //Container, der festhaelt, ob eine Menge noch betrachtet wird:
    vector<bool> inBetrachtung (0);
    for(int i = 0; i < anzahlObstsorten; i++)
    {
        inBetrachtung.push_back(true);
    }

    //Durchlaufen des Containers MMengen:
    for(int i = 0; i < 26; i++)
    {
        if(inBetrachtung[i] && MMengen[i].size() == 1)
        {
            //Der Schuessel mit dem Index i kann eindeutig eine Obstsorte zugeordnet werden!
            inBetrachtung[i] = false; //Diese Schuessel aus der Betrachtung streichen
            //Streichen der Obstsorte in dieser Schuessel aus allen anderen Mengen:
            for(int j = 0; j < (int) MMengen.size(); j++)
            {
                for(int k = 0; inBetrachtung[j] && k < (int) MMengen[j].size(); k++)
                {
                    if(MMengen[j][k] == MMengen[i][0])
                    {
                        MMengen[j].erase(MMengen[j].begin() + k);
                        break;
                    }
                }
            }
            //Zaehler der for-Schleife zuruecksetzen:
            i = -1;
        }
    }

    //Der dritte Schritt des Schlussverfahrens:

    vector<int> vorhandenObstsorten (anzahlObstsorten); //Dieser Container zaehlt, wie oft jede Obstsorte gefunden wurde
    vector<int> schuesselIndizes (anzahlObstsorten); //Dieser Container haelt fuer jede Obstsorte fest, in welcher Schuessel sie zuletzt gefunden wurde
    //Container inBetrachtung zurücksetzen
    for(int i = 0; i < anzahlObstsorten; i++)
    {
        inBetrachtung[i] = true;
        vorhandenObstsorten[i] = 0;
    }

    for(int i = 0; i < anzahlObstsorten; i++) //Durch MMengen gehen
    {
        for(int j = 0; j < (int) MMengen[i].size() && inBetrachtung[i]; j++)
        {
            vorhandenObstsorten[MMengen[i][j]]++;
            schuesselIndizes[MMengen[i][j]] = i;
        }

        if(i == anzahlObstsorten-1)
        {
            //Der gesamte Container MMengen wurde durchlaufen!
            for(int j = 0; j < anzahlObstsorten; j++) //Durch Obstsorten gehen
            {
                if(vorhandenObstsorten[j] == 1)
                {
                    MMengen[schuesselIndizes[j]].clear();
                    MMengen[schuesselIndizes[j]].push_back(j);
                    inBetrachtung[schuesselIndizes[j]] = false;
                }
            }
            //Container vorhandenObstsorten zuruecksetzen:
            for(int j = 0; j < anzahlObstsorten; j++)
            {
                vorhandenObstsorten[j] = 0;
            }
        }
    }

    //Nun wird die Menge G aller Schuesseln, die mindestens eine von Donalds Wunschsorten enthalten koennten, bestimmt:
    vector<int> MengeG (0); //Container fuer die Menge G
    for(int i = 0; i < anzahlObstsorten; i++) //Durchlaufen des Containers MMengen
    {
        for(int j : MMengen[i])
        {
            if(find(wunschsorten.begin(), wunschsorten.end(), j) != wunschsorten.end())
            {
                //Eine der moeglichen Obstsorten fuer diese Schuesseln ist auch eine von Donalds Wunschsorten
                MengeG.push_back(i); //Index i der Menge G hinzufuegen
                break;
            }
        }
    }

    //Konstruktion der Menge W
    vector<int> MengeW (0);
    for(int i : MengeG) //durchlaufen der Menge G
    {
        for(int j : MMengen[i])
        {
            MengeW.push_back(j);
        }
    }

    //Entfernen doppelter Elemente aus der Menge W:
    for(int i = 0; i < (int) MengeW.size(); i++)
    {
        for(int j = i+1; j < (int) MengeW.size(); j++)
        {
            if(MengeW[i] == MengeW[j])
            {
                MengeW.erase(MengeW.begin()+j);
                j--;
            }
        }
    }

    //Oeffnen der Ausgabedatei:
    ofstream fout;
    fout.open(dateinameAusgabedatei, ios_base::out);

    if(MengeW.size() == wunschsorten.size())
    {
        //Es ist moeglich, eindeutig anzugeben, an welchen Schuesseln Donald sich anstellen muss
        fout<<"Donald hat genug Obstspiesse beobachtet, um sicher wissen zu koennen, an welchen Schuesseln er sich anstellen muss!"<<endl;
        fout<<"Schuesseln, in denen sich seine Wunschsorten befinden: ";
        for(int i : MengeG)
        {
            fout<<i+1<<" ";
        }
        fout<<endl;
        fout.close();
        return 0;
    }
    else
    {
        //Es ist nicht moeglich, eindeutig anzugeben, an welchen Schuesseln Donald sich anstellen muss
        fout<<"Donald hat leider nicht genug Obstspiesse beobachtet. Es kann nicht eindeutig festgestellt werden, an welchen Schuesseln er sich anstellen muss."<<endl;
        fout<<"Im Folgenden wird fuer jede Schuessel angegeben, welche Obstsorten sich in ihr befinden koennten. Diese Mengen sind so klein wie moeglich."<<endl;
        for(int i = 0; i < anzahlObstsorten; i++)
        {
            fout<<"Schuessel Nr. "<<i+1<<": ";
            if(namenObstsorten[MMengen[i][0]] != "") //Es sind Obstsorten bekannt, die in dieser Schuessel liegen koennten - sonst liegt hier wegen der Konstruktionsweise der MMEngen ein leerer String.
            {
                for(int j : MMengen[i])
                {
                    fout<<namenObstsorten[j]<<" ";
                }
                fout<<endl;
            }
            else
            {
                fout<<"Es ist keine Obstsorte bekannt, die sich in dieser Schuessel befinden koennte."<<endl;
            }
        }
    }

    return 0;
}
