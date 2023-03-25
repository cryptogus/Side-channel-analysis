# FIND ARIA128 MASTER KEY

## Download binary file
```bash
$ curl -c ./cookie -s -L "https://drive.google.com/file/d/1DHgmrA4_5ht7KMVQPrshUgHIh-1uo-r6/view?usp=share_link" > /dev/null
$ curl -Lb ./cookie "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' ./cookie`&id=1DHgmrA4_5ht7KMVQPrshUgHIh-1uo-r6" -o CTF-4-ARIA-trace.bin
$ du -B MB CTF-4-ARIA-trace.bin 
24MB    CTF-4-ARIA-trace.bin
```
## Transform binary files to trace files
```bash
$ python3 bin2trace.py
```
## 1라운드에서 주어지지 않은 키를 찾는 Idea
```
행렬 연산에서 1라운드 키 index를 0부터 시작했을때 1,6, 11, 12 제공 X

s0 = p3 ^ p4 ^ p6 ^ p8 ^ p9 ^ p13 ^ p14
s1 = p2 ^ p5 ^ p7 ^ p8 ^ p9 ^ p12 ^ p15
s2 = p1 ^ p4 ^ p6 ^p10 ^ p11 ^p12 ^p15
s3 = p0 ^ p5 ^ p7 ^ p10 ^p11 ^p13 ^p14
s4 = p0 ^ p2 ^ p5 ^ p8 ^ p11 ^p14 ^ p15
s5 = p1 ^ p3 ^ p4 ^ p9 ^ p10 ^p14 ^ p15
s6 = p0 ^ p2 ^ p7 ^ p9 ^ p10 ^p12 ^ p13
s7 = p1 ^ p3 ^ p6 ^ p8 ^ p11 ^p12 ^ p13
s8 = p0 ^ p1 ^ p4 ^ p7 ^ p10 ^p13 ^ p15
s9 = p0 ^ p1 ^ p5 ^ p6 ^ p11 ^p12 ^ p14
s10 = p2 ^p3 ^ p5 ^ p6 ^ p8 ^ p13 ^ p15
s11 = p2 ^p3 ^ p4 ^ p7 ^ p9 ^ p12 ^ p14
s12 = p1 ^p2 ^ p6 ^ p7 ^ p9 ^ p11 ^ p12
s13 = p0 ^p3 ^ p6 ^ p7 ^ p8 ^ p10 ^ p13
s14 = p0 ^p3 ^ p4 ^ p5 ^ p9 ^ p11 ^ p14
s15 = p1 ^p2 ^ p4 ^ p5 ^ p8 ^ p10 ^ p15
```
## Guess ARIA round key using FOCPA
```bash
$ cd FOCPA_C
# using c source code!
```
you can find FOCPA result in 5 directories

## Master Key 추출

```bash
$ python3 MasterKeyRecovery1.py
$ gcc MasterKeyRecovery2.c
# W1[16] array is result from MasterKeyRecovery1.py in MasterKeyRecovery2.c
```
```c
////From MasterKeyRecovery1.py
unsigned char W1[16] = {    0b00100111
							,0b00010111
							,0b10000111
							,0b01001001
							,0b11111000
							,0b11110001
							,0b11101110
							,0b10110100
							,0b10111001
							,0b01100101
							,0b10100011
							,0b01010101
							,0b00110111
							,0b00100100
							,0b00000101
							,0b11100010 };
```
