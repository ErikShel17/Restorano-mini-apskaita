#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

const int MAX_ITEMS = 50;
const double TAX_RATE = 0.21;

struct menuItemType {
    string menuItem;     // patiekalo pavadinimas
    double menuPrice;    // patiekalo kaina
};

void getData(menuItemType menuList[], int& menuSize);
void showMenu(menuItemType menuList[], int menuSize);
void printReceipt(menuItemType menuList[], int quantities[], int menuSize);

int main() {
    menuItemType menuList[MAX_ITEMS];
    int quantities[MAX_ITEMS] = { 0 };
    int pasirinkimai[MAX_ITEMS];

    int menuSize = 0;
    int kiekPatiekalu;
    int choice;
    int amount;

    getData(menuList, menuSize);

    if (menuSize == 0) {
        cout << "Meniu nepavyko nuskaityti arba failas tuscias." << endl;
        return 1;
    }

    cout << fixed << setprecision(2);

    while (true) {
        showMenu(menuList, menuSize);

        cout << "\nKiek patiekalu norite pasirinkti? ";
        cout << "(0 - baigti uzsakyma): ";
        cin >> kiekPatiekalu;

        if (kiekPatiekalu == 0) {
            break;
        }

        if (kiekPatiekalu < 0 || kiekPatiekalu > menuSize) {
            cout << "Klaida: neteisingas patiekalu kiekis.\n";
        }
        else {
            cout << "Iveskite " << kiekPatiekalu << " patiekalu numerius: ";

            bool klaida = false;

            for (int i = 0; i < kiekPatiekalu; i++) {
                cin >> choice;

                if (choice < 1 || choice > menuSize) {
                    klaida = true;
                }

                pasirinkimai[i] = choice;
            }

            if (klaida) {
                cout << "Klaida: vienas arba keli patiekalu numeriai neegzistuoja meniu.\n";
            }
            else {
                for (int i = 0; i < kiekPatiekalu; i++) {
                    int index = pasirinkimai[i] - 1;

                    cout << "Kiek porciju norite patiekalui "
                        << menuList[index].menuItem << "? ";
                    cin >> amount;

                    if (amount <= 0) {
                        cout << "Klaida: porciju kiekis turi buti didesnis uz 0.\n";
                    }
                    else {
                        quantities[index] += amount;
                        cout << "Patiekalas pridetas i uzsakyma.\n";
                    }
                }
            }
        }
    }

    printReceipt(menuList, quantities, menuSize);

    return 0;
}

void getData(menuItemType menuList[], int& menuSize) {
    ifstream inFile("menu.txt");

    if (!inFile) {
        cout << "Klaida: nepavyko atidaryti menu.txt failo." << endl;
        return;
    }

    menuSize = 0;

    while (menuSize < MAX_ITEMS && getline(inFile, menuList[menuSize].menuItem)) {
        inFile >> menuList[menuSize].menuPrice;
        inFile.ignore();

        menuSize++;
    }

    inFile.close();
}

void showMenu(menuItemType menuList[], int menuSize) {
    cout << "\n========== PUSRYCIU MENIU ==========\n";
    cout << left << setw(5) << "Nr."
        << setw(25) << "Patiekalas"
        << right << setw(10) << "Kaina" << endl;

    cout << "------------------------------------\n";

    for (int i = 0; i < menuSize; i++) {
        cout << left << setw(5) << i + 1
            << setw(25) << menuList[i].menuItem
            << right << setw(10) << menuList[i].menuPrice << " EUR" << endl;
    }
}

void printReceipt(menuItemType menuList[], int quantities[], int menuSize) {
    ofstream outFile("receipt.txt");

    if (!outFile) {
        cout << "Klaida: nepavyko sukurti receipt.txt failo." << endl;
        return;
    }

    double subtotal = 0.0;

    outFile << fixed << setprecision(2);
    cout << fixed << setprecision(2);

    cout << "\n========== SASKAITA ==========\n";
    outFile << "========== SASKAITA ==========\n";

    cout << left << setw(25) << "Patiekalas"
        << setw(10) << "Kiekis"
        << setw(10) << "Kaina"
        << setw(10) << "Suma" << endl;

    outFile << left << setw(25) << "Patiekalas"
        << setw(10) << "Kiekis"
        << setw(10) << "Kaina"
        << setw(10) << "Suma" << endl;

    cout << "--------------------------------------------------\n";
    outFile << "--------------------------------------------------\n";

    for (int i = 0; i < menuSize; i++) {
        if (quantities[i] > 0) {
            double itemTotal = quantities[i] * menuList[i].menuPrice;
            subtotal += itemTotal;

            cout << left << setw(25) << menuList[i].menuItem
                << setw(10) << quantities[i]
                << setw(10) << menuList[i].menuPrice
                << setw(10) << itemTotal << endl;

            outFile << left << setw(25) << menuList[i].menuItem
                << setw(10) << quantities[i]
                << setw(10) << menuList[i].menuPrice
                << setw(10) << itemTotal << endl;
        }
    }

    double tax = subtotal * TAX_RATE;
    double total = subtotal + tax;

    cout << "--------------------------------------------------\n";
    cout << left << setw(35) << "Suma be PVM:" << subtotal << " EUR" << endl;
    cout << left << setw(35) << "PVM 21%:" << tax << " EUR" << endl;
    cout << left << setw(35) << "Galutine suma:" << total << " EUR" << endl;

    outFile << "--------------------------------------------------\n";
    outFile << left << setw(35) << "Suma be PVM:" << subtotal << " EUR" << endl;
    outFile << left << setw(35) << "PVM 21%:" << tax << " EUR" << endl;
    outFile << left << setw(35) << "Galutine suma:" << total << " EUR" << endl;

    outFile.close();

    cout << "\nSaskaita issaugota faile receipt.txt\n";
}