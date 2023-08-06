#ifndef DSTRUCTS_ARRAY_H
#define DSTRUCTS_ARRAY_H

// ============================================================================================
//
// array.h -- Array Template Base Class
//
// ============================================================================================

/*
This class is an array.  An "Array" is a "LinearStruct".

The structure of an "Array" object looks like this:

  --------------------------------   ------------
  | Element | Element | Element | ... | Element |
  --------------------------------   ------------

The desired element is determined by the key, which is passed to the subscript operator.

NOTE:  The key's type is not necessarily an integer!

There is one abstract virtual method:  "operator[]()".  Descendent classes are strongly advised
to implement a methods that conform with standard C/C++ array subscripting behaviour.
*/

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <platform.h>

#ifdef FAT_FILENAMES
  #include <dstructs/linearst.h>
#else
  #include <dstructs/linearstruct.h>
#endif

// ============================================================================================
// CLASS DECLARATIONS
// ============================================================================================

template <class T, class Key> class Array:
  virtual public LinearStruct<T>
{
  public:

    // Meaningful operators

    virtual T& operator[](const Key&) = 0;
};

#endif

