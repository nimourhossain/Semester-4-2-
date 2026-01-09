#include <iostream>
#include <cstring>
#include <cctype>
#include <vector>

using namespace std;

void decrypt_transposition_pptx(char cipher[], int cols, char plain[]) {
    int len = strlen(cipher);
    int rows = len / cols;
    
    char grid[rows][cols];
    int k = 0;

    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            grid[r][c] = cipher[k++];
        }
    }

    k = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            plain[k++] = grid[r][c];
        }
    }
    plain[k] = '\0';
}

void transposition_attack_optimized(char cipher[]) {
    cout << "\n--- Transposition Cipher Attack (Based on PPTX Method 2) ---" << endl;
    
    const int WORD_COUNT = 25;
    char common_words[WORD_COUNT][10] = {
        "the","and","that","have","for","not","with","you",
        "this","but","his","from","they","we","say","her",
        "she","will","one","all","would","there","their","what","about"
    };

    int len = strlen(cipher);
    char best_plain[1000] = "";
    int best_cols = 0;
    int max_score = 0;

    cout << "Cipher Length (L): " << len << endl;
    cout << "Checking keys that divide L..." << endl;

    for (int cols = 2; cols < len; cols++) {
        if (len % cols != 0) continue;

        char decrypted[1000];
        decrypt_transposition_pptx(cipher, cols, decrypted);

        int current_score = 0;
        for (int w = 0; w < WORD_COUNT; w++) {
            if (strstr(decrypted, common_words[w]) != NULL)
                current_score += strlen(common_words[w]);
        }

        if (current_score > max_score) {
            max_score = current_score;
            best_cols = cols;
            strcpy(best_plain, decrypted);
        }
    }

    if (max_score > 0) {
        cout << "\n[SUCCESS] Best Match Found!" << endl;
        cout << "------------------------------------------------" << endl;
        cout << "Key Found (Number of Columns): " << best_cols << endl;
        cout << "------------------------------------------------" << endl;
        cout << "Decrypted Text: " << best_plain << endl;
    } else {
        cout << "\n[FAILED] Could not decrypt." << endl;
    }
}

int main() {
    char cipher[1000];
    cout << "Enter the Encrypted text (Ciphertext): ";
    cin.getline(cipher, 1000);
    transposition_attack_optimized(cipher);
    return 0;
}
