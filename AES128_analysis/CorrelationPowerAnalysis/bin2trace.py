'''
    2022 부채널 분석 경진대회
            2022.08.01
'''
import struct
import sys
#bin 파일 종류에 따라 코드 수정 필요

binFilePath = "."

binFileName1 = "CTF-1-AES-plain"
binFileName2 = "CTF-1-AES-trace"

FileName1 = "plaintext"
FileName2 = "AES128"


# bin파일 -> txt파일로 바꾸기(1개의 파형개형확인을 위함)
plain  = open(binFilePath + '/' + binFileName1+'.bin', 'rb')
trace  = open(binFilePath + '/' + binFileName2+'.bin','rb')
fp1    = open(binFilePath + '/' + FileName1 + '.txt', 'w')
fp2    = open(binFilePath + '/' + FileName2 + '.txt', 'w')

#파형정보
trace_num = int.from_bytes(trace.read(4), "little")
point_num = int.from_bytes(trace.read(4), "little")
print(trace_num)
print(point_num)
#################################################################
trace.seek(16+point_num*0)#16+point_num*4*i (i는 0~파형갯수-1), 4는 한 포인트의 자료형 크기
for i in range(point_num):
    
    #unpack 자료형 - https://docs.python.org/ko/3/library/struct.html#struct.calcsize
    X = struct.unpack('f', trace.read(4))[0] #tuple 형태, ex) 'f'면 unpack('f', trace.read(4))[1], HxD 등으로 파일 구성(binary) 확인해보기 - 경진대회 문제에 파형의 한 포인트가 8바이트 duble형도 있음

    fp2.write(str(X) + '\n')
fp2.close()
# trace
trace.seek(16) #파형정보 부터 start

fp     = open(binFilePath + '/' + FileName2 + '.trace', 'wb')
fp.write(FileName2[:20].ljust(20).encode('ascii')) #파일 이름 기록에 20byte 사용
fp.write(struct.pack('i', trace_num)) #4byte
fp.write(struct.pack('i', point_num)) #4byte
fp.write('END!'.encode('ascii')) #4byte 사용

#모든 파형의 포인트 정보 기록
for j in range(trace_num*point_num):
    
    fp.write(trace.read(4))
fp.close()
trace.close()
#plaintext
################################################################
plain.seek(16)
for i in range(trace_num*16): #16 = block size (bytes)
    
    #unpack 자료형 - https://docs.python.org/ko/3/library/struct.html#struct.calcsize
    #print(sys.getsizeof(plain.read(1)))
    X = struct.unpack('B', plain.read(1))[0] 
    X = hex(X)
    fp1.write(str(X)[2:])
    
    if (i + 1) %16 != 0:
        fp1.write(' ')
    if (i + 1)% 16 == 0:
        fp1.write('\n')
fp1.close()
plain.close()
