#!/usr/bin/csh



foreach c ( "LZMA" "GZIP" "BZIP2" )
    foreach s ( 1 3 6 )
        foreach r ( "-r" "" )
            echo "python3 face_detection.py $r -s $s -c $c"
            echo "Compressor: $c; Reference size: $s; Reducing mode: "
            python3 face_detection.py $r -s $s -c $c 
            echo ""

end
