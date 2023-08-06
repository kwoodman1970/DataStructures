#ifndef DSTRUCTS_LINEARSTRUCTURE_H
#define DSTRUCTS_LINEARSTRUCTURE_H

// ============================================================================================
//
// linearstruct.h -- Linear Data Structure Base Class
//
// ============================================================================================

/*
This class is an abstract data type base class for all linear data structures.

It's completely abstract -- there are no methods and no members.
*/

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <assert.h>

#ifdef FAT_FILENAMES
  #include <dstructs/datastru.h>
#else
  #include <dstructs/datastructure.h>
#endif

// ============================================================================================
// CLASS DECLARATION
// ============================================================================================

template<class T> class LinearStructure:
  virtual public DataStructure<T>
{
  public:
    virtual void        concatenate(const DataStructure<T>&) = 0;

    LinearStructure<T>& operator=(const DataStructure<T>&);
    LinearStructure<T>& operator+=(const DataStructure<T>&);
};

// ============================================================================================
// METHOD DEFINITIONS
// ============================================================================================

template<class T> LinearStructure<T>& LinearStructure<T>::operator=
(
  const DataStructure<T>& source
)

{
  empty();
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template<class T> LinearStructure<T>& LinearStructure<T>::operator+=
(
  const DataStructure<T>& source
)

{
  concatenate(source);
  return *this;
}

#endif