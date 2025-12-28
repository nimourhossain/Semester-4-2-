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

    // Map থেকে Vector এ কনভার্ট করা হচ্ছে (Compatible version)
    vector<pair<char, int>> sorted_counts;
    // আগের [key, val] এর বদলে pair ব্যবহার করা হয়েছে যাতে সব কম্পাইলারে চলে
    for (auto const& pair : counts) {
        sorted_counts.push_back({pair.first, pair.second});
    }

    // ফ্রিকোয়েন্সি অনুযায়ী Descending Sort
    sort(sorted_counts.begin(), sorted_counts.end(),
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second;
         });

    // Top N রিটার্ন করা
    if (sorted_counts.size() > n) {
        sorted_counts.resize(n);
    }
    return sorted_counts;
}

// 5. Statistical Attack (With Human Verification)
void statistical_attack_interactive(string cipher, string sample_text) {
    cout << "\n--- Statistical Attack (Interactive Mode) ---" << endl;

    // সাইফারের ফ্রিকোয়েন্সি (Top 3)
    vector<pair<char, int>> top_cipher = getTopFrequentChars(cipher, 3);
    if (top_cipher.empty()) return;

    // স্যাম্পলের ফ্রিকোয়েন্সি (Top 3)
    vector<pair<char, int>> top_sample = getTopFrequentChars(sample_text, 3);
    if (top_sample.empty()) return;

    // প্রিন্ট করা
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

    cout << "Checking hypotheses... (Type 'y' if the text makes sense, 'n' to continue)\n" << endl;

    // ৩x৩ লুপ চালানো
    for (auto c_item : top_cipher) {
        for (auto s_item : top_sample) {

            char cipher_char = c_item.first;
            char sample_char = s_item.first;

            int c_val = cipher_char - 'a';
            int p_val = sample_char - 'a';

            // Plaintext 'a' (value 0) হলে স্কিপ
            if (p_val == 0) {
                continue;
            }

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
                int decryption_key = modInverse(found_k);
                string decrypted_preview = decrypt(cipher, found_k);

                cout << "Hypothesis: Cipher '" << cipher_char << "' -> Sample '" << sample_char << "'" << endl;
                cout << "  -> Encryption Key (K): " << found_k << endl;
                cout << "  -> Decryption Key (Inverse): " << decryption_key << endl;
                cout << "  -> Decrypted Text: " << decrypted_preview << endl;

                cout << "  >>> Is this the correct text? (y/n): ";
                char user_check;
                cin >> user_check;

                if (tolower(user_check) == 'y') {
                    cout << "\n[SUCCESS] Attack Successful! Keys confirmed." << endl;
                    return; // ফাংশন থেকে বের হয়ে যাবে
                } else {
                    cout << "  -> Continuing search...\n" << endl;
                }
            }
        }
    }
    cout << "\n[FAILED] No suitable key accepted by user in top 3x3 combinations." << endl;
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

    // বাফার ক্লিয়ার করা (cin >> key এর পর নিউলাইন থেকে যায়)
    cin.ignore();

    cout << "\n--- For Statistical Attack ---" << endl;
    cout << "Enter a sample text:" << endl;
    getline(cin, sample_text);

    if (sample_text.length() < 1) {
        sample_text = "eeee tttt aaaa oooo iiii";
    }

    string cipher = encrypt(plaintext, key);
    cout << "\nEncrypted text: " << cipher << endl;

    statistical_attack_interactive(cipher, sample_text);

    return 0;
}
