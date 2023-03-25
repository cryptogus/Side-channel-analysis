# FIND ARIA128 MASTER KEY

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