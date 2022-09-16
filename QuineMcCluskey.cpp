#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct Term {
    vector <int> numbers;
    string code;
    bool isPi;
};

struct Column {
    vector <vector<Term>> floors;
};

string binary_code (int num, unsigned int var);
string make_new_binary_code(string s1, string s2);
bool is_cube(string s1 , string s2);
bool is_valid_cube(Column column, Term t);
void create_next_column(vector <Column> &Mckelaski);
bool is_unique_epi(vector <Term> epiList, Term t);

int main()
{
    unsigned int var;
    cout << "Enter number of variables: ";
    cin >> var;
    int n;
    cout << "Enter the number of minterms: ";
    cin >> n;
    vector <int> minterms;
    for (int i = 0; i < n; i++) {
        int min;
        cout << i+1 <<"th minterm: ";
        cin >> min;
        minterms.push_back(min);
    }

    cout << "Enter the number of dontcares: ";
    cin >> n;
    vector <int> dontcares;
    for (int i = 0; i < n; i++) {
        int dont;
        cout << i+1 <<"th dontcare: ";
        cin >> dont;
        dontcares.push_back(dont);
    }

    vector <Column> Mckelaski;
    Column firstColumn;
    firstColumn.floors.resize(6);
    for (unsigned int i = 0; i < minterms.size(); i++) {
        Term temp;
        string binaryCode = binary_code(minterms[i], var);
        temp.code = binaryCode;
        temp.isPi = true;
        temp.numbers.push_back(minterms[i]);
        int countOne = 0;
        for (unsigned int j = 0; j < binaryCode.length(); j++) {
            if (binaryCode[j] == '1') {
                countOne++;
            }
        }
        firstColumn.floors[countOne].push_back(temp);
    }

    for (unsigned int i = 0; i < dontcares.size(); i++) {
        Term temp;
        string binaryCode = binary_code(dontcares[i], var);
        temp.code = binaryCode;
        temp.isPi = true;
        temp.numbers.push_back(dontcares[i]);
        int countOne = 0;
        for (unsigned int j = 0; j < binaryCode.length(); j++) {
            if (binaryCode[j] == '1') {
                countOne++;
            }
        }
        firstColumn.floors[countOne].push_back(temp);
    }

    Mckelaski.push_back(firstColumn);
    create_next_column(Mckelaski);

    vector <int> uniqueMinterms;
    vector <Term> EpiList;
    vector <Term> PiList;
    for (unsigned int i = 0; i < Mckelaski.size(); i++) {
        for (unsigned int j = 0; j < Mckelaski[i].floors.size(); j++) {
            for (unsigned int k = 0; k < Mckelaski[i].floors[j].size(); k++) {
                if (Mckelaski[i].floors[j][k].isPi == true) {
                    PiList.push_back(Mckelaski[i].floors[j][k]);
                }
            }
        }
    }


    for (unsigned int i = 0 ; i < PiList.size(); i++) {
        for (unsigned int j = 0; j < PiList[i].numbers.size(); j++) {
            bool check = true;
            for (unsigned int k = 0; k < dontcares.size(); k++) {
                if (PiList[i].numbers[j] == dontcares[k]) {
                    check = false;
                    break;
                }
            }
            if (check == true) {
                uniqueMinterms.push_back(PiList[i].numbers[j]);
            }
        }
    }

    for (unsigned int i = 0; i < uniqueMinterms.size(); i++) {
        int count = 0;
        for (unsigned int j = i + 1; j < uniqueMinterms.size(); j++) {
            if (uniqueMinterms[i] == uniqueMinterms[j]) {
                uniqueMinterms.erase(uniqueMinterms.begin() + j);
                j--;
                count++;
            }
        }
        if (count != 0) {
            uniqueMinterms.erase(uniqueMinterms.begin() + i);
            i--;
        }
    }

    for (unsigned int i = 0; i < PiList.size(); i++) {
        for (unsigned int j = 0; j < PiList[i].numbers.size(); j++) {
            for (unsigned int k = 0; k < uniqueMinterms.size(); k++) {
                if (PiList[i].numbers[j] == uniqueMinterms[k]) {
                    if (is_unique_epi(EpiList,PiList[i])) {
                        EpiList.push_back(PiList[i]);
                    }
                }
            }
        }
    }


    cout << endl << endl;
    for (unsigned int i = 0; i < PiList.size(); i++) {
        cout << i+1 << "th PI: ";
        for (unsigned int j = 0; j < PiList[i].numbers.size(); j++) {
            cout << PiList[i].numbers[j] << " ";
        }
        cout << endl;
    }
    cout << "\n**************************\n";
    for (unsigned int i = 0; i < EpiList.size(); i++) {
        cout << i+1 << "th EPI: ";
        for (unsigned int j = 0; j < EpiList[i].numbers.size(); j++) {
            cout << EpiList[i].numbers[j] << " ";
        }
        cout << endl;
    }
}

