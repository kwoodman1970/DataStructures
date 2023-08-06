#ifndef DSTRUCTS_SARRAY_H
#define DSTRUCTS_SARRAY_H

// ============================================================================================
//
// sarray.h -- Static Array Template Class
//
// ============================================================================================

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <assert.h>
#include <stdarg.h>

#include <sdp.h>
#include <dstructs/array.h>

// ============================================================================================
// CLASS DECLARATIONS
// ============================================================================================

template <class T> class SArray:
  virtual public Array<T, unsigned int>
{
  public:
                           SArray(const unsigned int);
  	                   SArray(const unsigned int, const LinearStruct<T>&);
	                   SArray(const unsigned int, const T *const, const unsigned int);
                           SArray(const unsigned int, const unsigned int, ...);

    // LinearStruct virtual methods

    virtual const bool     isEmpty() const throw ()
                             {return false;}
    virtual const bool     isFull() const throw ()
                             {return false;}
    virtual void           empty()
                           {
                             throw OperationFailed("An SArray can't be emptied.", __FILE__,
                               __LINE__);
                             return;
                           }

    virtual void           iterStart() const throw ();
    virtual const bool     iterMore() const throw ();
    virtual void           iterNext() const throw (OperationFailed);
    virtual const T *const iterCurrent() const throw (OperationFailed);

    // Array virtual methods

    virtual T&             operator[](const unsigned int);

    // Meaningful operators

    SArray<T>&             operator=(const LinearStruct<T>& source);

  private:
    const SDAP<T>           _elements;                 // array of elements
    const SDP<unsigned int> _iterCurrent;              // index of current element in iteration
};

// ============================================================================================
// METHOD DEFINITIONS
// ============================================================================================

/*********************************************************************************************/

template <class T> SArray<T>::SArray
(
  const unsigned int size         // the number of elements that the array must be able to hold
):

/*
This constructor instanciates an empty array that can contain "size" elements.

PRECONDITIONS:
There must be enough memory to create an "SArray" -- otherwise, "DataStruct::CannotCreate" will
be thrown.  Also, "size" must be greater than 0 (as with standard C/C++ arrays).

POSTCONDITIONS:
An array with room for "size" elements is created.  If "T" doesn't have a defined default
constructor then the array should be considered to be uninitialized.
*/

  LinearStruct(),
  _elements((size > 0U) ? new T[size] : NULL),
  _iterCurrent(new unsigned int)

{
  if ((_elements == NULL) || (_iterCurrent == NULL))
    throw ExceptionErrno(ENOMEM, NULL, __FILE__, __LINE__);

  _numElements = size;
  return;
}

/*********************************************************************************************/

template <class T> SArray<T>::SArray
(
  const unsigned int   size,      // the number of elements that the array must be able to hold
  const LinearStruct<T>& source   // linear structure from which elements will be copied
):

/*
This constructor instanciates an array and copies the contents of "source" to the new array
object.  The copy is deep -- that is, each element in "source" is copied -- so that the new
array's structure is independent of "source's" structure.

Refer to the "operator=()" method for information how the copy is performed.

PRECONDITIONS:
There must be enough memory to create an "SArray" -- otherwise, "DataStruct::CannotCreate" will
be thrown.  Also, "size" must not be less than the number of elements in "source".

POSTCONDITIONS:
An array with room for "size" elements is created.  All elements in "source" will have been
copied into the low-end elements of the array.

If "T" doesn't have a defined default constructor then the remaining elements should be
considered to be uninitialized.
*/

  LinearStruct(),
  _elements((size > 0U) ? new T[size] : NULL),
  _iterCurrent(new unsigned int)

{
  if ((_elements == NULL) || (_iterCurrent == NULL))
    throw ExceptionErrno(ENOMEM, NULL, __FILE__, __LINE__);

  if (size < source.numElements())
  {
    throw ExceptionErrno(EINVAL, "\"size\" can't be less than \"source.numElements()\".",
      __FILE__, __LINE__);
  }

  _numElements = size;
  operator=(source);

  return;
}

/*********************************************************************************************/

template <class T> SArray<T>::SArray
(
  const unsigned int size,        // the number of elements that the array must be able to hold
  const T *const     source,
  const unsigned int sourceSize
):

/*
This constructor instanciates an array and copies the contents of "source" to the new array
object.  The copy is deep -- that is, each element in "source" is copied -- so that the new
array's structure is independent of "source's" structure.

Refer to the "copyFrom()" method for information how the copy is performed.

PRECONDITIONS:
There must be enough memory to create an "SArray" -- otherwise, "DataStruct::CannotCreate" will
be thrown.  Also, "size" must not be less than the number of elements in "source".

POSTCONDITIONS:
An array with room for "size" elements is created.  All elements in "source" will have been
copied into the low-end elements of the array.

If "T" doesn't have a defined default constructor then the remaining elements should be
considered to be uninitialized.
*/

  LinearStruct(),
  _elements((size > 0U) ? new T[size] : NULL),
  _iterCurrent(new unsigned int)

