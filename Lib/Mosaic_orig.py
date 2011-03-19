import os
import numpy
import Image
import ImageStat

def isImage(file):
      try:
            im = Image.open(file)
            return True
      except:
            return False


def preprocess(paths):
    """ from a list of files and/or directores retuns list of valid files """
##     dic_history = {}

##     if os.path.exists(loadFile):
##         f = open(loadFile)
##         dic_history = eval(f.read())
##         f.close()

    files=[]

    for path in paths:
        path=path.strip()
        path=path.replace('\\ ',' ')
        path=path.replace("\\'","'")
##         path=path.replace(' ','\\ ')
##         path=path.replace("'","\\'")
        if isImage(path):
              files.append(path)
        elif os.path.isdir(path):
              Files = os.listdir(path)
              for f in Files:
                    fnm = os.path.join(path,f)
                    if isImage(fnm):
                          files.append(fnm)
    return files


def rgb(files,loadFile=None,method='mean',tkbar=None):
    # ok first creates the files r,g,b arrays
    fr=[]
    fg=[]
    fb=[]
    prev = 0
    n = len(files)
    
    if loadFile is not None and os.path.exists(loadFile):
          f = open(loadFile)
          dic_history = eval(f.read())
          f.close()
    else:
          dic_history={}
          
    for i in range(n):
        f = files[i]
##        if tkbar:
##              prev =genutil.statusbar(float(i),n-1,prev=prev,tk=True,title='Processing files RGB values')
##        else:
##              genutil.statusbar(i,n-1)
        if f in dic_history.keys():
              r,g,b = dic_history[f]
        else:
              im = Image.open(f)
              S = ImageStat.Stat(im)
              if method == 'mean':
                 r,g,b=S._getmean()
              else: # for now no matter what it does the mean....
                 r,g,b=S._getmean()
        dic_history[f]=r,g,b
        fr.append(r)
        fg.append(g)
        fb.append(b)
    # Now stores back the history file
    if loadFile is not None:
          f=open(loadFile,'w')
          f.write(repr(dic_history))
          f.close()
    if tkbar:
          prev[1].destroy()
          prev[0].destroy()
          
    return numpy.array(fr), numpy.array(fg), numpy.array(fb)

# Ok now the big work, creating the puzzle, returns an Image file which you can save
def mosaic(mosaic_file,files,shrink = 20, mosaic_size=75, nfiles=5,rgb_file=None, verbose=False, tkbar=False):

    # ok first creates the list of used files and their r,g,b arrays
    fr,fg,fb = rgb(files,tkbar=tkbar,loadFile=rgb_file)

    # first reads inthe target image
    Im = Image.open(mosaic_file)

    # Now computes the diferent image sizes
    orig_size = Im.size
    new_size = orig_size[0]/shrink,orig_size[1]/shrink
    out_size = (new_size[0]*mosaic_size,new_size[1]*mosaic_size)
    if verbose:
        print 'Input size:',orig_size
        print 'Resized size',new_size
        print 'Output size',out_size

    # shrinks the original image (faster processing, plus it will be (RE)grown by mosaic_size
    Im=Im.resize(new_size)

    # get R,G,B channels for rms computation
    r,g,b=Im.split()

    # converts them to numpy for fast treatment
    r=numpy.array(r.getdata())
    g=numpy.array(g.getdata())
    b=numpy.array(b.getdata())

    # Computes the rms difference between each pixelof the target image and the "pixel value" of each file
    # Warning this could be a big files if the number of image is big and/or the "shrink" factor is too small
    # therefore we split in 10 chunks automatically adjust "split" if still too big
    rms = numpy.ones((nfiles,len(r)))
    split = 1
    seg_size = len(r)
    while seg_size>250000:
        split+=1
        seg_size = len(r)/split
    for i in range(split):
        if verbose:
            print i+1,'of',split
        # rms
        tmp_rms = numpy.sqrt((fr[:,None]-r[None,i*seg_size:(i+1)*seg_size])**2+(fb[:,None]-b[None,i*seg_size:(i+1)*seg_size])**2+(fg[:,None]-g[None,i*seg_size:(i+1)*seg_size])**2)

        # Sorts the rms
        tmp_rms = numpy.argsort(tmp_rms,axis=0)

        # Keeps only the "nfiles" closest files
        tmp_rms=tmp_rms[:nfiles,:]
        rms[:,i*seg_size:(i+1)*seg_size]=tmp_rms
        del(tmp_rms)

    # Now for some diversity gets a random file within the "nfiles" closest files
    i = numpy.random.random_integers(nfiles-1,0,shape=(len(r),))
    #rms = genutil.arrayindexing.get(rms,i,axis=0)
    rms = i.choose(rms)
    if verbose: print 'Final rms shape:',rms.shape

    # Converts to list
    open_files=[]
    for e in rms.tolist():
        if not e in open_files:
            open_files.append(e)
    n = len(open_files)

    # Now creates the output image
    imout = Image.new(Im.mode,out_size)

    icount=0 # counter to know how much of the total image is already processed
    itot = new_size[0]*new_size[1]
    prev = 0
    for j in range(n):
        ifnm = open_files[j]
        fnm = files[ifnm]
        im = Image.open(fnm)
        size,mode,format = im.size,im.mode,im.format
        # crops image to make it square
        if size[0]>size[1]:
            delta = size[0]-size[1]
            im=im.crop((delta/2,0,size[1]+delta/2,size[1]))
        else:
            delta = size[1]-size[0]
            im=im.crop((0,delta/2,size[0],size[0]+delta/2))
        im.load() # to make sure it is cropped correctly (see PIL doc warning)

        # Shrink to mosaic_size to put it instead of output pixels
        im=im.resize((mosaic_size,mosaic_size))
        for i in range(len(rms)):
            if rms[i] == ifnm:
                # computes locatin in i,j in original image
                if tkbar:
                   prev =genutil.statusbar(float(icount),itot-1,prev=prev,tk=True,title='Creating Mosaic File')
                else:
                    genutil.statusbar(icount,itot-1)
                icount+=1
                I = i % new_size[0]
                J = (i-I)/new_size[0]
                imout.paste(im,(I*mosaic_size,J*mosaic_size,(I+1)*mosaic_size,(J+1)*mosaic_size))
    if tkbar:
          prev[1].destroy()
          prev[0].destroy()
    return imout
