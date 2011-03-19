import PixStudio

#print dir(PixStudio.EXIF)
fnm = "/Users/doutriaux1/Desktop/IMAG0211.jpg"

f= open(fnm)

tags = PixStudio.EXIF.process_file(f)

print tags.keys()

for k in tags.keys():
    if k not in ('JPEGThumbnail', 'TIFFThumbnail', 'Filename',
                   'EXIF MakerNote'):
        print k,tags[k]
#print myexif
