## Prerequisites

- make sure to have autoconf automake and libtool
- install 'eigen' and 'gflags' via brew (brew install eigen) or aptitude (apt-get install libeigen3-dev libgflags-dev)
- optionally install (via apt-get install) libgoogle-glog-dev

    # currently installs gcc 4.9
    brew install gcc autoconf automake eigen gflags
    git clone https://github.com/beniz/libcmaes.git
    cd libcmaes
    ./autogen.sh
    # adjust to the installed version
    CCDIR=/usr/local/Cellar/gcc/4.9.1/bin
    CCVER=4.9
    ./configure CC="$CCDIR/gcc-$CCVER" CXX="$CCDIR/g++-$CCVER" --with-eigen3-include=/usr/local/Cellar/eigen/3.2.2/include/eigen3 CXXFLAGS="-lstdc++ -I/usr/local/include -O4" LDADD=-lgflags LDFLAGS=-L/usr/local/lib
    make
    ./tests/test_functions

