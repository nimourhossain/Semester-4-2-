#include <bits/stdc++.h>
using namespace std;


string encrypt(string text, int key) {
    string result = "";
    for (char c : text)
        result += char((c - 'a' + key) % 26 + 'a');
    return result;
}


string decrypt(string text, int key) {
    string result = "";
    for (char c : text)
        result += char((c - 'a' - key + 26) % 26 + 'a');
    return result;
}

int main() {
    string p, d;
    int k;

    cout << "Enter message: ";
    cin >> p;

    cout << "Enter key: ";

    cin >> k;

    string e = encrypt(p, k);
    cout << "Encrypted text: " << e << endl;

    string w[10] = {"me","very","good","code","box","not","possible","well","book","room"};

    for (int x = 0; x < 26; x++) {
        d = decrypt(e, x);
        for (int i = 0; i < 10; i++) {
            if (d.find(w[i]) != -1) {
                cout << "Decoded text: " << d << endl;
                cout << "Decryption key: " << x << endl;
                return 0;
            }

        }
    }

    return 0;
}
