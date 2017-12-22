from distutils.sysconfig import get_config_vars
from distutils.core import setup, Extension

cfg_vars = get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")

setup(
    name="KrovetzStemmer",
    version="0.6",
    description="Python binding to the C++ KrovetzStemmer package",
    author="Ruey-Cheng Chen",
    author_email="rueycheng@gmail.com",
    url="https://github.com/rmit-ir/KrovetzStemmer",
    ext_modules=[
        Extension(
            "krovetzstemmer",
            extra_compile_args=['-stdlib=libstdc++'],
            sources=["krovetzstemmer_module.cpp", "KrovetzStemmer.cpp"]
        )
    ]
)
