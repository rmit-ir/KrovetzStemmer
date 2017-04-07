# KrovetzStemmer

Python binding to the C++ KrovetzStemmer package, originally distributed with
the Lemur toolkit.


## Installation ##

The package is on PyPI so usually you just go:

    pip install krovetzstemmer

Or you can choose to clone this repo, cd into it, and install the package from there:

    pip install .


## Usage ##

The usage is similar to the python package `porterstemmer`.

    from krovetzstemmer import Stemmer
    stemmer = Stemmer()
    stemmer.stem('utilities')  # got: 'utility'
    stemmer.stem(u'utilities')  # got: u'utility'

## Contributors ##

* Ruey-Cheng Chen
