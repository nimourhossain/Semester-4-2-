import sys
from collections import Counter

# 1. GCD Function
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

# 2. Modular Inverse Function (Decryption Key বের করার জন্য)
def mod_inverse(k):
    for i in range(1, 26):
        if (k * i) % 26 == 1:
            return i
    return -1

# 3. Encryption Function
def encrypt(plaintext, key):
    cipher = ""
    for c in plaintext:
        if c.isalpha():
            p = ord(c.lower()) - ord('a')
            c_val = (p * key) % 26
            cipher += chr(c_val + ord('a'))
        else:
            cipher += c
    return cipher

# 4. Decryption Function
def decrypt(cipher, key):
    inv = mod_inverse(key)
    if inv == -1: return "Invalid Key"
    
    plain = ""
    for c in cipher:
        if c.isalpha():
            c_val = ord(c.lower()) - ord('a')
            p = (c_val * inv) % 26
            plain += chr(p + ord('a'))
        else:
            plain += c
    return plain

# 5. Statistical Attack (With Human Verification & Decryption Key Display)
def statistical_attack_interactive(cipher, sample_text):
    print("\n--- Statistical Attack (Interactive Mode) ---")
    
    # সাইফারের ফ্রিকোয়েন্সি (Top 3)
    cipher_chars = [c.lower() for c in cipher if c.isalpha()]
    if not cipher_chars: return
    top_cipher = Counter(cipher_chars).most_common(3)
    
    # স্যাম্পলের ফ্রিকোয়েন্সি (Top 3)
    sample_chars = [c.lower() for c in sample_text if c.isalpha()]
    if not sample_chars: return
    top_sample = Counter(sample_chars).most_common(3)

    print(f"Top Cipher Chars: {[x[0] for x in top_cipher]}")
    print(f"Top Sample Chars: {[x[0] for x in top_sample]}")
    print("Checking hypotheses... (Type 'y' if the text makes sense, 'n' to continue)\n")

    # ৩x৩ লুপ চালানো
    for c_item in top_cipher:
        for s_item in top_sample:
            
            cipher_char = c_item[0]
            sample_char = s_item[0]
            
            c_val = ord(cipher_char) - ord('a')
            p_val = ord(sample_char) - ord('a')

            # Plaintext 'a' (value 0) হলে স্কিপ
            if p_val == 0:
                continue

            # K খোঁজা
            found_k = -1
            for k in range(1, 26):
                if gcd(k, 26) == 1:
                    if (p_val * k) % 26 == c_val:
                        found_k = k
                        break 
            
            if found_k != -1:
                # ডিক্রিপশন কী (Inverse Key) বের করা হচ্ছে
                decryption_key = mod_inverse(found_k) 
                decrypted_preview = decrypt(cipher, found_k)
                
                print(f"Hypothesis: Cipher '{cipher_char}' -> Sample '{sample_char}'")
                print(f"  -> Encryption Key (K): {found_k}")
                print(f"  -> Decryption Key (Inverse): {decryption_key}") # এখানে শো করা হচ্ছে
                print(f"  -> Decrypted Text: {decrypted_preview}")
                
                # এখানে ইউজার ইনপুট নেওয়া হবে
                user_check = input("  >>> Is this the correct text? (y/n): ").lower()
                
                if user_check == 'y':
                    print("\n[SUCCESS] Attack Successful! Keys confirmed.")
                    return 
                else:
                    print("  -> Continuing search...\n")

    print("\n[FAILED] No suitable key accepted by user in top 3x3 combinations.")

def main():
    plaintext = input("Enter plaintext: ")
    try:
        key = int(input("Enter Encryption key (must be coprime to 26): "))
    except ValueError: return

    if gcd(key, 26) != 1:
        print("Invalid Key!")
        return

    print("\n--- For Statistical Attack ---")
    print("Enter a sample text:")
    sample_text = input()
    if len(sample_text) < 1: sample_text = "eeee tttt aaaa oooo iiii"

    cipher = encrypt(plaintext, key)
    print(f"\nEncrypted text: {cipher}")
    
    # ইন্টারেক্টিভ ফাংশন কল
    statistical_attack_interactive(cipher, sample_text)

if __name__ == "__main__":
    main()