This code uses the Needleman–Wunsch algorithm and a dynamic programming table to compare two genomes. The genomes
are in a fasta file in the format:
>sequence 1 name
(Nucleotide sequence)

>sequence 2 name
(Nucleotide sequence)

After compilation the .exe can be called from the command line using:
 $ <executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>
 The parameters config file gives the values for matches/mismatches