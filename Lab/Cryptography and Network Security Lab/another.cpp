#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// GCD বের করার ফাংশন
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Multiplicative Inverse বের করার ফাংশন
int modInverse(int k) {
    for (int i = 1; i < 26; i++) {
        if ((k * i) % 26 == 1)
            return i;
    }
    return -1;
}

// এনক্রিপশন ফাংশন
string encrypt(string plaintext, int key) {
    string cipher = "";
    for (char c : plaintext) {
        if (isalpha(c)) {
            int p = tolower(c) - 'a';
            int ciph = (p * key) % 26;
            cipher += char(ciph + 'A');
        } else {
            cipher += c;
        }
    }
    return cipher;
}

// ডিক্রিপশন ফাংশন
string decrypt(string cipher, int key) {
    int inv = modInverse(key);
    if (inv == -1) return "Invalid Key";

    string plain = "";
    for (char c : cipher) {
        if (isalpha(c)) {
            int ciph = toupper(c) - 'A';
            int p = (ciph * inv) % 26;
            plain += char(p + 'a');
        } else {
            plain += c;
        }
    }
    return plain;
}

// --- Modified Statistical Attack ---
void statisticalAttack(string cipher) {
    cout << "\n--- Statistical Attack Results ---" << endl;
    cout << "Analyzing probabilities..." << endl;

    // ১. ফ্রিকোয়েন্সি ম্যাপ তৈরি
    map<char, int> freqMap;
    for (char c : cipher) {
        if (isalpha(c)) {
            freqMap[toupper(c)]++;
        }
    }

    if (freqMap.empty()) {
        cout << "No letters to analyze." << endl;
        return;
    }

    // ২. ফ্রিকোয়েন্সি অনুযায়ী সর্ট করা
    vector<pair<char, int>> sortedFreq(freqMap.begin(), freqMap.end());
    sort(sortedFreq.begin(), sortedFreq.end(),
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second;
         });

    // ডুপ্লিকেট রেজাল্ট এড়ানোর জন্য ব্যবহৃত কী (Key) স্টোর করে রাখব
    vector<int> usedKeys;

    // ৩. Top 3 ক্যারেক্টার চেক করা
    int limits = min((int)sortedFreq.size(), 3);

    for (int i = 0; i < limits; i++) {
        char cipherChar = sortedFreq[i].first;
        int c_val = cipherChar - 'A';
        int p_val = 4; // Assume 'e' = 4

        // ৪. K বের করার চেষ্টা
        int foundK = -1;
        for (int k = 1; k < 26; k++) {
            if (gcd(k, 26) == 1) {
                if ((p_val * k) % 26 == c_val) {
                    foundK = k;
                    break;
                }
            }
        }

        if (foundK != -1) {
            // চেক করি এই কী (Key) আগে ব্যবহার করেছি কিনা
            bool alreadyFound = false;
            for(int uk : usedKeys) {
                if(uk == foundK) alreadyFound = true;
            }

            if (!alreadyFound) {
                int inverseX = modInverse(foundK);
                cout << "\n[Possible Match #" << i + 1 << "]" << endl;
                cout << "  Assumption: '" << cipherChar << "' is 'e'" << endl;
                cout << "  Key Found: " << foundK << endl;
                cout << "  Decrypted Text: " << decrypt(cipher, foundK) << endl;

                // লিস্টে কী অ্যাড করলাম যাতে একই রেজাল্ট বারবার না দেখায়
                usedKeys.push_back(foundK);
            }
        }
    }

    if (usedKeys.empty()) {
        cout << "\nNo valid multiplicative keys found from top frequencies." << endl;
    } else {
        cout << "\nCheck the decrypted texts above. Choose the one that makes sense in English." << endl;
    }
}

int main() {
    string plaintext;
    int key;

    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    cout << "Enter key (must be coprime to 26): ";
    cin >> key;

    if (gcd(key, 26) != 1) {
        cout << "Invalid Key! GCD(key, 26) must be 1." << endl;
        return 0;
    }

    string cipher = encrypt(plaintext, key);
    cout << "\nEncrypted text: " << cipher << endl;

    // Attack
    statisticalAttack(cipher);

    return 0;
}
