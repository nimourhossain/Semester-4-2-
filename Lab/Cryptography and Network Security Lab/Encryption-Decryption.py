pltx = "iambadboycode"
k = 12
enc = ""

# ---------- Encryption ----------
for i in pltx:
    singleChar = ord(i) - ord('a')
    singleChar += k
    singleChar = singleChar % 26
    singleChar += ord('a')
    enc += chr(singleChar)

print("Encrypted text:", enc)



sampleArray = ['me','very','good','table','box','not','possible','well','book','room']

found_k = None

for kx in range(26):  
    decoded = ""

    for i in enc:
        singledecode = ord(i) - ord('a')
        singledecode -= kx         
        singledecode = singledecode % 26
        singledecode += ord('a')
        decoded += chr(singledecode)

    
    for word in sampleArray:
        if word in decoded:
            found_k = kx
            print("Decoded text:", decoded)
            break

    if found_k is not None:
        break

print(f"Found key = {found_k}")