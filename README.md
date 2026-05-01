This mini-program was developed as a small project for the course nt101 - VNUHCM UIT
It's used to demonstrate the way of using these 2 encryption algorithms: Playfair (classic) and RSA (morden).
It has all the necessary functions for the Playfair (encryption, decryption) and RSA (encryption, decryption, sign a digital signature using built-in sha256 hashing, verify a signature).

Other than that, it has a useful feature: Checksum checker
You can choose a file and calculate its checksums (with the common hashes like md5, sha1, sha256, sha512). You can also compare the expected original checksum and the one that is calculated by the program to know if your file has been modified or not.
Authors: Pham Duc Long - UI/UX Design & Checksum Feature Implementation.
         Ngo Nhat Linh: Core Logic for Playfair & RSA Algorithms.
         
![[./checksum.png]]
