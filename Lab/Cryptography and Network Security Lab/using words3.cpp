#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverse(int k) {
    for (int i = 1; i < 26; ++i) {
        if ((k * i) % 26 == 1) {
            return i;
        }
    }
    return -1;
}

string encrypt(string plaintext, int key) {
    string cipher = "";
    for (char c : plaintext) {
        if (isalpha(c)) {
            int p = tolower(c) - 'a';
            int c_val = (p * key) % 26;
            cipher += (char)(c_val + 'a');
        } else {
            cipher += c;
        }
    }
    return cipher;
}

string decrypt(string cipher, int key) {
    int inv = modInverse(key);
    if (inv == -1) return "Invalid Key";
    string plain = "";
    for (char c : cipher) {
        if (isalpha(c)) {
            int c_val = tolower(c) - 'a';
            int p = (c_val * inv) % 26;
            plain += (char)(p + 'a');
        } else {
            plain += c;
        }
    }
    return plain;
}

vector<pair<char, int>> getTopFrequentChars(string text, int n) {
    map<char, int> counts;
    for (char c : text) {
        if (isalpha(c)) {
            counts[tolower(c)]++;
        }
    }
    vector<pair<char, int>> sorted_counts;
    for (auto const& pair : counts) {
        sorted_counts.push_back({pair.first, pair.second});
    }
    sort(sorted_counts.begin(), sorted_counts.end(),
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second;
         });
    if (sorted_counts.size() > n) {
        sorted_counts.resize(n);
    }
    return sorted_counts;
}

void statistical_attack_interactive(string cipher, string sample_text) {
    cout << "\n--- Statistical Attack (Automated Mode) ---" << endl;
    vector<string> common_words = {
        "the", "be", "to", "of", "and", "a", "in", "that", "have", "i",
        "it", "for", "not", "on", "with", "he", "as", "you", "do", "at"
    };
    vector<pair<char, int>> top_cipher = getTopFrequentChars(cipher, 3);
    if (top_cipher.empty()) return;
    vector<pair<char, int>> top_sample = getTopFrequentChars(sample_text, 3);
    if (top_sample.empty()) return;
    cout << "Top Cipher Chars: [";
    for (size_t i = 0; i < top_cipher.size(); ++i) {
        cout << "'" << top_cipher[i].first << "'";
        if (i < top_cipher.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "Top Sample Chars: [";
    for (size_t i = 0; i < top_sample.size(); ++i) {
        cout << "'" << top_sample[i].first << "'";
        if (i < top_sample.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "Analyzing hypotheses and checking dictionary..." << endl;
    for (auto c_item : top_cipher) {
        for (auto s_item : top_sample) {
            char cipher_char = c_item.first;
            char sample_char = s_item.first;
            int c_val = cipher_char - 'a';
            int p_val = sample_char - 'a';
            if (p_val == 0) continue;
            int found_k = -1;
            for (int k = 1; k < 26; ++k) {
                if (gcd(k, 26) == 1) {
                    if ((p_val * k) % 26 == c_val) {
                        found_k = k;
                        break;
                    }
                }
            }
            if (found_k != -1) {
                int decryption_key = modInverse(found_k);
                string decrypted_preview = decrypt(cipher, found_k);
                int match_count = 0;
                for (const string& word : common_words) {
                    if (decrypted_preview.find(word) != string::npos) {
                        match_count++;
                    }
                }
                if (match_count > 0) {
                    cout << "\n[SUCCESS] Valid English Text Found!" << endl;
                    cout << "Hypothesis: Cipher '" << cipher_char << "' -> Sample '" << sample_char << "'" << endl;
                    cout << "  -> Encryption Key (K): " << found_k << endl;
                    cout << "  -> Decryption Key (Inverse): " << decryption_key << endl;
                    cout << "  -> Decrypted Text: " << decrypted_preview << endl;
                    return;
                }
            }
        }
    }
    cout << "\n[FAILED] Could not automatically determine the correct key based on the top 20 words." << endl;
}

int main() {
    string plaintext, sample_text;
    int key;
    cout << "Enter plaintext: ";
    getline(cin, plaintext);
    cout << "Enter Encryption key (must be coprime to 26): ";
    if (!(cin >> key)) {
        cout << "Key must be an integer." << endl;
        return 0;
    }
    if (gcd(key, 26) != 1) {
        cout << "Invalid Key! GCD(key, 26) must be 1." << endl;
        return 0;
    }
    cin.ignore();
    cout << "\n--- For Statistical Attack ---" << endl;
    cout << "Enter a sample text (used to calculate frequency):" << endl;
    getline(cin, sample_text);
    if (sample_text.length() < 1) {
        sample_text = "eeee tttt aaaa oooo iiii";
    }
    string cipher = encrypt(plaintext, key);
    cout << "\nEncrypted text: " << cipher << endl;
    statistical_attack_interactive(cipher, sample_text);
    return 0;
}
