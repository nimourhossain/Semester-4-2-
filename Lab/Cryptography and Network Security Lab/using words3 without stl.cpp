#include<bits/stdc++.h>

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

void encrypt(char plaintext[], int key, char cipher[]) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            int p = tolower(plaintext[i]) - 'a';
            int c = (p * key) % 26;
            cipher[i] = char(c + 'a');
        } else {
            cipher[i] = plaintext[i];
        }
    }
    cipher[len] = '\0';
}

void decrypt(char cipher[], int key, char plain[]) {
    int inv = modInverse(key);
    if (inv == -1) {
        strcpy(plain, "Invalid Key");
        return;
    }

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

void getTopFrequentChars(char text[], char result[], int n) {
    int freq[26] = {0};

    for (int i = 0; text[i]; i++) {
        if (isalpha(text[i]))
            freq[tolower(text[i]) - 'a']++;
    }

    for (int i = 0; i < n; i++) {
        int maxIdx = -1;
        for (int j = 0; j < 26; j++) {
            if (freq[j] > 0 && (maxIdx == -1 || freq[j] > freq[maxIdx]))
                maxIdx = j;
        }
        if (maxIdx == -1) break;
        result[i] = char(maxIdx + 'a');
        freq[maxIdx] = 0;
    }
}

void statistical_attack_interactive(char cipher[], char sample_text[]) {
    cout << "\n--- Statistical Attack (Automated Mode) ---" << endl;

    char common_words[20][10] = {
        "the","be","to","of","and","a","in","that","have","i",
        "it","for","not","on","with","he","as","you","do","at"
    };

    char top_cipher[3] = {0};
    char top_sample[3] = {0};

    getTopFrequentChars(cipher, top_cipher, 3);
    getTopFrequentChars(sample_text, top_sample, 3);

    cout << "Top Cipher Chars: [";
    for (int i = 0; i < 3; i++) {
        if (top_cipher[i]) {
            cout << "'" << top_cipher[i] << "'";
            if (i < 2) cout << ", ";
        }
    }
    cout << "]" << endl;

    cout << "Top Sample Chars: [";
    for (int i = 0; i < 3; i++) {
        if (top_sample[i]) {
            cout << "'" << top_sample[i] << "'";
            if (i < 2) cout << ", ";
        }
    }
    cout << "]" << endl;

    cout << "Analyzing hypotheses and checking dictionary..." << endl;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (!top_cipher[i] || !top_sample[j]) continue;

            int c_val = top_cipher[i] - 'a';
            int p_val = top_sample[j] - 'a';

            if (p_val == 0) continue;

            int found_k = -1;
            for (int k = 1; k < 26; k++) {
                if (gcd(k, 26) == 1 && (p_val * k) % 26 == c_val) {
                    found_k = k;
                    break;
                }
            }

            if (found_k != -1) {
                char decrypted[1000];
                decrypt(cipher, found_k, decrypted);

                int match_count = 0;
                for (int w = 0; w < 20; w++) {
                    if (strstr(decrypted, common_words[w]) != NULL)
                        match_count++;
                }

                if (match_count > 0) {
                    cout << "\n[SUCCESS] Valid English Text Found!" << endl;
                    cout << "Hypothesis: Cipher '" << top_cipher[i]
                         << "' -> Sample '" << top_sample[j] << "'" << endl;
                    cout << "  -> Encryption Key (K): " << found_k << endl;
                    cout << "  -> Decryption Key (Inverse): "
                         << modInverse(found_k) << endl;
                    cout << "  -> Decrypted Text: " << decrypted << endl;
                    return;
                }
            }
        }
    }

    cout << "\n[FAILED] Could not automatically determine the correct key based on the top 20 words." << endl;
}

int main() {
    char plaintext[1000];
    char sample_text[1000];
    char cipher[1000];
    int key;

    cout << "Enter plaintext: ";
    cin.getline(plaintext, 1000);

    cout << "Enter Encryption key (must be coprime to 26): ";
    cin >> key;

    if (gcd(key, 26) != 1) {
        cout << "Invalid Key! GCD(key, 26) must be 1." << endl;
        return 0;
    }

    cin.ignore();

    cout << "\n--- For Statistical Attack ---" << endl;
    cout << "Enter a sample text (used to calculate frequency):" << endl;
    cin.getline(sample_text, 1000);

    if (strlen(sample_text) < 1)
        strcpy(sample_text, "eeee tttt aaaa oooo iiii");

    encrypt(plaintext, key, cipher);
    cout << "\nEncrypted text: " << cipher << endl;

    statistical_attack_interactive(cipher, sample_text);
    return 0;
}
