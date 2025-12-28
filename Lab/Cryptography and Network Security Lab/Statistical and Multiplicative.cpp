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

// Multiplicative Inverse বের করার ফাংশন (X বের করা)
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
            cipher += char(ciph + 'a'); // আউটপুট আপারকেস হবে
        } else {
            cipher += c; // স্পেস বা অন্য ক্যারেক্টার অপরিবর্তিত থাকবে
        }
    }
    return cipher;
}

// ডিক্রিপশন ফাংশন (Ciphertext এবং Key ব্যবহার করে)
string decrypt(string cipher, int key) {
    int inv = modInverse(key);
    if (inv == -1) return "Invalid Key for Decryption";

    string plain = "";
    for (char c : cipher) {
        if (isalpha(c)) {
            int ciph = toupper(c) - 'A'; // ইনপুট আপারকেস ধরে নেওয়া হচ্ছে
            int p = (ciph * inv) % 26;
            plain += char(p + 'a'); // আউটপুট লোয়ারকেস হবে
        } else {
            plain += c;
        }
    }
    return plain;
}

// Statistical Attack ফাংশন (Top 3 Frequent Character চেক করবে)
void statisticalAttack(string cipher) {
    cout << "\n--- Statistical Attack Results ---" << endl;

    // ১. ফ্রিকোয়েন্সি ম্যাপ তৈরি
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

    // ২. ফ্রিকোয়েন্সি অনুযায়ী সর্ট করা (Descending Order)
    vector<pair<char, int>> sortedFreq(freqMap.begin(), freqMap.end());
    sort(sortedFreq.begin(), sortedFreq.end(),
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second; // বেশি ফ্রিকোয়েন্সি আগে থাকবে
         });

    // ৩. Top 3 ক্যারেক্টার দিয়ে লুপ চালানো
    int limits = min((int)sortedFreq.size(), 3);

    for (int i = 0; i < limits; i++) {
        char cipherChar = sortedFreq[i].first;
        int c_val = cipherChar - 'A';
        int p_val = 4; // ধরে নিচ্ছি প্লেইনটেক্সট 'e' (যার ভ্যালু 4)

        cout << "\nAttempt " << i + 1 << ": Assuming '" << cipherChar << "' maps to 'e' (4)" << endl;

        // ৪. K বের করা: (4 * K) % 26 == c_val
        int foundK = -1;
        for (int k = 1; k < 26; k++) {
            // Key অবশ্যই 26 এর সাথে কো-প্রাইম হতে হবে
            if (gcd(k, 26) == 1) {
                if ((p_val * k) % 26 == c_val) {
                    foundK = k;
                    break;
                }
            }
        }

        if (foundK != -1) {
            // ৫. K পাওয়া গেলে Inverse (X) বের করা এবং ডিক্রিপ্ট করা
            int inverseX = modInverse(foundK);
            cout << "  -> Found Potential Key (K): " << foundK << endl;
            cout << "  -> Multiplicative Inverse (X): " << inverseX << endl;
            cout << "  -> Decrypted Message: " << decrypt(cipher, foundK) << endl;
        } else {
            cout << "  -> No valid key found for this assumption (Equation 4*K = " << c_val << " mod 26 has no solution)." << endl;
        }
    }
}

int main() {
    string plaintext;
    int key;

    // ইনপুট নেওয়া
    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    cout << "Enter key (must be coprime to 26): ";
    cin >> key;

    // ভ্যালিডেশন চেক
    if (gcd(key, 26) != 1) {
        cout << "Invalid Key! GCD(key, 26) must be 1." << endl;
        return 0;
    }

    // এনক্রিপশন
    string cipher = encrypt(plaintext, key);
    cout << "\nEncrypted text: " << cipher << endl;

    // সাধারণ ডিক্রিপশন (চেক করার জন্য)
    cout << "Decrypted with original key: " << decrypt(cipher, key) << endl;

    // স্ট্যাটিসটিক্যাল অ্যাটাক চালানো
    statisticalAttack(cipher);

    return 0;
}