void create_next_column(vector <Column> &Mckelaski) {
    Column nextCol;
    nextCol.floors.resize(6);
    int indexLastcol = Mckelaski.size() - 1;
    for (unsigned int i = 0; i < Mckelaski[indexLastcol].floors.size() - 1; i++) {
        for (unsigned int j = 0; j < Mckelaski[indexLastcol].floors[i].size(); j++) {
            for (unsigned int k = 0; k < Mckelaski[indexLastcol].floors[i+1].size(); k++) {
                if (is_cube(Mckelaski[indexLastcol].floors[i][j].code,Mckelaski[indexLastcol].floors[i+1][k].code) == true) {
                    Mckelaski[indexLastcol].floors[i][j].isPi = false;
                    Mckelaski[indexLastcol].floors[i+1][k].isPi = false;
                    Term temp;
                    string s = make_new_binary_code(Mckelaski[indexLastcol].floors[i][j].code,Mckelaski[indexLastcol].floors[i+1][k].code);
                    temp.code = s;
                    temp.isPi = true;

                    for (unsigned int m = 0; m < Mckelaski[indexLastcol].floors[i][j].numbers.size(); m++) {
                        temp.numbers.push_back(Mckelaski[indexLastcol].floors[i][j].numbers[m]);
                    }
                    for (unsigned int m = 0; m < Mckelaski[indexLastcol].floors[i+1][k].numbers.size(); m++) {
                        temp.numbers.push_back(Mckelaski[indexLastcol].floors[i+1][k].numbers[m]);
                    }

                    if(is_valid_cube(nextCol, temp) == true) {
                        int countOne = 0;
                        for (unsigned int j = 0; j < s.length(); j++) {
                            if (s[j] == '1') {
                                countOne++;
                            }
                        }
                        nextCol.floors[countOne].push_back(temp);
                    }
                }
            }
        }
    }

    int count = 0;
    for (unsigned int i = 0; i < nextCol.floors.size(); i++) {
        if (nextCol.floors[i].size() != 0) {
            count++;
        }
    }

    if (count <= 0) {
        return;
    }
    else {
        Mckelaski.push_back(nextCol);
        create_next_column(Mckelaski);
    }

}

bool is_valid_cube(Column column, Term t) {
    for (unsigned int i = 0; i < column.floors.size(); i++) {
        for (unsigned int j = 0; j < column.floors[i].size(); j++) {
            if (t.code == column.floors[i][j].code) {
                return false;
            }
        }
    }
    return true;
}

bool is_unique_epi(vector <Term> epiList, Term t) {
    for (unsigned int i = 0; i < epiList.size(); i++) {
        if (epiList[i].code == t.code) {
            return false;
        }
    }
    return true;
}

string make_new_binary_code(string s1, string s2) {
    string newCode = "";
    bool flag = true;
    unsigned int i = 0;
    for (; i < s1.length() && flag == true; i++) {
        if (s1[i] != s2[i]) {
            newCode += '-';
        }
        else {
            newCode += s1[i];
        }
    }
    return newCode;
}

bool is_cube(string s1 , string s2) {
    int count = 0;
    for (unsigned int i = 0; i < s1.length(); i++) {
        if (s1[i] != s2[i]) {
            count++;
        }
    }
    if (count == 1) {
        return true;
    }
    return false;
}

string binary_code (int num, unsigned int var) {

    int bin = 0;
    int r = 0;  //remainder
    int p = 1;  //product
    string binary;
    while(num != 0) {
        r = num % 2;
        bin = bin + (r * p);
        num /= 2;
        p *= 10;
    }
    binary = to_string(bin);
    while (binary.length() < var) {
        binary.insert(0,"0");
    }
    return binary;
}