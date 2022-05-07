#include <bits/stdc++.h>
using namespace std;

struct eisbudenverteilung //Speichert eine Eisbudenverteilung durch die Adressen der Eisbuden
{
    int position1;
    int position2;
    int position3;
};

void eingabeeinlesen(int* teichumfang, int* haeuser, vector<int>* hausnummern, string* ausgabedatei) //Liest die Eingabedatei ein
{
    char eingabedatei[1000]; //Name der Eingabedatei
    cout<<"Geben Sie den Namen der Eingabedatei an: ";
    cin>>eingabedatei;
    cout<<"Geben Sie den Namen der Ausgabedatei an: ";
    cin>>*ausgabedatei;
    //Oeffnen der Eingabedatei:
    freopen(eingabedatei, "r", stdin);
    //Einlesen der Daten:
    cin>>*teichumfang;
    cin>>*haeuser;
    for(int i = 0; i < *haeuser; i++)
    {
        int nummer;
        cin>>nummer;
        hausnummern->push_back(nummer);
    }
    fclose(stdin);
}

int betrag(int a) //Betrag der Variablen a
{
    if(a < 0)
    {
        return -a;
    }
    return a;
}

int distanzEisbude(eisbudenverteilung buden, int haus, int teichumfang) //berechnet fuer ein Haus die minimale Distanz zur naechsten Eisbude
{
    int distanz1 = min(betrag(haus-buden.position1), teichumfang-betrag(haus-buden.position1));//Distanz zur ersten Eisbude
    int distanz2 = min(betrag(haus-buden.position2), teichumfang-betrag(haus-buden.position2));
    int distanz3 = min(betrag(haus-buden.position3), teichumfang-betrag(haus-buden.position3));
    return min(distanz1, min(distanz2, distanz3));
}

bool abstimmen(eisbudenverteilung statusquo, eisbudenverteilung neu, vector<int>* hausnummern, int haeuser, int teichumfang)
{
    //Diese Funktion liefert True zurueck, wenn die zweite Eisbudenverteilung eine Mehrheit von der ersten aus
    //bekommen wuerde und False sonst
    int mehrheit = haeuser/2 + 1; //Speichert, wie viele Haeuser mindestens fuer eine Umlegung zustimmen muessen
    int zustimmungen = 0; //Zahelt die Haueser, die einer Verlegung zustimmen
    for(int i : (*hausnummern))
    {
        if(distanzEisbude(neu, i, teichumfang) < distanzEisbude(statusquo, i, teichumfang))
        {
            zustimmungen++;
            //Bei Gleichheit stimmt das Haus gegen die Verlegung!
        }
    }
    if(zustimmungen >= mehrheit)
    {
        return true; //Die Mehrheit stimmt fuer eine Verlegung
    }
    return false;
}

bool evPruefen(eisbudenverteilung test, vector<int>* hausnummern, int haeuser, int teichumfang) //Ueberprueft eine Eisbudenverteilung auf Stabilitaet
{
    //Diese Funktion liefert True zurueck, wenn die zu untersuchende Eisbudenverteilung stabil ist und False sonst

    //Durchlaufen von Eisbudenverteilungen, in denen sich zwei Eisbuden zueinander bewegt haben:
    //1 und 2 bewegen sich aufeinander zu:
    if(test.position2 - test.position1-1 >= 2) //Die beiden koennen sich zueinander bewegen
    {
        for(int p = test.position2+1; (p%teichumfang) != test.position1; p++)
        {
            eisbudenverteilung vergleich;
            vergleich.position1 = test.position1+1;
            vergleich.position2 = test.position2-1;
            vergleich.position3 = (p%teichumfang);
            if(abstimmen(test, vergleich, hausnummern, haeuser, teichumfang))
                return false;
        }
    }
    //2 und 3 bewegen sich aufeinander zu:
    if(test.position3-test.position2-1 >= 2)
    {
        for(int p = test.position3+1; (p%teichumfang) != test.position2; p++)
        {
            eisbudenverteilung vergleich;
            vergleich.position1 = test.position2+1;
            vergleich.position2 = test.position3-1;
            vergleich.position3 = (p%teichumfang);
            if(abstimmen(test, vergleich, hausnummern, haeuser, teichumfang))
                return false;
        }
    }
    //3 und 1 bewegen sich aufeinander zu:
    if(max(test.position1-1, 0)+teichumfang-1-test.position3 >= 2)
    {
        for(int p = test.position1+1; p < test.position3; p++)
        {
            eisbudenverteilung vergleich;
            vergleich.position1 = (test.position3+1)%teichumfang;
            vergleich.position2 = test.position1-1;
            if(vergleich.position2 == -1)
                vergleich.position2 = teichumfang-1;
            vergleich.position3 = p;
            if(abstimmen(test, vergleich, hausnummern, haeuser, teichumfang))
                return false;

        }
    }

    //Durchlaufen von Eisbudenverteilungen, bei denen sich alle Eisbuden in die gleiche Richtung bewegt haben:
    for(int a = test.position1+1; a < test.position2; a++)
    {
        for(int b = test.position2+1; b < test.position3; b++)
        {
            for(int c = test.position3+1; (c%teichumfang) != test.position1; c++)
            {
                eisbudenverteilung vergleich;
                vergleich.position1 = a;
                vergleich.position2 = b;
                vergleich.position3 = (c%teichumfang);
                if(abstimmen(test, vergleich, hausnummern, haeuser, teichumfang))
                    return false;
            }
        }
    }

    //Die Eisbudenverteilung ist stabil!!!
    return true;
}

