# LEA
LEA is lightweight block cipher uses any one size of key out of three key size of 128-bit, 192-bit and 256-bit based on different level of security. LEA block size is of 128-bit.

**Guess the master ky**
## Download binary file

https://drive.google.com/file/d/1-YNN_7ykPIU5GdX5Gtm3II8HstWKcWcj/view?usp=share_link


```bash
$ cd LEA128
$ curl -c ./cookie -s -L https://drive.google.com/file/d/1-YNN_7ykPIU5GdX5Gtm3II8HstWKcWcj/view?usp=share_link > /dev/null
$ curl -Lb ./cookie "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' ./cookie`&id=1-YNN_7ykPIU5GdX5Gtm3II8HstWKcWcj" -o CTF-3-LEA-trace.bin
$ du -B MB CTF-3-LEA-trace.bin 
322M      CTF-3-LEA-trace.bin
```