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
#include <sdp.h>

#ifdef FAT_FILENAMES
  #include <dstructs/datastru.h>
#else
  #include <dstructs/datastructure.h>
#endif

// ============================================================================================
// CLASS DECLARATIONS
// ============================================================================================

template<class T> class LinearStructure:
  virtual public DataStructureExceptions,
  virtual public DataStructure<T>
{
  public:
    LinearStructure<T>& operator=(const DataStructure<T>&)  throw (Full, OperationFailed);
    LinearStructure<T>& operator+=(const DataStructure<T>&) throw (Full, OperationFailed);

  protected:
    virtual void        concatenate(const DataStructure<T>&) throw (Full, OperationFailed) = 0;
};

#if 0

template<class T> class SLinearStruct:
  virtual public LinearStruct<T>
{
  public:
    SLinearStruct(const unsigned int);

  protected:
    const unsigned int      _maxElements;
    const SDAP<T>           _elements;
    const SDP<unsigned int> _iterCurrent;

    void assertInvariants() const throw ();
};

#endif

// ============================================================================================
// METHOD DEFINITIONS
// ============================================================================================

/*********************************************************************************************/

template<class T> LinearStructure<T>& LinearStructure<T>::operator=
(
  const DataStructure<T>& source
)
throw (DataStructureExceptions::Full, DataStructureExceptions::OperationFailed)

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
throw (DataStructureExceptions::Full, DataStructureExceptions::OperationFailed)

{
  concatenate(source);
  return *this;
}

// ============================================================================================
// SLINEARSTRUCT METHODS
// ============================================================================================

#if 0

/*********************************************************************************************/

template<class T> SLinearStruct<T>::SLinearStruct
(
  const unsigned int initialSize
):

  _maxElements(initialSize),
  _elements((initialSize > 0U) ? new T[initialSize] : NULL),
  _iterCurrent(new unsigned int)

{
  if (initialSize == 0U)
  {
    ExceptionErrno::presetDetails() << "\"initialSize\" can't be 0U.";
    throw ExceptionErrno(EINVAL, __FILE__, __LINE__);
  }

  if ((_elements == NULL) || (_iterCurrent == NULL))
    throw ExceptionErrno(ENOMEM, __FILE__, __LINE__);

  return;
}

/*********************************************************************************************/

template<class T> void SLinearStruct<T>::assertInvariants() const throw ()
{
  assert(_numElements <= _maxElements);
  LinearStruct<T>::assertInvariants();

  return;
}

#endif

#endif