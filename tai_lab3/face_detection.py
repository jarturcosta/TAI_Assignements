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
import time

# Default values of the type of compressor, the reference subset size and the boolean variable of reducing image size
reduce = False
compressor = "BZIP2"
ref_size = 3

usage = """Usage: face_detection.py     [  -r (reduce image size; default=false)  ]
                             [  -c compressor (indicate compressor: LZMA, GZIP or BZIP2; default=BZIP2)  ]
                             [  -s ref_size (size of the reference subset; default=3)  ] """


# Argument verification
if len(sys.argv) > 1:
    for i in range(1,len(sys.argv)):
        arg = sys.argv[i]
        if arg == "-r":
            reduce = True
        elif arg == "-c":
            compressor = sys.argv[i+1]
            if compressor not in ["LZMA", "BZIP2", "GZIP"]:
                print(usage)
                sys.exit()
        elif arg == "-s":
            ref_size = int(sys.argv[i+1])
            if ref_size < 0 or ref_size > 9:
                print("Only reference sizes between 1 and 9!",i)
                print(usage)
                sys.exit()
        else:
            if i > 1 and sys.argv[i-1] not in ["-c", "-s"]:
                print(usage)
                sys.exit()



# Function that reduces an image resolution to 45x56
def reduce_res(filename):
    img = Image.open(filename)  
    im_resized = img.resize((46,56), Image.ANTIALIAS)
    imgByteArr = io.BytesIO()
    im_resized.save(imgByteArr, format='PNG')
    return imgByteArr.getvalue()

# File names list construction 
filenames = ["orl_faces/s"+str(s).zfill(2)+"/"+str(f).zfill(2)+".pgm" for s in range(1,41) for f in range(1,11)]

# Extraction of the images content into a list of byte arrays
contents = {}
if (reduce):
    print("Reducing images resolution")
    contents = { f : reduce_res(f) for f in filenames}
else:
    contents = { f : io.FileIO(f).readall() for f in filenames}

sizes = [os.path.getsize(f) for f in filenames]

# Used filters for the LZMA compressor
lzma_filters = my_filters = [
    {
      "id": lzma.FILTER_LZMA2, 
      "preset": 9 | lzma.PRESET_EXTREME, 
      "dict_size": max(sizes) * 10,
      "lc": 3,
      "lp": 0,
      "pb": 0,
      "mode": lzma.MODE_FAST,
      "nice_len": 273,
      "mf": lzma.MF_HC3
    }
]

# Function for populating the reference dictionary with the reference subset data with a given size (number of photos in each subject's reference)
def create_reference_subset(ref_size):
    ref_dict = {}
    for s in range(1,41):
        for i in range(1,ref_size):
            if i>1:
                ref_dict["s"+str(s).zfill(2)]+=get_photo_content(s,i)
            else:
                ref_dict["s"+str(s).zfill(2)]=get_photo_content(s,i)

    return ref_dict

# Function for retrieving an image file content by passing the subject and the photo as an argument
def get_photo_content(subject, photo):
    return contents["orl_faces/s"+str(subject).zfill(2)+"/"+str(photo).zfill(2)+".pgm"]

# Function that returns the amount of bits necessary for compressing an image with the LZMA compressor and the filters defined above
def LZMA(content):
    return len(lzma.compress(content, format=lzma.FORMAT_RAW, filters= lzma_filters))

# Function that returns the amount of bits necessary for compressing an image with the BZIP2 compressor, the default compress level is 9
def BZIP2(content):
    return len(bz2.compress(content))

# Function that returns the amount of bits necessary for compressing an image with the GZIP compressor, the default compress level is 9
def GZIP(content):
    return len(gzip.compress(content))

# Function that calculates and returns the NCD score between two image file contents with a given compressor
def ncd(x,y,C):
    if C == "LZMA":
        return (LZMA(x+y) - min(LZMA(x),LZMA(y))) / max(LZMA(x),LZMA(y))
    elif C == "BZIP2":
        return (BZIP2(x+y) - min(BZIP2(x),BZIP2(y))) / max(BZIP2(x),BZIP2(y))
    elif C == "GZIP":
        return (GZIP(x+y) - min(GZIP(x),GZIP(y))) / max(GZIP(x),GZIP(y))

# Main programm
start = time.time()
ref_dict = create_reference_subset(ref_size)
total_matches = 0
# For all subjects
for s in range(1,41):
    r = ""
    n_correct_matches = 0
    # For all Test Subset images
    for f in range(ref_size+1,11):
        scores = {}
        # Comparing with all the reference subsets
        for ref in range(1,41):
            x = ref_dict["s"+str(ref).zfill(2)]                                         # With the X being the reference of a subject
            y = get_photo_content(str(s).zfill(2),str(f).zfill(2))                      # With the Y being the photo of the subject s and face f
            sc = ncd(x,y,compressor)                                                    # Calculate NCD score of X and Y by compressing with the parameterized compressor
            scores[ref] = sc                                                            # Insert in the scores dictionary
        ref_match = min(scores.items(), key=operator.itemgetter(1))[0]                  # Selecting the subject with best score by choosing the one with the lesser value (closer to 0 means proximity)
        if ref_match == s:
            n_correct_matches+=1                                                        # Counting correct matches
        r+=("s"+str(s).zfill(2) + "/" + str(s).zfill(2) + " -> " + str(s).zfill(2)) 
        if f != 10:
            r+="; "
    print(r+"\tCorrect Matches = "+str(n_correct_matches))
    total_matches+=n_correct_matches
end = time.time()
print("Total: ",str(total_matches)+"/"+str((10-ref_size)*40)+" ({0:.2f}%)".format(float(total_matches/((10-ref_size)*40)*100)))
print("Time elapsed: {0:.2f} seconds".format(end-start))