# img-converter
Convert image format.

Supported input formats: JPEG, PNG, BMP, HDR, PSD, TGA, PIC, PPM, PGM, PBM

Supported output formats: JPEG, PNG, BMP

#COMPILE:
```
make
```
Or
```
gcc lib/sod/sod.c img-converter.c -lm -Ofast -march=native -Wall -std=c99 -o img-converter
```

#USAGE
```
img-converter <src> <dst> [-q=QUALITY]
```