void ausgabe(string dateiname, eisbudenverteilung letzte, bool stabil) //Gibt das Ergebnis aus
{
    //Oeffnen der Ausgabedatei:
    ofstream ausgabedatei;
    ausgabedatei.open(dateiname, ios_base::out);

    if(stabil)
    {
        ausgabedatei<<"Es gibt mindestens einen stabilen Eisbudenstandort!"<<endl;
        ausgabedatei<<"Dabei haben die Eisbuden die Adressen "<<letzte.position1<<", "<<letzte.position2<<" und "<<letzte.position3<<"."<<endl;
    }
    else
    {
        ausgabedatei<<"Es gibt keine stabilen Eisbudenstandorte."<<endl;
    }

    //Schliessen der Ausgabedatei:
    ausgabedatei.close();
}

void init_container(map<int, int>* haeuserzahl, map<int, bool>* adressebesetzt, vector<int>* hausnummern, int haeuser, int teichumfang)
{
    //Diese Funktion initialisiert die beiden Container, die ihr ueber die ersten beiden Parameter uebergeben werden

    //Erst adressebesetzt initialisieren: True wenn ein Haus auf der entsprechenden Adresse steht, False sonst
    for(int i = 0; i < teichumfang; i++)
    {
        (*adressebesetzt)[i] = false;
    }
    for(int i = 0; i < haeuser; i++)
    {
        (*adressebesetzt)[(*hausnummern)[i]] = true;
    }

    //Initialisierung von haeuserzahl:
    (*haeuserzahl)[-1] = 0; //Noetig fuer spaetere Berechnung
    if((*adressebesetzt)[0])
        (*haeuserzahl)[0] = 1;
    else
        (*haeuserzahl)[0] = 0;
    for(int i = 1; i < teichumfang; i++)
    {
        if((*adressebesetzt)[i])
            (*haeuserzahl)[i] = (*haeuserzahl)[i-1]+1;
        else
            (*haeuserzahl)[i] = (*haeuserzahl)[i-1];
    }
}

