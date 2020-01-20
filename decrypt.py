import sys
import time
from Crypto.Cipher import AES


BLOC = 16

buf = open(sys.argv[1], "rb").read()
key = buf[:BLOC]
iv = buf[BLOC:BLOC*2]
data = buf[BLOC*2:]

pad = 0
if (len(data) % BLOC) > 0:
    # Decrypt the second-to-last ciphertext block using ECB mode
    pad = BLOC - (len(data) % BLOC)
    secondToLast = data[len(data)-2*BLOC+pad:len(data)-BLOC+pad]
    dec = AES.new(key, AES.MODE_ECB).decrypt(secondToLast)

    # Pad the ciphertext to the nearest multiple of the block size 
    # using the last B−M bits of block cipher decryption 
    # of the second-to-last ciphertext block
    data += bytes(dec[len(dec)-pad:len(dec)])

    # Swap the last two ciphertext blocks
    data = data[:len(data)-2*BLOC] + data[len(data)-BLOC:] + data[len(data)-2*BLOC:len(data)-BLOC]

# Decrypt the (modified) ciphertext using the standard CBC mode
index = 0
decd = b''
cipher = AES.new(key, AES.MODE_CBC, iv)
while index < len(data):
    decd += cipher.decrypt(data[index:index+BLOC])
    index += BLOC

# Truncate the plaintext to the length of the original ciphertext
if pad != 0:
    decd = decd[:len(decd)-pad]

out = open(sys.argv[2], "wb")
out.write(decd)
out.close()
