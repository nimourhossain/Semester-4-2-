
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// GCD Function
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Multiplicative Inverse Function
int modInverse(int k) {
    for (int i = 1; i < 26; i++) {
        if ((k * i) % 26 == 1)
            return i;
    }
    return -1;
}

// Encryption Function
string encrypt(string plaintext, int key) {
    string cipher = "";
    for (char c : plaintext) {
        if (isalpha(c)) {
            int p = tolower(c) - 'a';
            int ciph = (p * key) % 26;
            cipher += char(ciph + 'a');
        } else {
            cipher += c;
        }
    }
    return cipher;
}

// Decryption Function
string decrypt(string cipher, int key) {
    int inv = modInverse(key);
    if (inv == -1) return "Invalid Key for Decryption";

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

// Statistical Attack Function (Modified with Y/N check)
void statisticalAttack(string cipher) {
    cout << "\n--- Statistical Attack Results ---" << endl;

    // 1. Frequency Map
    map<char, int> freqMap;
    for (char c : cipher) {
        if (isalpha(c)) {
            freqMap[toupper(c)]++;
        }
    }

    if (freqMap.empty()) {
        cout << "No letters found to analyze." << endl;
        return;
    }

    // 2. Sort by Frequency
    vector<pair<char, int>> sortedFreq(freqMap.begin(), freqMap.end());
    sort(sortedFreq.begin(), sortedFreq.end(),
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second;
         });

    // 3. Loop through Top 3
    int limits = min((int)sortedFreq.size(), 3);

    for (int i = 0; i < limits; i++) {
        char cipherChar = sortedFreq[i].first;
        int c_val = cipherChar - 'A';
        int p_val = 4; // Assuming 'e' (value 4)

        cout << "\nAttempt " << i + 1 << ": Assuming '" << cipherChar << "' maps to 'e' (4)" << endl;

        // 4. Find K
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
            // 5. Decrypt and Ask User
            int inverseX = modInverse(foundK);
            string decryptedText = decrypt(cipher, foundK);

            cout << "  -> Found Potential Key (K): " << foundK << endl;
            cout << "  -> Multiplicative Inverse (X): " << inverseX << endl;
            cout << "  -> Decrypted Message: " << decryptedText << endl;

            // --- NEW CODE START ---
            char choice;
            cout << "  >>> Is this the correct text? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                cout << "\n[SUCCESS] Attack Successful! Loop stopped." << endl;
                break; // This breaks the loop
            } else {
                cout << "  -> Continuing to next hypothesis..." << endl;
            }
            // --- NEW CODE END ---

        } else {
            cout << "  -> No valid key found for this assumption." << endl;
        }
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

    // Run Attack
    statisticalAttack(cipher);

    return 0; // Fixed the typo 'ret urn' from original code
}
