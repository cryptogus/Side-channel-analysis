# Simple Power Analysis

### Download binary file
From https://drive.google.com/file/d/1jU5HO0sUBX5rWjupBX3fuekxU9CvLJ13/view?usp=share_link   

### Download in Ubuntu
```
$ cd ECDSA_SPA
$ curl -c ./cookie -s -L "https://drive.google.com/uc?export=download&id=1jU5HO0sUBX5rWjupBX3fuekxU9CvLJ13" > /dev/null
$ curl -Lb ./cookie "https://drive.google.com/uc?export=download&confirm=`awk '/download/ {print $NF}' ./cookie`&id=1jU5HO0sUBX5rWjupBX3fuekxU9CvLJ13" -o CTF-8-ECDSA-SPA-trace.bin

# confirm binary file size
$ du -h CTF-8-ECDSA-SPA-trace.bin 
479M    CTF-8-ECDSA-SPA-trace.bin
```