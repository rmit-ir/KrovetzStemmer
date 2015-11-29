from distutils.sysconfig import get_config_vars
from distutils.core import setup, Extension

cfg_vars = get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")

setup(
    name="KrovetzStemmer",
    version="1.0",
    ext_modules=[
        Extension("krovetzstemmer",
                  sources=["krovetzstemmer_module.cpp",
                           "KrovetzStemmer.cpp"])
    ]
)
