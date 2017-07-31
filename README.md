# FASTQ PAIR

This code does one thing: it takes two fastq files, and generates four fastq files. That's right, for free it doubles 
the number of fastq files that you have!!

It rewrites the files with the sequences in order, with matching files for the two files provided on the command line, 
and then any single reads that are not matched are place in two separate files, one for each original file.

This code is designed to be fast and memory efficient, and works with large fastq files. It does not store the whole
file in memory, but rather just stores the locations of each of the indices in the first file provided in memory.

###Speed and efficiency considerations

The most efficient way to use this code is to provide the smallest file first (though it doesn't matter which way you 
provide the files), and then to manipulate the `-t` parameter on the command line. The code implementation is based 
on a [hash table](https://en.wikipedia.org/wiki/Hash_table) and the size of that table is the biggest way to make this 
code run faster. If you set the hash table size too low, then the data structure quickly fills up and the performance
degrades to what we call _O_(n). On the other hand if you  set the table size too big, then you waste a lot of memory, 
and it takes longer to initialize the data structures safely.

The optimal table size is basically somewhere around the number of sequences in your fastq files. You can quickly find 
out how many sequences there are in your fastq file:

```
wc -l fastq_filename
```
The number of sequences will be the number printed here, divided by 4.

If you are not sure, you can run this code with the `-p` parameter. Before it prints out the matched pairs of sequences,
it will print out the number of sequences in each "bucket" in the table. If this number is more than about a dozen you
need to increase the value you provide to `-t`. If most of the entries are zero, then you should decrease the size of 
`-t`.

## Installing fastq_pair

To install the code, grab the github repository, then make a build directory:
```$xslt
mkdir build && cd build
cmake3 ..
make && sudo make install
```
There are more instructions on the [installation](INSTALLATION.md) page.

## Running fastq_pair 

`fastq_pair` takes two primary arguments. The name of the two fastq files that you want to pair up. 

```$xslt
fastq_pair file1.fastq file2.fastq
```

You can also change the size of the hash table using the `-t` parameter:

```$xslt
fastq_pair -t 50021 file1.fastq file2.fastq
```

You can also print out the number of elements in each bucket using the `-p` parameter:

```$xslt
fastq_pair -p -t 100 file1.fastq file2.fastq
```


## Testing fastq_pair

In the [test](test/) directory there are two fastq files that you can use to test `fastq_pair`. There are 250 sequences
in the [left](test/left.fastq) file and 75 sequences in the [right](test/right.fastq) file. Only 50 sequences are common
between the two files.

You can test the code with:

```$xslt
fastq_pair -t 1000 test/left.fastq test/right.fastq
```

This will make four files in the [test/](test) directory:
- left.fastq.paired.fq
- left.fastq.single.fq
- right.fastq.paired.fq
- right.fastq.single.fq

The _paired_ files have 50 sequences each, and the two _single_ files have 200 and 25 sequences (left and right respectively).

### A note about gzipped fastq files

Unfortunately `fastq_pair` doesn't work with gzipped files at the moment, because it relies heavily on random access of 
the file stream. That is complex with gzipped files, especially when the uncompressed file exceeds available memory 
(which is exactly the situation that `fastq_pair` was designed to handle). 

Therefore, at this time, `fastq_pair` does not support gzipped files. You need to uncompress the files before using 
`fastq_pair`.

If you really need to use gzipped files, and can accept slightly worse performance, then 
[we have some alternative](https://edwards.sdsu.edu/research/sorting-and-paring-fastq-files/) approaches
written in Python that you can try.