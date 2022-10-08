# Code-Compression & Decompression-Algos
### Introduction
- This project implements both code compression and decompression using C++.
- The dictionary can have eight entries (index 3 bits) and the eight entries are selected based on
frequency (the most frequent instruction has index 000). If two entries have the same frequency,
the priority should be given to the one that appears first in the original program order. 
- The original code consists of 32-bit binaries. I use only the following seven possible formats for
compression. 
- If one entry (32-bit binary) can be compressed in more than one way, the code choose the
most beneficial one i.e. the one that provides the shortest compressed pattern. If two formats produce
exactly the same compression, the code choose the one that appears earlier in the following listing (e.g., run-length
encoding appears earlier than direct matching). Also, if there is a scenario where we have two possible
ways of applying bitmasks to a 32-bit binary, the code always give preference to the scenario where the leftmost bit
‘1’ for the bitmask pattern (e.g., 11 is preferred over 01). The starting location of a mismatch
will be counted from the leftmost (MSB) bit of the pattern – If the mismatch is at the MSB, Mismatch Location will
be 00000.

### Seven formats of Code Compression
<code><img height="30" src="https://github.com/ViduraErandika/ViduraErandika/blob/main/Logos/flutter.svg"></code>
