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

void decrypt_affine(char cipher[], int a, int b, char plain[]) {
    int inv = modInverse(a);
    if (inv == -1) {
        strcpy(plain, "");
        return;
    }

    int len = strlen(cipher);
    for (int i = 0; i < len; i++) {
        if (isalpha(cipher[i])) {
            int c = tolower(cipher[i]) - 'a';
            int val = c - b;
            while (val < 0) val += 26;
            int p = (val * inv) % 26;
            plain[i] = char(p + 'a');
        } else {
            plain[i] = cipher[i];
        }
    }
    plain[len] = '\0';
}

void affine_auto_attack(char cipher[]) {
    const int WORD_COUNT = 25;
    char common_words[WORD_COUNT][10] = {
        "the","and","that","have","for","not","with","you",
        "this","but","his","from","they","we","say","her",
        "she","will","one","all","would","there","their","what","about"
    };

    int valid_a[] = {1,3,5,7,9,11,15,17,19,21,23,25};

    char best_plain[1000] = "";
    int best_a = 0, best_b = 0;
    int max_score = 0;

    for (int i = 0; i < 12; i++) {
        int a = valid_a[i];
        for (int b = 0; b < 26; b++) {
            char decrypted[1000];
            decrypt_affine(cipher, a, b, decrypted);

            int current_score = 0;
            for (int w = 0; w < WORD_COUNT; w++) {
                if (strstr(decrypted, common_words[w]) != NULL)
                    current_score += strlen(common_words[w]);
            }

            if (current_score > max_score) {
                max_score = current_score;
                best_a = a;
                best_b = b;
                strcpy(best_plain, decrypted);
            }
        }
    }

    if (max_score > 0) {
        cout << "\n[SUCCESS] Best Match Found!" << endl;
        cout << "Encryption Keys -> a: " << best_a << ", b: " << best_b << endl;
        cout << "Decryption Keys -> a^-1: " << modInverse(best_a) << ", b: " << best_b << endl;
        cout << "Decrypted Text: " << best_plain << endl;
    } else {
        cout << "\n[FAILED] Could not decrypt." << endl;
    }
}

int main() {
    char cipher[1000];
    cout << "Enter the Encrypted text (Ciphertext): ";
    cin.getline(cipher, 1000);
    affine_auto_attack(cipher);
    return 0;
}
