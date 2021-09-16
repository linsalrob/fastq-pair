# INSTALLING fastq_pair

We use cmake (version 3 or higher) for `fastq_pair`. The best way to install it is to use these commands:

```
mkdir build && cd build
cmake3 ../
make && sudo make install
```

If you don't have root access, you can also install `fastq_pair` in your home directory like this.

```
mkdir build && cd build
cmake3 -DCMAKE_INSTALL_PREFIX=$HOME/bin ..
make && make install
```

You should then be able to call `fastq_pair.

## Alternative Installation

As noted in [issue #3](https://github.com/linsalrob/fastq-pair/issues/3) you don't really need to use CMAKE at all.

As an alternative, you can run

```
gcc -std=gnu99  -o fastq_pair fastq_pair.c main.c  robstr.c is_gzipped.c 
```

Which will compile the code and create an executable for you!


### Note:

This implementation is based on a [hash table](https://en.wikipedia.org/wiki/Hash_table). The key
factor that controls the speed of the program is the size of the table. We start with a default table
size of 100,003. There are lots of recommendations on the table size, but for optimum speed it should 
approach (but doesn't have to be) the number of sequences in your fastq files. 

You can alter the size of the table with the -t parameter. If you request a table that is too big for
your available memory the program won't run.

