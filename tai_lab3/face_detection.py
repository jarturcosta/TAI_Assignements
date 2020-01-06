import sys
import os
import bz2
import io
import csv
import operator
import lzma
import gzip
from PIL import Image  
import PIL  


def reduce_res(filename):
    img = Image.open(filename)  
    im_resized = img.resize((46,56), Image.ANTIALIAS)
    imgByteArr = io.BytesIO()
    im_resized.save(imgByteArr, format='PNG')
    return imgByteArr.getvalue()


### VARS ###

filenames = ["orl_faces/s"+str(s).zfill(2)+"/"+str(f).zfill(2)+".pgm" for s in range(1,41) for f in range(1,11)]

contents = {}
if (len(sys.argv)>0 and sys.argv[1] == "-r"):
    print("Reducing images resolution")
    contents = { f : reduce_res(f) for f in filenames}
else:
    contents = { f : io.FileIO(f).readall() for f in filenames}

sizes = [os.path.getsize(f) for f in filenames]
lzma_filters = my_filters = [
    {
      "id": lzma.FILTER_LZMA2, 
      "preset": 9 | lzma.PRESET_EXTREME, 
      "dict_size": max(sizes) * 10, # a big enough dictionary, but not more than needed, saves memory
      "lc": 3,
      "lp": 0,
      "pb": 0, # assume ascii
      "mode": lzma.MODE_FAST,
      "nice_len": 273,
      "mf": lzma.MF_HC3
    }
]




### FUNCS ###



def get_photo_content(subject, photo):
    return contents["orl_faces/s"+str(subject).zfill(2)+"/"+str(photo).zfill(2)+".pgm"]

def LZMA(content):
    return len(lzma.compress(content, format=lzma.FORMAT_RAW, filters= lzma_filters))

def BZIP2(content):
    return len(bz2.compress(content, 9))

def GZIP(content):
    return len(gzip.compress(content,9))

def ncd(x,y,C):
    if C == "LZMA":
        return (LZMA(x+y) - min(LZMA(x),LZMA(y))) / max(LZMA(x),LZMA(y))
    elif C == "BZIP2":
        return (BZIP2(x+y) - min(BZIP2(x),BZIP2(y))) / max(BZIP2(x),BZIP2(y))
    elif C == "GZIP":
        return (GZIP(x+y) - min(GZIP(x),GZIP(y))) / max(GZIP(x),GZIP(y))


### MAIN ###


ref_dict = {}
for s in range(1,41):
    ref_dict["s"+str(s).zfill(2)]=get_photo_content(s,1)+get_photo_content(s,2)+get_photo_content(s,3)

#print(ref_dict["s01"])

total_matches = 0
for s in range(1,41):
    r = ""
    n_correct_matches = 0

    for f in range(4,11):
        scores = {}
        for ref in range(1,41):
            x = ref_dict["s"+str(ref).zfill(2)]
            y = get_photo_content(str(s).zfill(2),str(f).zfill(2))
            sc = ncd(x,y,sys.argv[-1])
            scores[ref] = sc
        ref_match = min(scores.items(), key=operator.itemgetter(1))[0]
        if ref_match == s:
            n_correct_matches+=1
        r+=(str(s) + "/" + str(f) + "->" + str(ref_match)+" - ")
    print(r+"\tCorrect Matches ="+str(n_correct_matches))
    total_matches+=n_correct_matches
print("Total: ",str(total_matches)+"/280")