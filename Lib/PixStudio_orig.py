import os,time,jpeg,Image,ImageTk, Tkinter,sys,Pmw,numpy,tkFileDialog,tkFont,shutil, Mosaic
sys.path.append(os.path.join('/Users','Charles_and_Meryem','Documents','PixStudio','Lib'))

def isImage(file):
    try:
        Image.open(file)
        return True
    except:
        return False
    
#---------------------------------------------------------------------------------
# Event handling function that will allow the passing of arguments
#---------------------------------------------------------------------------------
class Command:
   def __init__(self, func, *args, **kw):
      self.func = func
      self.args = args
      self.kw = kw

   def __call__(self, *args, **kw):
      args = self.args + args
      kw.update(self.kw)
      return apply(self.func, args, kw)
  
class PixStudio(object):
    def readpathin(self):
        self.pathin.setvalue(tkFileDialog.askdirectory(initialdir=self.pathin.getvalue()))
        # Busy Cursor
        cursor = self.root.cget('cursor')
        self.root.configure(cursor=self.defaults['cursors']['busy'])
        self.scan()
        # Not Busy Cursor
        self.root.configure(cursor=cursor)
    def readpathout(self):
        self.pathout.setvalue(tkFileDialog.askdirectory(initialdir=self.pathout.getvalue()))
    def readfileinfo(self):
        self
    def __init__(self,root):
        self.other_types = ['avi','mov','3gp']
        self.defpathin = os.path.join('/XP','D','My Pictures')
        self.defpathin = os.path.join('/XP/D/Eulalie\'s Pictures/2007-01-03-1149-11')
        self.defpathin = os.path.join('/Users/Charles_Meryem_Omar/Desktop')
        self.defpathout = os.path.join('/Users/Charles_Meryem_Omar/Pictures')
        self.def_images_file_info = os.path.join('images.inf')
        self.root = root
        self.root.title('Retrieving/Renaming Pictures')
        self.projects=[]
        l,h=self.root.winfo_screenwidth(),root.winfo_screenheight()
        self.root.geometry('%dx%d+5+5' % (l*.8,h))
        self.root.config(bg='white')
        self.sys_images=[]
        self.defaults={}
        self.defaults['fonts']={}
        self.defaults['fonts']['family']='Courier'
        self.defaults['fonts']['size']=12
        self.Balloon=Pmw.Balloon(root)
        self.internalframeborder=8

        opengif=Image.open('../Pix/open.gif')
        opengif = ImageTk.PhotoImage(opengif)
        self.defaults['images']={}
        self.defaults['images']['open']=opengif
        self.defaults['images']['movies']='../Pix/movies.png'
        self.defaults['images']['target']='../Pix/default.jpg'
        self.defaults['colors']={}
        self.defaults['colors']['main']='white'
        self.defaults['colors']['complementary']='black'
        self.defaults['colors']['secondary']=['salmon','bisque']
        self.defaults['cursors']={}
        self.defaults['cursors']['normal']=''
        self.defaults['cursors']['busy']='watch'


        self.root.configure(cursor=self.defaults['cursors']['normal'])
        
        ## Tkinter Variables
        self.keep        = Tkinter.BooleanVar( root, value=False)
        self.thumbs_size = Tkinter.IntVar(     root, value=80)
        self.images_size = Tkinter.IntVar(     root, value=5)
        self.random      = Tkinter.IntVar(     root, value=5)
        self.shrink      = Tkinter.IntVar(     root, value=20)
        self.delta       = Tkinter.IntVar(     root, value=45)
        self.type        = Tkinter.IntVar(     root, value=1)
        self.mode        = Tkinter.StringVar(  root, value='Acquisition')
        #self.mode        = Tkinter.StringVar(  root, value='Mosaic')
        self.Target      = Tkinter.StringVar(  root, value=self.defaults['images']['target'])
        self.lastDir     = Tkinter.StringVar(  root, value=self.defpathin)

        self.colors = self.defaults['colors']
        
        self.font = tkFont.Font(size=self.defaults['fonts']['size'],
                                family = self.defaults['fonts']['family'],
                                )
        self.Bfont = tkFont.Font(size=self.defaults['fonts']['size'],
                                 family = self.defaults['fonts']['family'],
                                 weight='bold',
                                )
        self.Tfont = tkFont.Font(size=self.defaults['fonts']['size'],
                                family = self.defaults['fonts']['family'],
                                )

        ## Creates Menu
        self.MainMenu = Tkinter.Menu(self.root)
        menu_fichier = Tkinter.Menu(self.MainMenu,tearoff=0)

        # Here tries to add a mode radiobutton
        menu_mode = Tkinter.Menu(self.MainMenu,tearoff=0)
        menu_mode.add_radiobutton(label='Acquisition',variable=self.mode,command=self.setup_mode)
        menu_mode.add_radiobutton(label='Mosaic',variable=self.mode,command=self.setup_mode)
        menu_fichier.add_cascade(label='Change Mode',menu=menu_mode)
       
        menu_fichier.add_command(label='Quit',command=self.root.destroy)
        
        self.MainMenu.add_cascade(label='File',menu=menu_fichier)

        # Ok creates an about menu place holder
        menu_about = Tkinter.Menu(self.MainMenu,tearoff=0)
        self.MainMenu.add_cascade(label='About',menu=menu_about)
        self.root.config(menu=self.MainMenu)

        self.setup_mode()
        
    def setup_mode(self):
        try:
            self.pw.destroy()
            self.MainMenu.delete(2)
            self.menu_tools.destroy()
        except:
            pass
        
        self.pw = Pmw.PanedWidget(self.root,orient='horizontal',
                                  hull_borderwidth = 1,
                                  hull_relief = 'sunken',
                                  hull_bg='blue')
        self.pw.pack(expand=1,fill='both')
        F = self.pw.add('left',max=.5)
        F.pack(expand=1,fill='both')
        self.left_panel = Pmw.ScrolledFrame(F,hull_bg=self.colors['main'],clipper_bg=self.colors['main'],frame_bg=self.colors['main'])
        self.left_panel.pack(expand=1,fill='both')
        for comp in ['hull','clipper','frame']:
            cl = self.left_panel.component(comp)
            cl.pack(expand=1,fill='both')
        self.left_panel = self.left_panel.interior()
        self.left_panel.pack(expand=1,fill='both')
        
        self.right_panel = self.pw.add('right')
        self.right_panel.bind("<Configure>",self.rescale)
        # First Create a Frame to enveloppe all
        F = Tkinter.Frame(self.left_panel,bg=self.colors['main'])
        F.pack(fill='both',expand=1)

        if self.mode.get().lower()=='acquisition':
            setup = self.setup_acquisition_mode
            txt_mode = 'Image Acquisition Mode'
            txt_help = 'Select a directory on the left panel and click on scan'
        elif self.mode.get().lower()=='mosaic':
            setup = self.setup_mosaic_mode
            txt_mode = 'Mosaic Mode'
            txt_help = 'Add directories from the left panel\nSelect pictures then click on mosaic button'
            
        # Creates the mode Name
        lbl = Tkinter.Label(F,text=txt_mode,
                            bg=self.colors['complementary'],
                            fg=self.colors['main'],
                            font=self.Bfont)
        lbl.pack(side='top',fill='x')
        
        ## Puts an help frame in right panel
        self.help = Tkinter.Label(self.right_panel,
                                  bg=self.colors['main'],
                                  text=txt_help,
                                  font=self.Bfont)
        self.help.pack(expand=1,fill='both')

        setup(F)
        return
    
    def setup_mosaic_mode(self,MF):

        ## Menus
        self.menu_tools = Tkinter.Menu(self.MainMenu,tearoff=0)
        self.menu_tools.add_command(label='Add Dir',command=self.addDir)
        self.menu_tools.add_command(label='Create Mosaic',command=self.createMosaic)
        self.MainMenu.insert_cascade(2,label='Tools',menu=self.menu_tools)

        # Icons for clode buttons
        im = Image.open('Pix/exit.png')
        im = im.resize((10,10))
        self.closePix = ImageTk.PhotoImage(im)

        ## Setup area

        ##############################################
        ##############################################
        ## Frame to receive general setup things
        ##############################################
        ##############################################
        Fl = Tkinter.Frame(MF,bg=self.colors['main'],bd=self.internalframeborder,relief='groove',highlightcolor=self.colors['complementary'])
        Fl.pack(fill='both')
        lbl = Tkinter.Label(Fl,text='GUI Parameters',bg=self.colors['main'],font=self.Bfont)
        lbl.pack(side='top',fill='x')
        isection = -1
        ##############################################
        ## Thumbs size section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',expand=1,fill='x')
        self.scale = Tkinter.Scale(F,from_=50,
                           to=200,
                           variable=self.thumbs_size,
                           bg=color,
                           orient='horizontal',
                           font=self.font,
                           label='Display Thumbs size',
                           command=self.rescale,
                           )
        self.scale.pack(side='top',fill='x')
        self.Balloon.bind(self.scale,'Use this scale to change thumbnails size in the right hand panel')

        ##############################################
        ##############################################
        ## Frame to receive setup things
        ##############################################
        ##############################################
        Fl = Tkinter.Frame(MF,bg=self.colors['main'],bd=self.internalframeborder,relief='groove',highlightcolor=self.colors['complementary'])
        Fl.pack(fill='both')
        lbl = Tkinter.Label(Fl,text='Mosaic Parameters',bg=self.colors['main'],font=self.Bfont)
        lbl.pack(side='top',fill='x')
        isection = -1
        self.Balloon.bind(lbl,'In this section you can set parameters to create the mosaic picture')
        ##############################################
        ## Target Image section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',expand=1,fill='x')
        self.target_lbl = Tkinter.Label(F,bg=color,
                            font = self.font,
                            text='Target Image')
        self.target_lbl.pack(side='top')
        width = Fl.winfo_width()
        self.target = Tkinter.Button(F,width=width,height=width,command=self.changeTarget,bg=color)
        self.target.pack(side='top')
        self.Balloon.bind(self.target,'click to change the image used as template for the mosaic')
        sc = Tkinter.Scale(F,from_=2,
                           to=100,
                           variable=self.shrink,
                           bg=color,
                           orient='horizontal',
                           font=self.font,
                           label='Shrink Target Size',
                           )
        sc.pack(side='top',fill='x',expand=1)
        self.Balloon.bind(sc,'Use this scale to reduce the size of the template image\nRemember that the final image will have this size multiplied by the size of the images used for the mosaic')
        ##############################################
        ## Randomization section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',expand=1,fill='x')
        sc = Tkinter.Scale(F,from_=1,
                           to=20,
                           variable=self.random,
                           bg=color,
                           orient='horizontal',
                           font=self.font,
                           label='Randomization Factor',
                           )
        sc.pack(side='top',fill='x')
        self.Balloon.bind(sc,'This scale determine the "randomization".\n I.e at each pixel will be associated this number of possible pictures\n(the n closest)\n Then one of them is picked randomly\nThis is usefull for area where you have a lot of pixel with the same value, it avoid the same image to be present too many times next to itself')
        ##############################################
        ## Mosaic size section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',expand=1,fill='x')
        sc = Tkinter.Scale(F,from_=20,
                           to=200,
                           variable=self.images_size,
                           bg=color,
                           orient='horizontal',
                           font=self.font,
                           label='Mosaic Images size',
                           )
        sc.pack(side='top',fill='x',expand=1)
        self.Balloon.bind(sc,'This scale is used to determine the size of the images put in place of the template picture\'s pixels\nRemember the size of the final picture is this number times the template picture\'s (once shrunk)')
        # Images info file
        Fo = Tkinter.Frame(F,bg=color)
        Fo.pack(side='top',fill='x',expand=1)
        self.images_file_info = Pmw.EntryField(Fo,labelpos = 'w',
                                     label_text='Mosaic Images infos:',
                                     label_bg=color,
                                     label_font=self.font,
                                     value=self.def_images_file_info)
        self.images_file_info.pack(side='left',expand=1)
        b=Tkinter.Button(Fo,text='browse',command=self.readfileinfo,image=self.defaults['images']['open'])
        b.pack(side='right')
        txt = 'file containing informations about the images used to replace the pixels (i.e their mean value)'
        self.Balloon.bind(b,txt)
        self.Balloon.bind(self.images_file_info,txt)

        ##############################################
        ## Directories section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,bg=color,relief='groove',bd=self.internalframeborder/2)
        F.pack(side='top',fill='x')
        # pathout
        Fo = Tkinter.Frame(F,bg=color)
        Fo.pack(side='top',fill='x',expand=1)
        self.pathout= Pmw.EntryField(Fo,labelpos = 'w',
                                     label_text='Store to  :',
                                     label_bg=color,
                                     label_font=self.font,
                                     value=self.defpathout)
        self.pathout.pack(side='left',expand=1)
        b=Tkinter.Button(Fo,text='browse',command=self.readpathout,image=self.defaults['images']['open'])
        b.pack(side='right')
        self.Balloon.bind(b,'Directory where the mosaic file created will be saved')
        ##############################################
        ##############################################
        ## Actions area
        ##############################################
        ##############################################
        Fl = Tkinter.Frame(MF,bg=self.colors['main'],bd=self.internalframeborder,relief='groove',highlightcolor=self.colors['complementary'])
        Fl.pack(fill='both',expand=1)
        lbl = Tkinter.Label(Fl,text='Actions',bg=self.colors['main'],font=self.Bfont)
        lbl.pack(side='top',fill='x')

        ##############################################
        # Add a directory button
        ##############################################
        isection=0
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',fill='x')

        b = Tkinter.Button(F,text='Add a Directory',command=self.addDir)
        b.pack(side='top',expand=1)
        self.Balloon.bind(b,'Click to add a directory from which you can select pictures to use to form the mosaic')
        b = Tkinter.Button(F,text='Create Mosaic',command=self.createMosaic)
        b.pack(side='top',expand=1)
        self.Balloon.bind(b,'Well... Click and wait and you are done!')
        ## Work Area
        Fr = Tkinter.Frame(self.right_panel,bg=self.colors['main'])
        Fr.pack(expand=1,fill='both')
        self.notebook = Pmw.NoteBook(Fr,hull_bg=self.colors['main'])
        self.notebook.pack(fill = 'both', expand = 1)
        self.root.update()
        self.changeTarget('')

    def changeTarget(self,*args):
        if args==():
            self.Target.set(tkFileDialog.askopenfilename())
            image = self.Target.get().strip()
        else:
            image = args[0]
        if image=='':
            image = self.defaults['images']['target']
        im = Image.open(image)
        w,h = wo,ho = im.size
        r = float(w)/h
        W = self.left_panel.winfo_width()*.1
        H = self.left_panel.winfo_height()*.1
        im = im.resize((int(H*r),int(H)))
        w,h = im.size
        self.target_image = ImageTk.PhotoImage(im)
        self.target.configure(width=H*r,height=H,
                              im = self.target_image)
        self.target_lbl.configure(text='Target Image (%dx%d)' % (wo,ho))
    
    def setup_acquisition_mode(self,MF):
        
        # Menus
        self.menu_tools = Tkinter.Menu(self.MainMenu,tearoff=0)
        self.menu_tools.add_command(label='Scan',command=self.readpathin)
        self.menu_tools.add_command(label='Acquire',command=self.acquire_all)
        self.MainMenu.insert_cascade(2,label='Tools',menu=self.menu_tools)

        ## Setup area

        ##############################################
        ##############################################
        ## Frame to receive setup things
        ##############################################
        ##############################################
        Fl = Tkinter.Frame(MF,bg=self.colors['main'],bd=self.internalframeborder,relief='groove',highlightcolor=self.colors['complementary'])
        Fl.pack(fill='both')
        lbl = Tkinter.Label(Fl,text='Parameters',bg=self.colors['main'],font=self.Bfont)
        lbl.pack(side='top',fill='x')
        isection=-1
        
        ##############################################
        ## Directories section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,bg=color,relief='groove',bd=self.internalframeborder/2)
        F.pack(side='top',fill='x')
        # pathin
        Fi = Tkinter.Frame(F,bg=color)
        Fi.pack(side='top',fill='x',expand=1)
        self.pathin= Pmw.EntryField(Fi,labelpos = 'w',
                                    label_text='Read from:',
                                    label_bg=color,
                                    label_font=self.font,
                                    value=self.defpathin)
        self.pathin.pack(side='left',expand=1)
        b=Tkinter.Button(Fi,text='browse',command=self.readpathin,image=self.defaults['images']['open'])
        b.pack(side='right')
        # pathout
        Fo = Tkinter.Frame(F,bg=color)
        Fo.pack(side='top',fill='x',expand=1)
        self.pathout= Pmw.EntryField(Fo,labelpos = 'w',
                                     label_text='Store to  :',
                                     label_bg=color,
                                     label_font=self.font,
                                     value=self.defpathout)
        self.pathout.pack(side='left',expand=1)
        b=Tkinter.Button(Fo,text='browse',command=self.readpathout,image=self.defaults['images']['open'])
        b.pack(side='right')

        ##############################################
        ## Thumbs size section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',expand=1,fill='x')
        self.scale = Tkinter.Scale(F,from_=50,
                           to=200,
                           variable=self.thumbs_size,
                           bg=color,
                           orient='horizontal',
                           font=self.font,
                           label='Thumbs size',
                           command=self.rescale,
                           )
        self.scale.pack(side='top',fill='x')

        ##############################################
        ## Time between projects section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        f = Tkinter.Frame(Fl,bg=color,relief='groove',bd=self.internalframeborder/2)
        f.pack(side='top',fill='x',expand=1)
        # scale for delta
        sc = Tkinter.Scale(f,from_=1,
                           to=100,
                           variable=self.delta,
                           bg=color,
                           orient='horizontal',
                           font=self.font,
                           label='Interval bet. projects',
                           )
        sc.pack(side='left',fill='x',expand=1)
        # radio buttons
        f=Tkinter.Frame(f,bg=color)
        f.pack(side='left')
        rb = Tkinter.Radiobutton(f,
                                 font=self.font,
                                 bg=color,
                                 variable=self.type,
                                 text = 'Minutes',
                                 value = 1,
                                 )
        rb.pack(side='top')
        rb = Tkinter.Radiobutton(f,
                                 font=self.font,
                                 bg=color,
                                 variable=self.type,
                                 text = 'Hours',
                                 value = 2,
                                 )
        rb.pack(side='left')
        
        ##############################################
        ## Keep original files section
        ##############################################
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',expand=1,fill='x')
        cb = Tkinter.Checkbutton(F,bg=color,variable=self.keep,text='Keep Original files',font=self.font)
        cb.pack(side='top',fill='x')


        ##############################################
        ##############################################
        ## Actions area
        ##############################################
        ##############################################
        Fl = Tkinter.Frame(MF,bg=self.colors['main'],bd=self.internalframeborder,relief='groove',highlightcolor=self.colors['complementary'])
        Fl.pack(fill='both',expand=1)
        lbl = Tkinter.Label(Fl,text='Actions',bg=self.colors['main'],font=self.Bfont)
        lbl.pack(side='top',fill='x')
        isection=-1

        ## Buttons
        isection+=1
        color = self.colors['secondary'][isection % 2]
        F = Tkinter.Frame(Fl,relief='groove',bg=color,bd=self.internalframeborder/2)
        F.pack(side='top',fill='x')

