# DataStructures (C++) &ndash; Classes for Data Structures
**Status:**  Inactive<br />
*This project is not being actively developed at this time but has not been abandoned either.*

## About This Project

In the mid 1990's, I was introduced to [data structures](https://builtin.com/data-science/data-structures) for the first time in a post-secondary course that used [Pascal](https://www.standardpascal.org/) (specifically, [Turbo Pascal 5.5](https://en.wikipedia.org/wiki/Turbo_Pascal#Version_5.5)).  When I later returned to full-time university studies and learned [C++](https://isocpp.org/), I immediately recognized the potential of implementing data structures as template classes.

I set some design goals:

- Create a reasonable and intuitive class hierarchy
- Include staticly- and dynamically-allocated implementations
- Allow data in one structure to be easily copied to a similar structure
- Minimize the code bloat that comes with templates by separating out non-template code

I took inspiration for this library from [Data Structures with Abstract Data Types and Pascal](https://isbnsearch.org/isbn/9780534092641) by [Daniel Stubbs](https://www.researchgate.net/scientific-contributions/Daniel-F-Stubbs-69937610) &amp; [Neil Webre](https://www.researchgate.net/scientific-contributions/Neil-W-Webre-2086313560) and [Algorithms in C](https://isbnsearch.org/isbn/9780201514254) by [Robert Sedgwick](https://sedgewick.io/).

I worked on this in my spare time but, alas, my attention was eventually drawn to other things and I never completed this project.  The advent of the [C++ Standard Template Library](https://www.cplusplus.com/reference/stl/) further dampened my spirits and I realized that my grandiose dream of providing a top-quality C++ data structures library to the world would never come to fruition.

Nevertheless, I'm still fond of this project and would like to finish it some day.

This is more-or-less the class hierarchy that I envisioned &amp; re-envisioned during this time (**bold** denotes a full or partial implementation):

- **DataStructure**
  - **LinearStructure**
    - **Array**
    - List
      - **LinkedList**
      - DoublyLinkedList
      - MultiLinkedList
    - Ring
    - **Stack**
    - **StackPair**
    - Queue
  - Tree
    - BinaryTree
    - BTree
    - AVLTree
  - Graph

## How to Install

*This library is far from complete, not fully documented and not in a condition for general release, but you're welcome to experiment with it in your own projects.*

Use the green `<> Code` button to either clone this repository or download the zip file.

To install, copy the contents of the `src/headers` directory to a directory for your compiler's 3<sup>rd</sup>-party standard header files.  Each compiler is different &ndash; refer to your compiler's manual for information on how to use such directories.

Do **not** place the headers in the same directory as the header files that shipped with your compiler.  Always use a separate directory (or directories) for 3<sup>rd</sup>-party header files.

## How to Use

*This library is far from complete, not fully documented and not in a condition for general release, but you're welcome to experiment with it in your own projects.*

Generally speaking, header files prefixed with "s" denote static allocation and those prefixed with "d" denote dynamic allocation.  Header files without such prefixes contain abstract class declarations. 

Choose which data structures you want to code with, then (as per Best Practices), place the following directive in your source files somewhere after all standard library header files and before all project header files:

```c
#include <dstructs/x.h>
```

where "x" is the header file with the desired data structure.  For example, to use a dynamically-allocated linked list, insert this directive:

```c
#include <dstructs/dlinkedlist.h>
```

Documentation for each class is included in (many of) the files.

### Example

You will need to install my [Platform](https://github.com/kwoodman1970/Platform) and TestSuite (coming soon) libraries for this.

Compile and link `src/example/testdatastructures.cpp` into a command-line executable and run it from the same directory that `src/example/testdata.txt` is in.  It will perform a simple test on the DStack data structure.  Can you make it test the SStack data structure, too?

## Technologies Used

- [Borland C++](https://en.wikipedia.org/wiki/Borland_C%2B%2B)

## TODO

- Refine &amp; expand the class hierarchy a bit more
- Move some header files into subdirectories (if the class hierarchy becomes complex enough to warrent it)
- Implent more data structures
- Create proper documentation (possibly using [Sphinx](https://www.sphinx-doc.org/)) instead of simply saying, "Look at the source files"

## How to Contribute

I'm not accepting contributions to this project at this time.
