# AES-128 FOCPA

- CTF-1-AES-trace.bin : AES-128 암호화 과정의 소비 전력 신호.   
- CTF-1-AES-plain.bin : AES-128의 평문.   

**Answer**   
Master Key: 0x47 0x4F 0x30 0x64 0x4C 0x75 0x63 0x6B 0x40 0x53 0x63 0x41 0x2D 0x43 0x54 0x46    
Change the master key to ASCII code. → https://www.rapidtables.com/convert/number/hex-to-ascii.html      

### Reference
NIST, FIPS-192: ADVANCED ENCRYPTION STANDARD (AES).   
Eric Brier et al., Correlation Power Analysis with a Leakage Model.   
Plaintext, Trace information source is "Side Channel Attack Contest 2022" → https://scacontest.com/ 