{
  if ((_elements == NULL) || (_iterCurrent == NULL))
    throw ExceptionErrno(ENOMEM, NULL, __FILE__, __LINE__);

  if (size < source.numElements())
  {
    throw ExceptionErrno(EINVAL, "\"size\" can't be less than \"sourceSize\".", __FILE__,
      __LINE__);
  }

  _numElements = size;

  for (unsigned int i = 0U; i < sourceSize; i++)
    _elements[i] = source[i];

  return;
}

/*********************************************************************************************/

template <class T> SArray<T>::SArray
(
  const unsigned int size,        // the number of elements that the stack must be able to hold
  const unsigned int numElements, // the number of elements in the parameter list
  ...                             // the elements to be pushed onto the stack (in order)
):

/*
This constructor creates a new array that contains copies of the elements passed in the
argument list.  The elements in the argument list are placed in the array in the order in
which they appear -- in other words, the first element in the argument list will be the first
element in the array.

Note:  due to type promotion issues, "T" cannot be "char", "unsigned char" or "float".

PRECONDITIONS:
"numElements" must be less than or equal to "size".  Also, there must be enough memory to
create an "SArray" -- otherwise, "DataStruct::CannotCreate" will be thrown.

POSTCONDITIONS:
An array with room for "size" elements is created.  All elements in argument list will have
been copied into the low-end elements of the array.

If "T" doesn't have a defined default constructor then the remaining elements should be
considered to be uninitialized.
*/

  LinearStruct(),
  _elements((size > 0U) ? new T[size] : NULL),
  _iterCurrent(new unsigned int)

{
  if ((_elements == NULL) || (_iterCurrent == NULL))
    throw ExceptionErrno(ENOMEM, NULL, __FILE__, __LINE__);

  if (size < source.numElements())
  {
    throw ExceptionErrno(EINVAL, "\"size\" can't be less than \"numElements\".", __FILE__,
      __LINE__);
  }

  _numElements = size;

  va_list argList;                                            // for managing the argument list

  /*
  A simple loop is used to copy each element argument to the array (in the order in which
  they're given).
  */

  va_start(argList, numElements);
  for (unsigned int i = 0U; i < numElements; i++)
    _elements[i] = va_arg(argList, T);
  va_end(argList);

  return;
}

/*********************************************************************************************/

template <class T> void SArray<T>::iterStart() const throw ()
{
  *_iterCurrent = 0U;

  return;
}

/*********************************************************************************************/

template <class T> const bool SArray<T>::iterMore() const throw ()
{
  assert(*_iterCurrent <= _numElements);

  return (*_iterCurrent < _numElements);
}

/*********************************************************************************************/

template <class T> void SArray<T>::iterNext() const throw (DataStruct::OperationFailed)
{
  assert(*_iterCurrent <= _numElements);

  if (*_iterCurrent == _numElements)
  {
    throw OperationFailed("Current element in iteration is undefined.", NULL, __FILE__,
      __LINE__);
  }

  ++(*_iterCurrent);

  return;
}

/*********************************************************************************************/

template <class T> const T *const SArray<T>::iterCurrent() const
  throw (DataStruct::OperationFailed)

/*
This method returns the element currently being iterated through, but doesn't allow it to be
changed.

PRECONDITIONS:
An iteration must be underway and the end of the iteration must not have been reached yet.

POSTCONDITIONS:
None.
*/

{
  assert(*_current <= _numElements);

  if (*_current == _numElements)
  {
    throw OperationFailed("Current element in iteration is undefined.", NULL, __FILE__,
      __LINE__);
  }

  return &(_elements[*_current]);
}

/*********************************************************************************************/

template <class T> SArray<T>& SArray<T>::operator=
(
  const LinearStruct<T>& source                       // the source data structure to copy from
)

/*
This method copies the contents of "source" to the instance.  The copy is deep -- that is,
each element in "source" is copied -- so that the array's elements are independent of
"source's".

PRECONDITIONS:
There must be enough room in the array to hold the copies of "source's" elements.

POSTCONDITIONS:
"source's" elements are copied to the instance's elements.
*/

{
  if (_numElements < source.numElements())
  {
    throw OperationFailed("Assignment operation on SArray would cause an overflow.", NULL,
      __FILE__, __LINE__);
  }

  unsigned int index(0U);                   // index to the next element in _arrayspace to fill

  for (source.iterStart(); source.iterMore(); source.iterNext())
    _elements[index++] = *source.iterCurrent();

  return *this;
}

/*********************************************************************************************/

template <class T> T& SArray<T>::operator[]
(
  const unsigned int index
)

/*
PRECONDITIONS:

POSTCONDITIONS:
*/

{
  if (index >= _numElements)
    throw ExceptionErrno(EINVAL, "\"index\" is out of range.", NULL, __FILE__, __LINE__);

  return _elements[index];
}

#endif

