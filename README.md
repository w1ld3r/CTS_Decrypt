# CTS Decrypt
Implementation in Python 3 and C for CBC ciphertext stealing decryption using a standard CBC interface (Ciphertext stealing - CTS)

## File structure
To decrypt a file encrypted in CTS Mode, the struct of the encrypted file should be like this:
MD5 Key (16 bytes) - IV (16 bytes) - Encrypted Message

## Python
### Prerequisites
Install python3 and pip:
```
sudo apt install python3 python3-pip
```
Install PyCryptodome 
```
pip3 install pycryptodome
```
### Usage
Execute:
```
python3 decrypt.py encrypted.bin decrypted.dec
```
## C
### Prerequisites
Install gcc:
```
sudo apt install gcc
```
Install OpenSSL library
```
sudo apt install libssl–dev
```
### Usage
Compile:
```
gcc -o decrypt decrypt.c -lcrypto
```
Execute:
```
./decrypt encrypted.bin decrypted.dec
```

## Test
- Encrypt lenna.png with CIPHERME.exe
- Use decrypt.py or decrypt.c to decrypt the encrypted file

## License
This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details.
