# Simple Power Analysis
Guess the key through the shape of the Trace of ECDSA.
## Download binary file
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

## Parsing the Point of the Trace of ECDSA
```
$ python3 b2tECDSA.py

$ pip install --upgrade pip
$ pip install bokeh
$ pip install numpy

$ python3 Trace_plot.py
$ cd html_plot
```
If you use Jupyter botebook (or google colab), you can use `output_notebook` function in Trace_plot to see plot directly
About `output_<>`   
See bokeh.io   
### Troubleshooting
If there is a problem with the package, it can be solved as follows. (maybe)
```
$ pip install --upgrade numpy
$ pip install --upgrade bokeh
```

## Note
The jpg files in the "8. ECDSA" folder are made using MATLAB.