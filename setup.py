from distutils.sysconfig import get_config_vars
from distutils.core import setup, Extension

cfg_vars = get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace('-Wstrict-prototypes', '')

long_description = open('README.md').read()

setup(
    name='KrovetzStemmer',
    version='0.8',
    description='Python binding to the C++ KrovetzStemmer package',
    long_description=long_description,
    author='Ruey-Cheng Chen',
    author_email='rueycheng@gmail.com',
    url='https://github.com/rmit-ir/KrovetzStemmer',
    ext_modules=[
        Extension(
            'krovetzstemmer',
            sources=['krovetzstemmer_module.cpp', 'KrovetzStemmer.cpp']
        )
    ],
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'License :: OSI Approved :: MIT License',
        'Topic :: Text Processing'
    ],
    license='MIT'
)
