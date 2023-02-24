'''
    2022 부채널 분석 경진대회
            2022.08.01
국민대학교 정보보안암호수학과 Lee Hyun-Ho
'''
import struct
import os

# get file path
binFilePath = '.'

binFileName = 'CTF-8-ECDSA-SPA-trace'

# bin파일 -> txt파일로 바꾸기(1개의 파형개형확인을 위함)
trace  = open(binFilePath + '/' + binFileName + '.bin','rb')

#파형정보
trace_num = int.from_bytes(trace.read(4), "little")
point_num = int.from_bytes(trace.read(4), "little")
print('trace num:',trace_num)
print('point num:',point_num)
'''
f = open(binFilePath + '/' + 'ECDSA.txt', 'w')
trace.seek(16)
for i in range(point_num):
    X = struct.unpack('f', trace.read(4))[0]
    f.write(str(X) + '\n')
f.close()

print("finish")
'''
# 파형 포인트가 너무 크니 1개의 파형을 2000000포인트씩 63개로 나눔
for j in range(63):
   
    fp     = open(binFilePath + '/63part/' + 'ECDSA' + str(j+1) + '.txt', 'w')
    trace.seek(16+2000000*j*4)#16+point_num*4*i (i는 0~파형갯수-1)
    
    for i in range(2000000):
        #unpack 자료형 - https://docs.python.org/ko/3/library/struct.html#struct.calcsize
        X = struct.unpack('f', trace.read(4))[0] #tuple 형태, ex) 'f'면 unpack('f', trace.read(4))[1], HxD 등으로 파일 구성(binary) 확인해보기 - 경진대회 문제는 파형의 한 포인트가 8바이트 duble형이였음
        #print(trace.tell())    
        fp.write(str(X) + '\n')
    fp.close()