##         b = Tkinter.Button(F,text='(Re)Scan',command=self.scan)
##         b.pack(side='top',expand=1)
        b = Tkinter.Button(F,text='Retrieve All Projects',command=self.acquire_all)
        b.pack(side='top',expand=1)

        ## Work Area
        Fr = Tkinter.Frame(self.right_panel,bg=self.colors['main'])
        Fr.pack(expand=1,fill='both')
        self.notebook = Pmw.NoteBook(Fr,hull_bg=self.colors['main'])
        self.notebook.pack(fill = 'both', expand = 1)
        
        
        
##         self.pw.updatelayout()

    def scan(self):
        self.help.destroy()
        files=os.listdir(self.pathin.getvalue())
        files.sort()
        self.ftimes = []
        self.ptimes = []
        self.files=[]
        prev = 0
        prevptime = 0
        # First of all figures out date on each file (jpeg capture time if available)
        for f in files:
            f= os.path.join(self.pathin.getvalue(),f)
            ext = f.split('.')[-1].lower()
            if isImage(f) or ext in self.other_types:
                fnm = os.path.join(self.pathin.getvalue(),f)
                self.files.append(fnm)
                ftime = os.path.getctime(fnm)
                if isImage(f): # Image file
                    try: # jpeg exif ?
                        exif = jpeg.getExif(fnm)
                        info = exif.dict()
                        date = info['DateTimeOriginal'][2]
                        date = time.strptime(date,'%Y:%m:%d %H:%M:%S')
                        ptime = time.mktime(date)
                        prev_ext='jpeg'
                    except:
                        if ftime - prev < self.delta.get()*60**self.type.get():
                            ptime = prevptime
                        else:
                            ptime = ftime
                elif ext == '3gp':
                    #print 'Found 3gp:',fnm,ftime
                    date = fnm.split("video-")[-1][:-4]
                    ptime = time.mktime(time.strptime(date,"%Y-%m-%d-%H-%M-%S"))
                    #print date,ptime,ftime
                else:
                    # ok probably a movie, no movie library now, we take time from previous
                    if ftime - prev < self.delta.get()*60**self.type.get():
                        ptime = prevptime
                    else:
                        ptime = ftime
                prev = ftime
                prevptime = ptime
                self.ftimes.append(ftime)
                self.ptimes.append(ptime)
        self.sort_projects()
                
    def sort_projects(self):
        self.projects=[]
        order = numpy.argsort(self.ptimes) # sorts by picture time
        files = numpy.take(self.files,order)
        ptimes = numpy.take(self.ptimes,order)
        prev = 0
        pimg=[]
        for p in self.notebook.pagenames():
            self.notebook.delete(p)
        for i in range(len(files)):
            fnm=files[i]
            ptime = ptimes[i]
            gmtime = time.gmtime(ptime)
            if ptime-prev>self.delta.get()*60**self.type.get():
                if pimg!=[]:
                    self.projects.append({})
                    self.projects[-1]['image_files']=pimg
                    pimg=[]
            prev=ptime
            pimg.append((fnm,gmtime))
        if pimg!=[]:
            self.projects.append({})
            self.projects[-1]['image_files']=pimg
        self.display_projects()

    def removeDir(self,project,nm):
        self.projects.remove(project)
        nb = self.notebook.page(nm).destroy()
        self.notebook.delete(nm)
        
    def addDir(self):
        if self.projects==[]:
            self.help.destroy()
            
        D = tkFileDialog.askdirectory(initialdir=self.lastDir.get())
        
        if D==(): # Abort
            return
        
        self.lastDir.set(D)

        # Busy Cursor
        cursor = self.root.cget('cursor')
        self.root.configure(cursor=self.defaults['cursors']['busy'])
        files = os.listdir(D)
        pimg = []
        for f in files:
            try:
                nm = os.path.join(D,f)
                Image.open(nm)
                pimg.append((numpy.array(nm),D)) # to be consistent with others and able to use existing funcs
            except:
                pass
        self.projects.append({})
        self.projects[-1]['image_files']=pimg
        self.display_projects(self.projects[-1])
        self.root.configure(cursor=cursor)

    def display_projects(self,*args):
        # Busy Cursor
        cursor = self.root.cget('cursor')
        self.root.configure(cursor=self.defaults['cursors']['busy'])
        ## first cleanup existing pages
        ## Little prep
        ip=0
        self.Tfont.configure(size=self.font.cget('size'))
        if len(args)==0:
            projs = self.projects
        else:
            projs = args
        for p in projs:
            p['images']=[]
            p['tk_images']=[]
            p['selected']=[]
            p['rotated']=[]
            p['buttons']=[]
            p['checkbuttons']=[]
            p['frames']=[]
            p['labels']=[]
            gmtime = p['image_files'][0][1]
            if isinstance(gmtime,str):
                nm_root = os.path.split(gmtime)[1] # Xtra space to allow button
            else:
                nm_root = time.strftime('%Y-%m-%d',gmtime)
            nm_root = nm_root.replace('_','-')
            # Now adds number if name already there
            multi = 1
            cont = True
            while cont:
                try:
                    if multi == 1:
                        nm = nm_root
                    else:
                        nm = '%s (%i)' % (nm_root, multi)
                    if isinstance(gmtime,str):
                        nm = '%s  ' % nm
                    nb = self.notebook.add(nm,tab_bg=self.colors['main'],tab_font=self.Bfont,page_bg='pink')
                    cont = False
                except Exception,err:
                    print 'error:',err
                    multi+=1
                    

            nb = self.notebook.page(nm)
            self.notebook.selectpage(nm) # gives focus
            
            F = Tkinter.Frame(nb,bg=self.colors['main'])
            F.pack(expand=1,fill='both')
