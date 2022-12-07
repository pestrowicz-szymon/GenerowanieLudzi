#include <iostream>
#include <random>
#include <algorithm>

using namespace::std;

random_device rd;
mt19937 gen(rd());

string imionaM[10] = {"Andrzej", "Marian", "Zbigniew", "Wojciech", "Karol", "Janusz", "Sebastian", "Jakub", "Kazimierz", "Marcin" };
string imionaK[10] = {"Katarzyna", "Cecylia", "Honorata", "Melania", "Malgorzata", "Karolina", "Wiktoria", "Ilona", "Aleksandra", "Alicja"};
string nazwiska[10] = {"Nowak", "Wojcik", "Kowalczyk", "Wozniak", "Mazur", "Krol", "Zajac", "Wieczorek", "Wrobel", "Pawlak"};

bool mezczyzna() {
    uniform_int_distribution<> distr(0,1);
    return distr(gen);
}

int *losoweImieINazwisko() {
    static int losoweDane[2];
    uniform_int_distribution<> distr(0,9);
    losoweDane[0] = distr(gen);
    losoweDane[1] = distr(gen);
    return losoweDane;
}

int *losowaData() {
    static int losowyDzien[3];
    uniform_int_distribution<> distr(1963,2003);
    losowyDzien[0] = distr(gen);
    distr = uniform_int_distribution<>(1,12);
    losowyDzien[1] = distr(gen);
    if (losowyDzien[1] == 1 || losowyDzien[1] == 3 || losowyDzien[1] == 5 || losowyDzien[1] == 7 || losowyDzien[1] == 8 || losowyDzien[1] == 10 || losowyDzien[1] == 12) {
        distr = uniform_int_distribution<>(1,31);
    } else if (losowyDzien[1] == 2) {
        if((losowyDzien[0] % 4 == 0 && losowyDzien[0] % 100 != 0) || losowyDzien[0] % 400 == 0) {
            distr = uniform_int_distribution<>(1,29);
        }
        else {
            distr = uniform_int_distribution<>(1,28);
        }
    } else {
        distr = uniform_int_distribution<>(1,30);
    }
    losowyDzien[2] = distr(gen);
    return losowyDzien;
}



int *wygenerujPESEL(int dzienUrodzenia[3], bool jestMezczyzna) {
    static int PESEL [11];
    PESEL [0] = ((dzienUrodzenia[0]%100)/10);
    PESEL [1] = (dzienUrodzenia[0]%10);
    if(dzienUrodzenia[0] > 1999) {
        PESEL [2] = ((dzienUrodzenia[1]/10)+2);
    } else {
        PESEL [2] = (dzienUrodzenia[1]/10);
    }
    PESEL [3] = (dzienUrodzenia[1]%10);
    PESEL [4] = (dzienUrodzenia[2]/10);
    PESEL [5] = (dzienUrodzenia[2]%10);
    uniform_int_distribution<> distr(0,9);
    PESEL [6] = distr(gen);
    PESEL [7] = distr(gen);
    PESEL [8] = distr(gen);
    distr = uniform_int_distribution<>(0,4);
    if(jestMezczyzna) {
        PESEL [9] = ((distr(gen) * 2 + 1));
    } else {
        PESEL [9] = (( distr(gen) * 2));
    }
    PESEL[10] = ((10 - ((((PESEL[0]*1)%10) + ((PESEL[1]*3)%10) + ((PESEL[2]*7)%10) + ((PESEL[3]*9)%10) + ((PESEL[4]*1)%10) + ((PESEL[5]*3)%10) + ((PESEL[6]*7)%10) + ((PESEL[7]*9)%10) + ((PESEL[8]*1)%10) + ((PESEL[9]*3)%10))%10))%10);

    return PESEL;
}

int main() {
    cout << "INSERT INTO ludzie VALUES " << endl;
    for(int i=0; i<45; i++) {
        cout << "(";
        cout << "'" << i+1 << "',";         // id

        // losujemy dane o czlowieku
        int *dataUrodzenia = losowaData();
        bool jestMezczyzna = mezczyzna();
        int *pesel = wygenerujPESEL(dataUrodzenia, jestMezczyzna);
        int *imieINazwisko = losoweImieINazwisko();

        cout << "'";
        for(int i=0; i<11; i++) {
            if (*(pesel+1) > 9){
                cout << "ERROR";
            }
            cout << *(pesel+i);             // pesel
        }
        cout << "',";

        cout << "'";
        if (jestMezczyzna) {
            cout << imionaM[*imieINazwisko];    // imie meskie
        } else {
            cout << imionaK[*imieINazwisko];    // imie zenskie
        }
        cout << "',";

        cout << "'" << nazwiska[*(imieINazwisko+1)] << "',";    // nazwisko

        cout << "'" << *dataUrodzenia << "-" << *(dataUrodzenia+1) << "-" << *(dataUrodzenia+2) << "',";    // data urodzenia

        cout << "'";
        if (jestMezczyzna) {
            cout << "M";                    // plec meska
        } else {
            cout << "K";                    // imie zenska
        }
        cout << "'";

        cout << ")";

        if(i != 44) {
            cout << "," << endl;
        }
    }

    cout << ";";
}
