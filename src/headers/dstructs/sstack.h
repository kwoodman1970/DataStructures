#ifndef DSTRUCTS_SSTACK_H
#define DSTRUCTS_SSTACK_H

// ============================================================================================
//
// sstack.h -- Implementation of a static stack -- that is, a stack that stores its elements in
// an array of fixed size.
//
// ============================================================================================

// ============================================================================================
// DESIGN NOTES
// ============================================================================================

/*
"SStack" is implemented as an array.  Element 0 is the tail -- that is, the last element to
be popped off of the stack.

+-----+-----+-----+-----+-----+-----+-----+-----+
| 5th | 4th | 3rd | 2nd | 1st |  ?  |  ?  |  ?  |
+-----+-----+-----+-----+-----+-----+-----+-----+
                           ^     ^
                   head ---+     |
                   _numElements -+

To reduce complexity and increase efficiency, "SLinearStruct::_numElements" is considered to be
one greater than the head -- specificly, the index of where the next element to be pushed onto
the stack would go.

All methods are written with the possibility that an exception may be thrown as one "T" is
assigned to another.  That means that, if an exception is thrown while a method is being
called, the "SStack" will not have changed as far as the caller is concerned.
*/

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <assert.h>
#include <stdarg.h>
#include <iomanip.h>

#include <sdp.h>
#include <dstructs/stack.h>

// ============================================================================================
// CLASS DECLARATIONS
// ============================================================================================

template <class T> class SStack:
  virtual public SLinearStruct<T>,
  virtual public Stack<T>
{
  public:
		           SStack(const unsigned int);
		           SStack(const unsigned int, const LinearStruct<T>&);
                           SStack(const unsigned int, const T *const, const unsigned int);
                           SStack(const unsigned int, const unsigned int, ...);

    // LinearStruct virtual methods

    virtual const bool     isEmpty() const throw ()
                             {assertInvariants(); return (_numElements == 0U);}
    virtual const bool     isFull() const throw ()
                             {assertInvariants(); return (_numElements == _maxElements);}
    virtual void           empty() throw ();

    virtual void           iterStart() const throw ();
    virtual const bool     iterMore() const throw ();
    virtual void           iterNext() const throw (OperationFailed);
    virtual const T *const iterCurrent() const throw (OperationFailed);

    // Stack virtual methods

    virtual void           push(const T&);
    virtual void           pop(T&);
    virtual void           peek(T&) const;

    // Meaningful operators

    SStack<T>&             operator=(const LinearStruct<T>& source);
    SStack<T>&             operator+=(const LinearStruct<T>&);
    SStack<T>              operator+(const LinearStruct<T>& source);
};

// ============================================================================================
// METHOD DEFINITIONS:
// ============================================================================================

/*********************************************************************************************/

template <class T> SStack<T>::SStack
(
  const unsigned int size         // the number of elements that the stack must be able to hold
):

/*
This constructor instanciates an empty stack that can contain "size" elements.

PRECONDITIONS:
None.

POSTCONDITIONS:
An empty stack with room for "size" elements is created.
*/

  SLinearStruct<T>(size)

{
  assertInvariants();
  return;
}

/*********************************************************************************************/

template <class T> SStack<T>::SStack
(
  const unsigned int     size,    // the number of elements that the stack must be able to hold
  const LinearStruct<T>& source   // linear data structure from which elements will be copied
):

/*
This constructor instanciates a stack and copies the contents of "source" to the new stack
object.  The copy is deep -- that is, each element in "source" is copied -- so that the new
stack's structure is independent of "source's" structure.

Refer to the "operator+=()" method for information how the copy is performed.

PRECONDITIONS:
None.

POSTCONDITIONS:
A new stack is created.  Its contents are a copy of "source's".
*/

  SLinearStruct<T>(size)

{
  operator+=(source);
  return;
}

/*********************************************************************************************/

template <class T> SStack<T>::SStack
(
  const unsigned int size,           // the no. of elements that the stack must be able to hold
  const T *const     elements,       // the array of elements to copy into the stack
  const unsigned int numElements     // the no. of elements in the array
):

