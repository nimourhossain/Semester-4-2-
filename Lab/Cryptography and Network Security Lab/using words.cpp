#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cctype>

using namespace std;

// 1. GCD Function
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 2. Modular Inverse Function
int modInverse(int k) {
    for (int i = 1; i < 26; ++i) {
        if ((k * i) % 26 == 1) {
            return i;
        }
    }
    return -1;
}

// 3. Encryption Function
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

// 4. Decryption Function
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

// Helper function to get Top N frequent characters
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

// 5. Statistical Attack (Automated Logic + Old Print Style)
void statistical_attack_automated(string cipher, string sample_text) {
    cout << "\n--- Statistical Attack ---" << endl;

    // Top 20 Most Common English Words (Used for internal checking)
    vector<string> common_words = {
        "the", "be", "to", "of", "and", "a", "in", "that", "have", "it",
        "for", "not", "on", "with", "he", "as", "you", "do", "at", "this"
    };

    // Get frequencies
    vector<pair<char, int>> top_cipher = getTopFrequentChars(cipher, 3);
    vector<pair<char, int>> top_sample = getTopFrequentChars(sample_text, 3);

    if (top_cipher.empty() || top_sample.empty()) return;

    // --- আগের স্টাইলে ফ্রিকোয়েন্সি প্রিন্ট করা ---
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

    cout << "\nAnalyzing and decrypting..." << endl;

    // বেস্ট রেজাল্ট স্টোর করার ভেরিয়েবল
    int best_k = -1;
    int best_inv = -1;
    string best_plaintext = "";
    int max_word_matches = -1;

    // সব কম্বিনেশন চেক করা
    for (auto c_item : top_cipher) {
        for (auto s_item : top_sample) {

            char cipher_char = c_item.first;
            char sample_char = s_item.first;

            int c_val = cipher_char - 'a';
            int p_val = sample_char - 'a';

            if (p_val == 0) continue; // Skip if plain char is 'a' (0)

            // K খোঁজা
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
                // Decrypt using this hypothesis
                string candidate_text = decrypt(cipher, found_k);
                int inv_key = modInverse(found_k);

                // স্কোর চেক করা (কয়টি কমন ওয়ার্ড আছে)
                int current_matches = 0;
                for (string word : common_words) {
                    if (candidate_text.find(word) != string::npos) {
                        current_matches++;
                    }
                }

                // যদি বর্তমান টেক্সট আগের চেয়ে ভালো হয়, তবে আপডেট করা হবে
                if (current_matches > max_word_matches) {
                    max_word_matches = current_matches;
                    best_k = found_k;
                    best_inv = inv_key;
                    best_plaintext = candidate_text;
                }
            }
        }
    }

    // --- ফাইনাল রেজাল্ট প্রিন্ট (আগের ফরম্যাটে) ---
    if (best_k != -1) {
        cout << "\n[RESULT FOUND]" << endl;
        cout << " -> Encryption Key (K): " << best_k << endl;
        cout << " -> Decryption Key (Inverse): " << best_inv << endl;
        cout << " -> Decrypted Text: " << best_plaintext << endl;
    } else {
        cout << "\n[FAILED] Could not find a matching key automatically." << endl;
    }
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

    cin.ignore(); // Clear buffer

    cout << "\n--- For Statistical Attack ---" << endl;
    cout << "Enter a sample text (used to calculate English frequency):" << endl;
    getline(cin, sample_text);

    if (sample_text.length() < 1) {
        // Default sample text
        sample_text = "the quick brown fox jumps over the lazy dog and that is very common";
    }

    string cipher = encrypt(plaintext, key);
    cout << "\nEncrypted text: " << cipher << endl;

    statistical_attack_automated(cipher, sample_text);

    return 0;
}
