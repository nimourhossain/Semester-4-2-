#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int modInverse(int k) {
    for (int i = 1; i < 26; i++) {
        if ((k * i) % 26 == 1)
            return i;
    }
    return -1;
}

void decrypt(char cipher[], int key, char plain[]) {
    int inv = modInverse(key);
    int len = strlen(cipher);
    for (int i = 0; i < len; i++) {
        if (isalpha(cipher[i])) {
            int c = tolower(cipher[i]) - 'a';
            int p = (c * inv) % 26;
            plain[i] = char(p + 'a');
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[len] = '\0';
}

int main() {
    char cipher[1000], plain[1000];
    char common_words[20][10] = {
        "the","be","to","of","and","a","in","that","have","i",
        "it","for","not","on","with","he","as","you","do","at"
    };

    cout << "Enter the Cipher Text: ";
    cin.getline(cipher, 1000);

    cout << "\nScanning for correct key..." << endl;

    bool found = false;

    for (int k = 1; k < 26; k++) {
        if (gcd(k, 26) == 1) {
            decrypt(cipher, k, plain);
            int match_count = 0;

            for (int w = 0; w < 20; w++) {
                if (strstr(plain, common_words[w]) != NULL) {
                    match_count++;
                }
            }

            if (match_count > 0) {
                cout << "\n[SUCCESS] Meaningful Text Found!" << endl;
                cout << "Encryption Key  : " << k << endl;
                cout << "Decryption Key  : " << modInverse(k) << endl;
                cout << "Decrypted Msg   : " << plain << endl;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        cout << "\n[FAILED] Could not automatically decrypt using the common words list." << endl;
    }

    return 0;
}
