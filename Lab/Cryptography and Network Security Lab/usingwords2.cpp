#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// g -> gcd (Greatest Common Divisor)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// m -> modInverse (Modular Inverse calculation)
int modInverse(int a) {
    for (int i = 1; i < 26; ++i) {
        if ((a * i) % 26 == 1) return i;
    }
    return -1;
}

// e -> encrypt (Encryption Function)
string encrypt(string text, int key) {
    string result = "";
    for (size_t i = 0; i < text.length(); i++) {
        if (isalpha(text[i])) {
            int val = tolower(text[i]) - 'a';
            int cipherVal = (val * key) % 26;
            result += (char)(cipherVal + 'a');
        } else {
            result += text[i];
        }
    }
    return result;
}

// d -> decrypt (Decryption Function)
string decrypt(string cipher, int key) {
    int inverseKey = modInverse(key);
    if (inverseKey == -1) return "";
    string result = "";
    for (size_t i = 0; i < cipher.length(); i++) {
        if (isalpha(cipher[i])) {
            int val = tolower(cipher[i]) - 'a';
            int plainVal = (val * inverseKey) % 26;
            result += (char)(plainVal + 'a');
        } else {
            result += cipher[i];
        }
    }
    return result;
}

// t -> getTop3Chars (Finds top 3 frequent characters)
string getTop3Chars(string text) {
    int freq[26] = {0};
    for (size_t i = 0; i < text.length(); i++) {
        if (isalpha(text[i])) {
            freq[tolower(text[i]) - 'a']++;
        }
    }

    string topChars = "";
    for (int i = 0; i < 3; i++) {
        int maxVal = -1;
        int maxIdx = -1;
        for (int j = 0; j < 26; j++) {
            if (freq[j] > maxVal) {
                maxVal = freq[j];
                maxIdx = j;
            }
        }
        if (maxIdx != -1) {
            topChars += (char)(maxIdx + 'a');
            freq[maxIdx] = -1; // Mark as visited
        }
    }
    return topChars;
}

// z -> statisticalAttack (The main logic for breaking the code)
void statisticalAttack(string cipherText, string sampleText) {
    cout << "\n--- Statistical Attack ---" << endl;

    // w -> commonWords
    string commonWords[] = {
        "the", "be", "to", "of", "and", "a", "in", "that", "have", "it",
        "for", "not", "on", "with", "he", "as", "you", "do", "at", "this"
    };

    string topCipher = getTop3Chars(cipherText);
    string topSample = getTop3Chars(sampleText);

    if (topCipher.length() == 0 || topSample.length() == 0) return;

    cout << "Top Cipher Chars: [";
    for (size_t i = 0; i < topCipher.length(); ++i) {
        cout << "'" << topCipher[i] << "'";
        if (i < topCipher.length() - 1) cout << ", ";
    }
    cout << "]" << endl;

    cout << "Top Sample Chars: [";
    for (size_t i = 0; i < topSample.length(); ++i) {
        cout << "'" << topSample[i] << "'";
        if (i < topSample.length() - 1) cout << ", ";
    }
    cout << "]" << endl;

    int bestKey = -1;
    int bestInverse = -1;
    string bestText = "";

    // mx -> maxScore (Initialized to 0 to require at least 1 match)
    int maxScore = 0;

    for (size_t i = 0; i < topCipher.length(); i++) {
        for (size_t j = 0; j < topSample.length(); j++) {

            int u = topCipher[i] - 'a';
            int v = topSample[j] - 'a';

            if (v == 0) continue;

            int candidateKey = -1; // fk -> candidateKey
            for (int k = 1; k < 26; ++k) {
                if (gcd(k, 26) == 1) {
                    if ((v * k) % 26 == u) {
                        candidateKey = k;
                        break;
                    }
                }
            }

            if (candidateKey != -1) {
                string tempDecrypted = decrypt(cipherText, candidateKey);
                int currentScore = 0; // cnt -> currentScore

                for (int x = 0; x < 20; x++) {
                    if (tempDecrypted.find(commonWords[x]) != string::npos) {
                        currentScore++;
                    }
                }

                if (currentScore > maxScore) {
                    maxScore = currentScore;
                    bestKey = candidateKey;
                    bestInverse = modInverse(candidateKey);
                    bestText = tempDecrypted;
                }
            }
        }
    }

    if (bestKey != -1) {
        cout << "\n[RESULT FOUND]" << endl;
        cout << " -> Encryption Key (K): " << bestKey << endl;
        cout << " -> Decryption Key (Inverse): " << bestInverse << endl;
        cout << " -> Decrypted Text: " << bestText << endl;
    } else {
        cout << "\n[FAILED] No valid English words found in any hypothesis." << endl;
    }
}

int main() {
    string plaintext, sampleText;
    int key;

    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    cout << "Enter Encryption key (must be coprime to 26): ";
    if (!(cin >> key)) return 0;

    if (gcd(key, 26) != 1) {
        cout << "Invalid Key! GCD(key, 26) must be 1." << endl;
        return 0;
    }

    cin.ignore();

    cout << "\n--- For Statistical Attack ---" << endl;
    cout << "Enter a sample text:" << endl;
    getline(cin, sampleText);

    if (sampleText.length() < 1) {
        sampleText = "the quick brown fox jumps over the lazy dog and that is very common";
    }

    string cipherText = encrypt(plaintext, key);
    cout << "\nEncrypted text: " << cipherText << endl;

    statisticalAttack(cipherText, sampleText);

    return 0;
}
