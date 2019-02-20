# Citing fastq-pair

If you use fastq-pair, please cite: 

fastq-pair: efficient synchronization of paired-end fastq files. 2019. Robert A Edwards and John A Edwards. [bioRxiv 552885](https://www.biorxiv.org/content/10.1101/552885v1); doi: https://doi.org/10.1101/552885

```
@article {Edwards552885,
	author = {Edwards, Robert and Edwards, John A},
	title = {fastq-pair: efficient synchronization of paired-end fastq files},
	elocation-id = {552885},
	year = {2019},
	doi = {10.1101/552885},
	publisher = {Cold Spring Harbor Laboratory},
	abstract = {Paired end DNA sequencing provides additional information about the sequence data that is used in sequence assembly, mapping, and other downstream bioinformatics analysis. Paired end reads are usually provided as two fastq-format files, with each file representing one end of the read. Many commonly used downstream tools require that the sequence reads appear in each file in the same order, and reads that do not have a pair in the corresponding file are placed in a separate file of singletons. Although most sequencing instruments capable of generating paired end reads produce files where each read has a corresponding mate, many downstream bioinformatics manipulations break the one-to-one correspondence between reads, and paired-end sequence files loose synchronicity, and contain either unordered sequences or sequences in one or other file without a mate. Trivial solutions to this problem require reading one or both of the DNA sequence files into memory but quickly become limited by computational resources for moderate to large sized sequence files that are common nowadays. Here, we introduce a fast and memory efficient solution, written in C for portability, that synchronizes paired-end fastq files for subsequent analysis and places unmatched reads into singleton files. The application fastq-pair is freely available from https://github.com/linsalrob/fastq-pair and is released under the MIT license.},
	URL = {https://www.biorxiv.org/content/early/2019/02/19/552885},
	eprint = {https://www.biorxiv.org/content/early/2019/02/19/552885.full.pdf},
	journal = {bioRxiv}
}
```