/*
This constructor creates a new stack that contains copies of the elements in the array
"elements".  The elements in the array are pushed onto the stack in the order in which they
appear -- in other words, the first element in the array will be the last element to be popped
off of the stack.

PRECONDITIONS:
"numElements" must be less than or equal to "size".  Also, if "numElements" is zero then
"elements" must be NULL (and conversely).

POSTCONDITIONS:
A new stack is created.  Its contents are copies of the elements passed as arguments.
*/

  SLinearStruct<T>(size)

{
  if (numElements > size)
  {
    ExceptionErrno::presetDetails() << "\"numElements\" (" << numElements
      << ") can't be larger than \"size\" (" << size << ").";
    throw ExceptionErrno(EINVAL, __FILE__, __LINE__);
  }

  if ((numElements == 0) != (elements == NULL))
  {
    ExceptionErrno::presetDetails() << "\"numElements\" is " << numElements
      << ", so \"elements\" (0x" << hex << setw(8) << setfill('0') << (void*)elements
      << ") can't be " << (numElements == 0 ? "non-NULL." : "NULL.");
    throw ExceptionErrno(EINVAL, __FILE__, __LINE__);
  }

  try
  {
    /*
    A simple loop is used to push each element in the array onto the stack (in the order in
    which they're given).
    */

    while (_numElements < numElements)
    {
      _elements[_numElements] = elements[_numElements];
      ++_numElements;
    }

    assertInvariants();
  }
  catch (...)
  {
    throw OperationFailed("Assignment operation on a \"T\" element failed.", __FILE__,
      __LINE__);
  }

  return;
}

/*********************************************************************************************/

template <class T> SStack<T>::SStack
(
  const unsigned int size,           // the no. of elements that the stack must be able to hold
  const unsigned int numElements,    // the no. of elements in the parameter list
  ...                                // the elements to be pushed onto the stack (in order)
):

/*
This constructor creates a new stack that contains copies of the elements passed in the
argument list.  The elements in the argument list are pushed onto the stack in the order in
which they appear -- in other words, the first element in the argument list will be the last
element to be popped from the stack.

WARNING:  due to type promotion issues, "T" cannot be "char", "unsigned char" or "float".

PRECONDITIONS:
"numElements" must be less than or equal to "size".

POSTCONDITIONS:
A new stack is created.  Its contents are copies of the elements passed as arguments.
*/

  SLinearStruct<T>(size)

{
  if (numElements > size)
  {
    ExceptionErrno::presetDetails() << "\"numElements\" (" << numElements
      << ") can't be larger than \"size\" (" << size << ")";
    throw ExceptionErrno(EINVAL, __FILE__, __LINE__);
  }

  if (numElements > 0U)
  {
    try
    {
      va_list argList;                                        // for managing the argument list

      /*
      A simple loop is used to push each element argument onto the stack (in the order in which
      they're given).
      */

      va_start(argList, numElements);

      while (_numElements < numElements)
        _elements[_numElements++] = va_arg(argList, T);

      va_end(argList);

      assertInvariants();
    }
    catch (...)
    {
      throw OperationFailed("Element assignment operation failed.", __FILE__, __LINE__);
    }
  }

  return;
}

/*********************************************************************************************/

template <class T> void SStack<T>::empty() throw ()

/*
This method ensures that there are no elements in the stack.

PRECONDITIONS:

None.

POSTCONDITIONS:

The stack is empty.
*/

{
  assertInvariants();

  _numElements = 0U;

  assertInvariants();
  return;
}

/*********************************************************************************************/

template <class T> void SStack<T>::iterStart() const throw ()
{
  assertInvariants();

  *_iterCurrent = _numElements;
  return;
}

/*********************************************************************************************/

template <class T> const bool SStack<T>::iterMore() const throw ()
{
  assertInvariants();

  return (*_iterCurrent > 0U);
}

/*********************************************************************************************/

template <class T> void SStack<T>::iterNext() const throw (DataStruct::OperationFailed)
{
  assertInvariants();

  if (*_iterCurrent == 0U)
    throw OperationFailed("Current iteration element is undefined.", __FILE__, __LINE__);

  --(*_iterCurrent);
  return;
}

/*********************************************************************************************/

template <class T> const T *const SStack<T>::iterCurrent() const
  throw (DataStruct::OperationFailed)
{
  assertInvariants();

  if (*_iterCurrent == 0U)
    throw OperationFailed("Current iteration element is undefined.", __FILE__, __LINE__);

  return &(_elements[*_iterCurrent - 1U]);
}
/*********************************************************************************************/