bool checkNotwendigeBedingungen(eisbudenverteilung test, int haeuser, int teichumfang, map<int, int>* haeuseranzahlen, map<int, bool>* adressebesetzt)
{
    //Diese Funtion ueberprueft, ob eine Eisbudenverteilung die notwendigen Bedingungen fuer eine stabile Eisbudenverteilung erfuellt
    //Ist dem so. liefert sie True zurueck und False sonst
    //Erst ueberpruefen, ob "zu viele" Haueser zwischen zwei Eisbuden liegen - deren Adresse sind aufsteigend geordnet
    int noetigFuerMehrheit = haeuser/2 + 1; //Liegen zwischen zwei Eisbuden mindestens so viele Haueser, kann die Eisbudenverteilung test nicht stabil sein
    if((*haeuseranzahlen)[test.position1-1] + (*haeuseranzahlen)[teichumfang-1] - (*haeuseranzahlen)[test.position3] >= noetigFuerMehrheit)
       {
           //Eisbudenverteilung test kann nicht stabil sein
           return false;
       }
    //Diese Bedingung wird teilweise schon in der main-Funktion getestet

    //Nun zaehlen, wie viele Eisbuden in der Mitte ihres Einflussbereichs liegen:
    int inmitte = 0; //Zaehlt die in der Mitte ihres Einflussbereichs liegenden Eisbuden
    int zwischen12 = test.position2-1-test.position1; //Anzahl der Adressen zwischen den Eisbuden 1 und 2
    int zwischen23 = test.position3-1-test.position2; //Anzahl der Adressen zwischen den Eisbuden 2 und 3
    int zwischen13 = teichumfang-zwischen12-zwischen23-3; //Anzahl der Adressen zwischen den Eisbuden 1 und 3
    //Jede der folgenden Variablen bezieht sich jeweils auf den Zwischenraum zwischen zwei Eisbuden
    //Liegen in diesem gerade viele Adressen, enthaelt sie die letzte Adresse, die im Einflussbereich der Eisbude mit der niedrigeren Adresse liegt
    //Im anderen Fall enthaelt sie die Adresse, die von beiden Eisbuden gleich weit entfernt ist.
    int mittelfeld12, mittelfeld23, mittelfeld13;
    mittelfeld12 = test.position1+(zwischen12/2)+(zwischen12%2);
    mittelfeld23 = test.position2+(zwischen23/2)+(zwischen23%2);
    mittelfeld13 = (test.position3+(zwischen13/2)+(zwischen13%2))%teichumfang;
    if(zwischen13%2 == 0)
    {
        mittelfeld13 = (mittelfeld13+1)%teichumfang;
    }

    //Variablen fuer die zwei Haelften des EInflussbereichs einer Eisbude bzw. die Anzahl der Haeuser darin
    //Links steht fuer die Richtung, in der die adressnummern abnehmen
    int links, rechts;

    //Nun werden die drei Eisbuden durchlaufen:

    //Eisbude Nr. 1
    if(mittelfeld13 < test.position1)
    {
        if(zwischen13%2 == 0)
        {
            links = (*haeuseranzahlen)[test.position1-1]-(*haeuseranzahlen)[mittelfeld13-1];
        }
        else
        {
            links = (*haeuseranzahlen)[test.position1-1]-(*haeuseranzahlen)[mittelfeld13];
        }
    }
    else
    {
        if(zwischen13%2 == 0)
        {
            links = (*haeuseranzahlen)[test.position1-1]+(*haeuseranzahlen)[teichumfang-1]-(*haeuseranzahlen)[mittelfeld13-1];
        }
        else
        {
            links = (*haeuseranzahlen)[test.position1-1]+(*haeuseranzahlen)[teichumfang-1]-(*haeuseranzahlen)[mittelfeld13];
        }
    }
    if(zwischen12 % 2 == 1)
    {
        rechts = (*haeuseranzahlen)[mittelfeld12-1]-(*haeuseranzahlen)[test.position1];
    }
    else
    {
        rechts = (*haeuseranzahlen)[mittelfeld12]-(*haeuseranzahlen)[test.position1];
    }

    if((*adressebesetzt)[mittelfeld13] && zwischen13%2 == 1 && links < rechts)
    {
        links++;
    }
    if((*adressebesetzt)[mittelfeld12] && zwischen12%2 == 1 && rechts < links)
    {
        rechts++;
    }
    if(betrag(rechts - links) == 1 && (*adressebesetzt)[test.position1])
    {
        rechts = links;
    }

    if(rechts == links)
    {
        inmitte++;
    }

    //Eisbude Nr. 2
    links = (*haeuseranzahlen)[test.position2-1]-(*haeuseranzahlen)[mittelfeld12];
    if(zwischen23 % 2 == 1)
    {
        rechts = (*haeuseranzahlen)[mittelfeld23-1]-(*haeuseranzahlen)[test.position2];
    }
    else
    {
        rechts = (*haeuseranzahlen)[mittelfeld23]-(*haeuseranzahlen)[test.position2];
    }

    if((*adressebesetzt)[mittelfeld12] && zwischen12%2 == 1 && links < rechts)
    {
        links++;
    }
    if((*adressebesetzt)[mittelfeld23] && zwischen23%2 == 1 && rechts < links)
    {
        rechts++;
    }
    if(betrag(rechts - links) == 1 && (*adressebesetzt)[test.position2])
    {
        rechts = links;
    }

    if(rechts == links)
    {
        inmitte++;
    }

    //Eisbude Nr. 3:
    links = (*haeuseranzahlen)[test.position3-1]-(*haeuseranzahlen)[mittelfeld23];
    if(mittelfeld13 > test.position3)
    {
        rechts = (*haeuseranzahlen)[mittelfeld13-1]-(*haeuseranzahlen)[test.position3];
    }
    else
    {
        rechts = (*haeuseranzahlen)[mittelfeld13-1]+(*haeuseranzahlen)[teichumfang-1]-(*haeuseranzahlen)[test.position3];
    }

    if((*adressebesetzt)[mittelfeld13] && zwischen13%2 == 1 && rechts < links)
    {
        rechts++;
    }
    if((*adressebesetzt)[mittelfeld23] && zwischen23%2 == 1 && links < rechts)
    {
        links++;
    }
    if(betrag(rechts - links) == 1 && (*adressebesetzt)[test.position3])
    {
        rechts = links;
    }

    if(rechts == links)
    {
        inmitte++;
    }

    if(inmitte < 2)
    {
        //Es liegen mindestens zwei Eisbuden ausserhalb der Mitte ihres Einflussbereichs
        return false;
    }

    return true;
}

