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

# Code 해석

First Order Correlation Power Analysis(FOCPA)는 8-bit 연산 기준으로(꼭 8-bit일 필요는 없음. 타겟 보드가 32-bit 프로세서일 수도 있는데, LEA의 설계가 32-bit 단위로 연산하도록 되어있어서, 32-bit 단위 연산으로 짠 LEA SW 코드를 8-bit 보드를 이용해 분석을 시도하면, 분석이 잘 안될 수 있음) 특정 연산의 결과 값의 Hamming Weight(or Distance)와 해당 연산을 수행하며 방출되는 전력 소비량이 비례한다는 특징을 이용한다.

![image](https://github.com/user-attachments/assets/b3417e3e-99d0-47f5-b1ac-66aced1a0adc)  
그림출처 - https://onlinelibrary.wiley.com/doi/10.1155/2022/7375097

그림에서 보이다시피 HM(Hamming) Weight 값들에 따라 전압의 크기의 관계가 명확하게 칼로 자르듯이 구분되지는 않는다. 하지만 어느정도 비례함은 알수가 있다.  

이 특징을 이용한다면 평문(또는 암호문, 중간 연산값 등)과 해당 평문으로 암호화시 사용되는 전력 소비량 정보가 있다고 가정할 때, Pearson Correlation Coefficient 공식을 이용하여 전력 소비량과 추측 값들의 Hamming Weight들 사이의 상관계수를 구하고, 그 중 가장 높은 상관계수를 가지는 값을 특정 연산의 결과 값을 추측하는 방법이다.

하지만 실제 전력 소비량 정보를 얻었을 때, 어떤 위치의 소비량이 특정 연산의 결과인지 알 수가 없다.
