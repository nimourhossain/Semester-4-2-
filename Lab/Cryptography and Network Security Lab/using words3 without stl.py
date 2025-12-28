import math
import string

# ---------- GCD ----------
def gcd(a, b):
    return math.gcd(a, b)

# ---------- Modular Inverse ----------
def mod_inverse(k, mod=26):
    try:
        return pow(k, -1, mod)
    except ValueError:
        return -1

# ---------- Encryption ----------
def encrypt(plaintext, key):
    cipher = ""
    for ch in plaintext:
        if ch.isalpha():
            p = ord(ch.lower()) - ord('a')
            c = (p * key) % 26
            cipher += chr(c + ord('a'))
        else:
            cipher += ch
    return cipher

# ---------- Decryption ----------
def decrypt(cipher, key):
    inv = mod_inverse(key)
    if inv == -1:
        return "Invalid Key"

    plain = ""
    for ch in cipher:
        if ch.isalpha():
            c = ord(ch.lower()) - ord('a')
            p = (c * inv) % 26
            plain += chr(p + ord('a'))
        else:
            plain += ch
    return plain

# ---------- Top Frequent Characters ----------
def get_top_frequent_chars(text, n):
    freq = [0] * 26
    for ch in text:
        if ch.isalpha():
            freq[ord(ch.lower()) - ord('a')] += 1

    result = []
    for _ in range(n):
        max_idx = max(range(26), key=lambda i: freq[i])
        if freq[max_idx] == 0:
            break
        result.append(chr(max_idx + ord('a')))
        freq[max_idx] = 0
    return result

# ---------- Statistical Attack ----------
def statistical_attack(cipher, sample_text):
    print("\n--- Statistical Attack (Automated Mode) ---")

    common_words = [
        "the","be","to","of","and","a","in","that","have","i",
        "it","for","not","on","with","he","as","you","do","at"
    ]

    top_cipher = get_top_frequent_chars(cipher, 3)
    top_sample = get_top_frequent_chars(sample_text, 3)

    print("Top Cipher Chars:", top_cipher)
    print("Top Sample Chars:", top_sample)

    print("Analyzing hypotheses and checking dictionary...")

    for c_char in top_cipher:
        for p_char in top_sample:

            c_val = ord(c_char) - ord('a')
            p_val = ord(p_char) - ord('a')

            if p_val == 0:
                continue

            for k in range(1, 26):
                if gcd(k, 26) == 1 and (p_val * k) % 26 == c_val:
                    decrypted = decrypt(cipher, k)

                    match_count = sum(
                        1 for w in common_words if w in decrypted
                    )

                    if match_count > 0:
                        print("\n[SUCCESS] Valid English Text Found!")
                        print(f"Hypothesis: Cipher '{c_char}' -> Sample '{p_char}'")
                        print(f"Encryption Key (K): {k}")
                        print(f"Decryption Key (Inverse): {mod_inverse(k)}")
                        print("Decrypted Text:", decrypted)
                        return

    print("\n[FAILED] Could not automatically determine the correct key.")

# ---------- Main ----------
def main():
    plaintext = input("Enter plaintext: ")

    key = int(input("Enter Encryption key (must be coprime to 26): "))
    if gcd(key, 26) != 1:
        print("Invalid Key! GCD(key, 26) must be 1.")
        return

    print("\n--- For Statistical Attack ---")
    sample_text = input("Enter a sample text: ")
    if len(sample_text.strip()) == 0:
        sample_text = "eeee tttt aaaa oooo iiii"

    cipher = encrypt(plaintext, key)
    print("\nEncrypted text:", cipher)

    statistical_attack(cipher, sample_text)


if __name__ == "__main__":
    main()