##             f = Tkinter.Frame(F,bg=self.colors['complementary'])
##             f.pack(fill='x',side='top')
            if not isinstance(gmtime,str):
                p['name']= Pmw.EntryField(F,
                                          labelpos = 'w',
                                          label_text = 'Project Name:',
                                          label_bg=self.colors['complementary'],
                                          label_fg=self.colors['main'],
                                          label_font=self.Bfont,
                                          entry_bg=self.colors['main'],
                                          entry_fg=self.colors['complementary'],
                                          entry_font=self.font,
                                          validate = None)
                p['name'].pack(side='top',fill='x')
                p['name'].setvalue('Project_%i' % ip)
            Ftop=Tkinter.Frame(F,bg=self.colors['main'],relief='raised')
            Ftop.pack(fill='x',side='top')

            # Inverse selection button
            b = Tkinter.Button(Ftop,text='Inverse Selection',command=Command(self.invertSelection,p))
            b.pack(side='left')
            self.Balloon.bind(b,'Inverse selection')
            if isinstance(gmtime,str):
                tab = self.notebook.tab(nm)
                b = Tkinter.Button(tab,image=self.closePix,text='Remove this dir',command=Command(self.removeDir,p,nm))
                b.pack(side='right')
                self.Balloon.bind(b,'Click to remove this directory')
            else:
                b = Tkinter.Button(Ftop,text='Get this project',command=self.acquire)
                b.pack(side='left')
                self.Balloon.bind(b,'Acquire Selected Pictures')

                b = Tkinter.Button(Ftop,text='Rotate Left',command=Command(self.rotate,p,90))
                b.pack(side='left')
                self.Balloon.bind(b,'Rotate Left Selected Pictures')
                b = Tkinter.Button(Ftop,text='Rotate Right', command=Command(self.rotate,p,-90))
                b.pack(side='left')
                self.Balloon.bind(b,'Rotate Right Selected Pictures')

            Fmid = Pmw.ScrolledFrame(F,hull_bg=self.colors['main'],clipper_bg=self.colors['main'],frame_bg=self.colors['main'])
            Fmid.pack(expand=1,fill='both')
            Fmid = Fmid.interior()
            Fmid.pack(expand=1,fill='both')
            p['Frame']=Fmid
            ip+=1
            sz = self.thumbs_size.get()
            npr = self.number_images_per_row()
            for ifile in range(len(p['image_files'])):
                finfo = p['image_files'][ifile]
                fnm,sfnm,ext,prefix,im = self.process_file_info(finfo)
                f = Tkinter.Frame(Fmid)
                row = ifile / npr
                col  = ifile % npr
                f.grid(column=col,
                       row=row,
                       ipadx=5,
                       ipady=5,
                       )
                
                b=Tkinter.Button(f,
                                 command=Command(self.clickbutton,p,ifile),
                                 bg=self.colors['main'],
                                 )
                b.pack(side='top')

                ## Now tries to put balloon on it
                size = os.path.getsize(fnm)
                Units =['bytes','kb','Mb','Gb','Tb']
                for i in range(len(Units)):
                    if size>1024:
                        size/=1024.
                    else:
                        break
                units = Units[i]
                if isImage(fnm):
                    tmp_im = Image.open(fnm)
                    balloon_txt='Location: %s\nDimensions: %s\nSize: %0.2f %s\n' % (fnm,tmp_im.size,size,units)
                else:
                    balloon_txt='Location: %s\nSize: %0.2f %s\n' % (fnm,size, units)
                try:
                    exif = jpeg.getExif(fnm).dict()
                    balloon_txt+='\n'
                    keys = exif.keys()
                    keys.sort()
                    for k in keys:
                        val = repr(exif[k][2])
                        if k[0]!="'" and len(val)<60:
                            balloon_txt+='%s : %s\n' % (k,val)                    
                except Exception,err:
                    pass

                self.Balloon.bind(b,balloon_txt[:-1])

                lbl = Tkinter.Label(f,
                                    font= self.Tfont,
                                    text = prefix)
                if prefix!='':
                    lbl.pack(side='top',
                             bg=self.colors['main'],
                             )

                S = Tkinter.BooleanVar(f,value=False)
                cb = Tkinter.Checkbutton(f,
                                         text=sfnm,
                                         command = Command(self.configure_button,p,ifile),
                                         font=self.Tfont,
                                         variable = S,
                                         bg=self.colors['main'],
                                        )
                cb.pack(side='top')
                p['images'].append(im)
                p['tk_images'].append('')
                p['selected'].append(S)
                p['rotated'].append(Tkinter.IntVar(f,value=0))
                p['buttons'].append(b)
                p['checkbuttons'].append(cb)
                p['frames'].append(f)
                p['labels'].append(lbl)
                self.configure_button(p,ifile)
                self.root.update()
        # Not Busy Cursor
        self.root.configure(cursor=cursor)

    def rescale(self,size):
        # Busy Cursor
        cursor = self.root.cget('cursor')
        self.root.configure(cursor=self.defaults['cursors']['busy'])
        try:
            self.Tfont.configure(size=self.font.cget('size'))
            for p in self.projects:
                n= len(p['images'])
                p['Frame'].grid_forget()
                npr = self.number_images_per_row()
                for i in range(n):
                    row = i / npr
                    col  = i % npr
                    p['frames'][i].grid(row=row,column=col)
                    self.configure_button(p,i)
        except Exception,err:
            pass
        # Not Busy Cursor
        self.root.configure(cursor=cursor)

    def configure_button(self,project,index):
        sz = self.thumbs_size.get()
        if project['selected'][index].get():
            bg = self.colors['complementary']
        else:
            bg=self.colors['main']
        project['frames'][index].configure(bg=bg)
        lbl = project['labels'][index]
        prefix = lbl.cget('text')
        cb = project['checkbuttons'][index]
        sfnm = prefix + cb.cget('text')
        prefix,sfnm = self.Thumbs_font_size(sfnm)
        cb.configure(text=sfnm)
        lbl.configure(text=prefix)
        S = project['images'][index].size
        r = float(S[0])/S[1]
        if r>1:
            sz1,sz2 = int(sz),int(sz/r)
        else:
            sz1,sz2 = int(sz/r), int(sz)

        if project['rotated'][index].get()!=0:
            tk_img = ImageTk.PhotoImage(project['images'][index].resize((sz1,sz2)).rotate(project['rotated'][index].get()))
        else:
            tk_img = ImageTk.PhotoImage(project['images'][index].resize((sz1,sz2)))
        project['tk_images'][index]=tk_img
        project['buttons'][index].configure(image=tk_img,
                                  width=sz,
                                  height=sz)
        
    def process_file_info(self,finfo):
        fnm,gmtime = finfo
        fnm= ''.join(fnm.tolist()).strip()
        sp = fnm.split('.')
        ext = sp[-1].lower()
        sfnm = '.'.join(sp[:-1])
        sfnm = os.path.split(sfnm)[-1]
        prefix = ''
        prefix,sfnm = self.Thumbs_font_size(sfnm)
        if isImage(fnm):
            im=Image.open(fnm)
        else:
            im = Image.open(self.defaults['images']['movies'])
        sz = self.scale.cget('to')
        S = im.size
        r = float(S[0])/S[1]
        if r>1:
            im = im.resize((int(sz),int(sz/r)))
        else:
            im = im.resize((int(sz/r),int(sz)))
        return fnm,sfnm,ext,prefix,im

    def Thumbs_font_size(self,sfnm):
        sz = self.thumbs_size.get()
        prefix=''
        while self.Tfont.measure(sfnm)+20>sz: # 20 size of checkbox
            if self.Tfont.cget('size')>1:
                self.Tfont.configure(size=self.Tfont.cget('size')-1)
            else:
                prefix+=sfnm[0]
                sfnm = sfnm[1:]
        return prefix,sfnm

    def number_images_per_row(self):
        sz = self.thumbs_size.get()
        w=eval(self.right_panel.winfo_geometry().split('x')[0]) - 80 # safety padding
        npr = int( w / (sz+10))  #10 is twice the padding
        return npr

    def invertSelection(self,project):
        n = len(project['selected'])
        for i in range(n):
            val = project['selected'][i].get()
            project['selected'][i].set(not val)
            self.configure_button(project,i)
            
    def rotate(self,project,angle):
        n = len(project['images'])
        for i in range(n):
            if project['selected'][i].get():
                rot = project['rotated'][i].get()
                project['rotated'][i].set(rot+angle)
                self.configure_button(project,i)
        return
    
    def clickbutton(self,project,index):
        """Select/Unselct an image in a project"""
        cb = project['checkbuttons'][index]
        f = project['frames'][index]
        V = project['selected'][index]
        cb.toggle()
        self.configure_button(project,index)
            

    def createMosaic(self):
        # Busy Cursor
        cursor = self.root.cget('cursor')
        self.root.configure(cursor=self.defaults['cursors']['busy'])
        tfile = self.Target.get().strip()
        if tfile=='':
            tfile = self.defaults['images']['target']
        files = []
        for p in self.projects:
            all = True
            n = len(p['image_files'])
            for i in range(n):
                if p['selected'][i].get():
                    all = False
                    break
            for i in range(n):
                if p['selected'][i].get() or all:
                    finfo = p['image_files'][i]
                    fnm,sfnm,ext,prefix,im = self.process_file_info(finfo)
                    files.append(fnm)
        #Process call
        shrink = self.shrink.get()
        mosaic_size = self.images_size.get()
        random = self.random.get()
        imout = Mosaic.mosaic(tfile,
                              files,
                              shrink = shrink,
                              mosaic_size= mosaic_size,
                              nfiles=random,
                              rgb_file=self.images_file_info.getvalue().strip(),
                              verbose=False,
                              tkbar=True)
        nm = '.'.join(os.path.split(tfile)[1].split('.')[:-1])
        nm = '%s_%dx%d_%d_%d_%d_%d.jpg' % (nm,
                                           imout.size[0],
                                           imout.size[1],
                                                shrink,
                                           mosaic_size,
                                           random,
                                           len(files),
                                           )
        onm = os.path.join(self.pathout.getvalue(),nm)
        imout.save(onm,"JPEG")
        # Not Busy Cursor
        self.root.configure(cursor=cursor)
        
    def acquire(self,project_id=None):
        # Busy Cursor
        cursor = self.root.cget('cursor')
        self.root.configure(cursor=self.defaults['cursors']['busy'])
        if project_id is None:
            project_id = self.notebook.index(self.notebook.getcurselection())
        num = 1
        p = self.projects[project_id]
        nms = self.notebook.pagenames()
        pgnm = nms[project_id].split('(')[0]
        pnm = p['name'].getvalue().strip()
        while pnm=='':
            pnm = raw_input('Please enter a project name for files from %s' % pgnm)
            pnm=pnm.strip()
        pdir = "%s, %s" % (pgnm,pnm)
        pathout = self.pathout.getvalue().strip()
        if pathout == '':
            pathout=self.defpathout
        pth = os.path.join(pathout,pdir)
        try:
            os.makedirs(pth)
        except:
            pass

        files = os.listdir(pth)

        ## Figures out start number
        num = 1
        n =len(pnm)
        for f in files:
            if f[:n]== pnm:
                sp = f[n+1:].split('.')[0]
                try:
                    num = max(num,int(sp)+1)
                except:
                    pass


        # Figures out if some selected or noe which means get all
        all = True
        n = len(p['image_files'])
        for i in range(n):
            if p['selected'][i].get():
                all = False
                break
        ncopy=0
        for ifile in range(n):
            if p['selected'][ifile].get() or all:
                finfo = p['image_files'][ifile]
                fnm,sfnm,ext,prefix,im = self.process_file_info(finfo)
                nm = "%s_%s.%s" % (pnm,str(num).zfill(3),ext)
                new_name = os.path.join(pth,nm)
                print 'Name is:',nm
                if self.keep.get():
                    shutil.copyfile(fnm,new_name)
                    shutil.copystat(fnm,new_name)
                    methd = 'copied'
                else:
                    print 'Moving:',fnm
                    print 'To:',new_name
                    shutil.move(fnm,new_name)
                    methd = 'moved'
                # now deals with rotations...
                rot = p['rotated'][i].get()
                if rot!=0:
                    im = Image.open(new_name)
                    exif = None
                    try:
                        exif = jpeg.getExif(new_name)
                    except:
                        pass
                    im = im.rotate(rot)
                    im.save(new_name)
                    if exif is not None:
                        jpeg.setExif(exif,new_name)
                    
                ncopy+=1
                num+=1

        D = Pmw.MessageDialog(p['Frame'],
                              title='File Processed',
                              message_text = '%s files %s to: %s' % (ncopy,methd,pth)
                              )
        if pgnm in self.notebook.pagenames():
            self.notebook.delete(pgnm)
        # Not Busy Cursor
        self.root.configure(cursor=cursor)

    def acquire_all(self):
        for i in range(len(self.projects)):
            self.acquire(project_id=i)
            
if __name__=='__main__':
    root=Tkinter.Tk()
    A=PixStudio(root)
    root.mainloop()
    
