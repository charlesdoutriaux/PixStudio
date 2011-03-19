from distutils.core import setup
Version = "0.1"

setup (name = "PixStudio",
       author="PCMDI Software Team",
       version=Version,
       description = "Pictures Manipulator",
       packages = ['PixStudio'],
       package_dir = {'PixStudio': 'Lib'},
      )
    