template <class T> void SStack<T>::push
(
  const T& elementToPush                               // the element to be placed on the stack
)

/*
This method pushes a copy of "elementToPush" onto the stack.

PRECONDITIONS:
The stack cannot be full.

POSTCONDITIONS:
The copy of "elementToPush" will be added at the top of the stack and will be the first element
to be popped off.
*/

{
  assertInvariants();

  if (_numElements == _maxElements)
    throw Full(__FILE__, __LINE__);

  _elements[_numElements] = elementToPush;
  ++_numElements;

  assertInvariants();
  return;
}

/*********************************************************************************************/

template <class T> void SStack<T>::pop
(
  T& poppedElement                                // the variable to receive the popped element
)

/*
This method pops the topmost element off of the stack and copies it to "poppedElement".

PRECONDITIONS:
The stack cannot be empty.

POSTCONDITIONS:
The next element to be popped off of the stack is copied to "poppedElement" and removed from
the stack.
*/

{
  assertInvariants();

  if (_numElements == 0U)
    throw Empty(__FILE__, __LINE__);

  poppedElement = _elements[_numElements - 1U];
  --_numElements;

  assertInvariants();
  return;
}

/*********************************************************************************************/

template <class T> void SStack<T>::peek
(
  T& elementToBePopped                 // the variable to receive the next element to be popped
)
const

/*
This method retrieves the topmost element from the stack (without popping it off) and copies it
to "elementToBePopped".

PRECONDITIONS:
The stack cannot be empty.

POSTCONDITIONS:
The next element to be popped off of the stack is copied to "elementToBePopped".
*/

{
  assertInvariants();

  if (_numElements == 0U)
    throw Empty(__FILE__, __LINE__);

  elementToBePopped = _elements[_numElements - 1U];
  return;
}

/*********************************************************************************************/

template <class T> SStack<T>& SStack<T>::operator=
(
  const LinearStruct<T>& source                       // the source data structure to copy from
)

{
  assertInvariants();

  /*
  Code re-use at its finest...
  */

  _numElements = 0U;
  operator+=(source);

  return *this;
}

/*********************************************************************************************/

template <class T> SStack<T>& SStack<T>::operator+=
(
  const LinearStruct<T>& source                       // the source data structure to copy from
)

/*
This method appends the contents of "source" to the instance.  The copy is deep -- that is,
each element in "source" is copied -- so that the new stack's structure is independent of
"source's" structure.

The first element in "source's" iteration order will be the first element to be popped off of
the stack.

PRECONDITIONS:
There must be enough room in the instance's stack to hold the copies of "source's" elements.

POSTCONDITIONS:
"source's" elements are appended to the instance's elements.
*/

{
  assertInvariants();

  if (source.numElements() > (_maxElements - _numElements))
  {
    OperationFailed::presetDetails() << "\"source\" contains " << source.numElements()
      << " but there's only room for " << (_maxElements - _numElements) << " elements.";
    throw OperationFailed("Assignment operation would cause an overflow.", __FILE__, __LINE__);
  }

  if (source.numElements() > 0U)
  {
    try
    {
      /*
      Simply pushing each element in "source" onto the stack in turn would put them in the
      wrong order.  "_numElements" must be increased by the number of elements that will be
      added and the elements in "_elements" must be filled in from the top down.
      */

      unsigned int newHead = _numElements + source.numElements();

      for (source.iterStart(); source.iterMore(); source.iterNext())
      {
        assert(source.iterCurrent() != NULL);
        _elements[--newHead] = *source.iterCurrent();
      }

      assert(newHead == _numElements);

      _numElements += source.numElements();

      assertInvariants();
    }
    catch (...)
    {
      throw OperationFailed("Element assignment operation failed.", __FILE__, __LINE__);
    }
  }

  return *this;
}

/*********************************************************************************************/

template <class T> SStack<T> SStack<T>::operator+
(
  const LinearStruct<T>& rhs                          // the source data structure to copy from
)

{
  /*
  Code re-use at its finest...
  */

  return (SStack<T>(_maxElements + rhs.numElements(), *this) += rhs);
}

#endif
