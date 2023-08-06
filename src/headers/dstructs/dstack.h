#ifndef DSTRUCTS_DSTACK_H
#define DSTRUCTS_DSTACK_H

// ============================================================================================
//
// DSTACK_H -- Implementation of a dynamic stack -- that is, a stack that stores its elements
// in dynamicly-allocated memory.
//
// ============================================================================================

/*
This class is a dynamic stack.  A "DStack" is a "Stack".

The structure of a "DStack" object looks like this:

Node 3           Node 2           Node 1           Node 0
-------------    -------------    -------------    -------------
| Element   |    | Element   |    | Element   |    | Element   |
| Next node |--->| Next node |--->| Next node |--->| Next node |--->NULL
-------------    -------------    -------------    -------------
       ^
       |
_top ---

When a new element is added to this structure, a "Node" object is created and set to point to
the "Node" currently pointed to by "_top" (in this case, "Node 3"); "_top" is then set to point
to the new "Node".

Similarly, when an element is removed from this structure, "_top" is set to point to the "Node"
pointed to by the "Node" that "_top" currently points to (in this case, "Node 2"); the element
from the topmost "Node" (in this case, "Node 3") is then extracted and the "Node" is destroyed.
*/

// ============================================================================================
// INCLUDE FILE
// ============================================================================================

#ifdef FAT_FILENAMES
  #include <dstructs/dlinearst.h>
#else
  #include <dstructs/dlinearstructure.h>
#endif

#include <dstructs/stack.h>

// ============================================================================================
// CLASS DECLARATION
// ============================================================================================

template<class T> class DStack:
  virtual public DataStructureExceptions,
  virtual public DLinearStructure<T>,
  virtual public Stack<T>
{
  public:
                 DStack()
                   {return;}
                 DStack(const DataStructure<T>& source):
                   DLinearStructure(source) {return;}
                 DStack(const unsigned int, ...);

    DStack<T>&   operator=(const DataStructure<T>&)  throw (Full, OperationFailed);
    DStack<T>&   operator+=(const DataStructure<T>&) throw (Full, OperationFailed);

    // Stack virtual methods

    virtual void push(const T&) throw (Full, OperationFailed);
    virtual void pop(T&)        throw (Empty, OperationFailed);
    virtual void peek(T&) const throw (Empty, OperationFailed);
};

// ============================================================================================
// METHOD DEFINITIONS:
// ============================================================================================

/*********************************************************************************************/

template<class T> DStack<T>::DStack
(
  const unsigned int numElements,         // the # of elements in the parameter list
  ...                               // the elements to be pushed onto the stack
)

/*
This constructor creates a new stack that contains copies of the elements passed in the
parameter list.  The elements in the parameter list are pushed onto the stack in the order in
which they appear -- in other words, the first element in the parameter list will be the last
element to be popped from the stack.

Note:  due to type promotion issues, "T" cannot be "char", "unsigned char" or "float".

"LinearStructure::CannotCreate" is thrown iff memory can't be allocated for the stack space and
iterator.

PRECONDITIONS:
"numElements" must be less than or equal to "size".

POSTCONDITIONS:
A new stack is created.  Its contents are copies of the elements passed in the parameter list.
*/

{
  va_list argList;                            // for managing the argument list

  /*
  A simple loop is used to push each argument in turn onto the stack.
  */

  va_start(argList, numElements);
  initializeVariableArguments(numElements, argList);
  va_end(argList);

  assertInvariants();
  return;
}

/*********************************************************************************************/

template<class T> void DStack<T>::push
(
  const T& elementToPush                  // the element to be placed on the stack
)
throw (DataStructureExceptions::Full, DataStructureExceptions::OperationFailed)

/*
This method pushes a copy of "elementToPush" onto the stack.

PRECONDITIONS:
The stack cannot be full.

POSTCONDITIONS:
The copy of "elementToPush" will be added at the top of the stack and will be the first element
to be popped off.
*/

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  Node* newNode;

  try
  {
    newNode = new Node(elementToPush, _first);
  }
  catch (...)
  {
    throw OperationFailed("Unable to add an element to a DStack.", __FILE__, __LINE__);
  }

  if (newNode == NULL)
    throw Full(__FILE__, __LINE__);

  _first = newNode;

  if (_last == NULL)
    _last = newNode;

  ++_numElements;
  return;
}

/*********************************************************************************************/

template<class T> void DStack<T>::pop
(
  T& poppedElement                      // the variable to receive the popped element
)
throw (DataStructureExceptions::Empty, DataStructureExceptions::OperationFailed)

/*
This method pops the topmost element off of the stack and copies it to "poppedElement".

PRECONDITIONS:
The stack cannot be empty.

POSTCONDITIONS:
The next element to be popped off of the stack is copied to "poppedElement" and removed from
the stack.
*/

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  if (_first == NULL)
    throw Empty(__FILE__, __LINE__);

  Node* nodeToRemove(_first);

  try
  {
    poppedElement = *(_first->element());
  }
  catch (...)
  {
    throw OperationFailed("Unable to remove an element from a DStack.", __FILE__, __LINE__);
  }

  _first = _first->next();

  if (_first == NULL)
    _last = NULL;

  delete nodeToRemove;
  --_numElements;

  return;
}

/*********************************************************************************************/

template<class T> void DStack<T>::peek
(
  T& elementToBePopped                 // the variable to receive the next element to be popped
)
const throw (DataStructureExceptions::Empty, DataStructureExceptions::OperationFailed)

/*
This method retrieves the topmost element from the stack (without popping it off) and copies it
to "elementToBePopped".

PRECONDITIONS:
The stack cannot be empty.

POSTCONDITIONS:
The next element to be popped off of the stack is copied to "elementToBePopped".
*/

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  if (_first == NULL)
    throw Empty(__FILE__, __LINE__);

  try
  {
    elementToBePopped = *(_first->element());
  }
  catch (...)
  {
    throw OperationFailed("Unable to copy an element from a DStack.", __FILE__, __LINE__);
  }

  return;
}

/*********************************************************************************************/

template<class T> DStack<T>& DStack<T>::operator=
(
  const DataStructure<T>& source                      // the source data structure to copy from
)
throw (DataStructureExceptions::Full, DataStructureExceptions::OperationFailed)

{
  empty();
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template<class T> DStack<T>& DStack<T>::operator+=
(
  const DataStructure<T>& source                      // the source data structure to copy from
)
throw (DataStructureExceptions::Full, DataStructureExceptions::OperationFailed)

{
  concatenate(source);
  return *this;
}

// ============================================================================================
// MEANINGFUL OPERATORS
// ============================================================================================

/*********************************************************************************************/

template<class T> DStack<T>& operator+
(
  const DStack<T>& lhs,                             // the source data structure to copy from
  const DataStructure<T>& rhs                       // the source data structure to copy from
)
throw (DataStructureExceptions::Full, DataStructureExceptions::OperationFailed)

{
  return DStack<T>(lhs) += rhs;
}

#endif
