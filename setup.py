from distutils.sysconfig import get_config_vars
from distutils.core import setup, Extension

cfg_vars = get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")

setup(
    name="KrovetzStemmer",
    version="0.1",
    description="Python binding to the C++ KrovetzStemmer package",
    author="Ruey-Cheng Chen",
    url="https://github.com/rueycheng/KrovetzStemmer",
    ext_modules=[
        Extension(
            "krovetzstemmer",
            sources=["krovetzstemmer_module.cpp",
                     "KrovetzStemmer.cpp"]
        )
    ]
)