int main()
{
    //Variablen fuer das Speichern der eingegebenen Daten:
    int teichumfang; //Umfang des Sees in Schritten
    int haeuser; //Anzahl der Haeuser
    vector<int> hausnummern (0); //Die Adressen der Haeuser um den See
    string ausgabedatei; //Der Name der Ausgabedatei


    //Einlesen der Eingabe:
    eingabeeinlesen(&teichumfang, &haeuser, &hausnummern, &ausgabedatei);

    //Container fuer das Ueberpruefen der notwendigen Bedingungen fuer eine stabile Eisbudenverteilung:
    map<int, int> haeuserzahl; //Ordnet jeder Adresse die Anzahl der Haueser im Bereich zwischen der Adresse 0 und dieser Adresse (inklusive) zu
    map<int, bool> adressebesetzt; //Gibt fuer jede Adresse an, ob sich ein Haus auf ihr befindet.
    init_container(&haeuserzahl, &adressebesetzt, &hausnummern, haeuser, teichumfang); //Initialisieren der beiden Container

    bool gefunden = false; //True, wenn eine stabile Eisbudenverteilung gefunden wurde
    eisbudenverteilung evstabil; //Speichert eine evtl. existierende stabile Eisbudenverteilung
    int testcounter = 0; //Gibt spaeter aus, wie viele Eisbudenverteilungen untersucht wurden

    //Nun durch Eisbudenverteilungen gehen:
    for(int a = 0; a < teichumfang && !gefunden; a++)
    {
        for(int b = a+1; b < teichumfang && haeuserzahl[b-1] - haeuserzahl[a] < haeuser/2 + 1 && !gefunden; b++)
        {
            for(int c = b+1; c < teichumfang && haeuserzahl[c-1] - haeuserzahl[b] < haeuser/2 + 1 && !gefunden; c++)
            {
                eisbudenverteilung test;
                test.position1 = a;
                test.position2 = b;
                test.position3 = c;
                if(checkNotwendigeBedingungen(test, haeuser, teichumfang, &haeuserzahl, &adressebesetzt))
                {
                    testcounter++;
                    if(evPruefen(test, &hausnummern, haeuser, teichumfang))
                    {
                        //Eine stabile Eisbudenverteilung wurde gefunden!
                        gefunden = true;
                        evstabil = test;
                    }
                }
            }
        }
    }

    //Ausgabe:
    ausgabe(ausgabedatei, evstabil, gefunden);

    return 0;
}
