#ifndef DSTRUCTS_LINKEDLIST_H
#define DSTRUCTS_LINKEDLIST_H

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <assert.h>

#ifdef FAT_FILENAMES
  #include <dstructs/linearst.h>
#else
  #include <dstructs/linearstructure.h>
#endif

// ============================================================================================
// CLASS DECLARATION
// ============================================================================================

template <class T> class LinkedList:
  virtual public LinearStructure<T>
{
  public:
    LinkedList<T>&     operator=(const DataStructure<T>&);
    LinkedList<T>&     operator+=(const DataStructure<T>&);

    virtual void       findFirst() throw (Empty) = 0;
    virtual void       findNext() throw (Empty, OperationFailed) = 0;
    virtual T *const   retrieve() throw (Empty) = 0;
    virtual void       insert(const T&);
    virtual void       append(const T&);
    virtual void       remove() = 0;
    virtual const bool isLast() const throw (Empty) = 0;
};

// ============================================================================================
// METHOD DEFINITIONS
// ============================================================================================


template<class T> LinkedList<T>& LinkedList<T>::operator=
(
  const DataStructure<T>& source
)

{
  empty();
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template<class T> LinkedList<T>& LinkedList<T>::operator+=
(
  const DataStructure<T>& source
)

{
  concatenate(source);
  return *this;
}

#endif