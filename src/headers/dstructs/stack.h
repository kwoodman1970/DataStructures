#ifndef DSTRUCTS_STACK_H
#define DSTRUCTS_STACK_H

// ============================================================================================
//
// stack.h -- Stack Base Class
//
// ============================================================================================

/*
This class is a base class for stack implementations.  A "Stack" is a "LinearStructure".

For iterations, the topmost element is considered to be the first element in the iteration.
*/

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#ifdef FAT_FILENAMES
  #include <dstructs/linearst.h>
#else
  #include <dstructs/linearstructure.h>
#endif

#ifdef _WINSOCKAPI_
  #error winsock.h got included somewhere
#endif

// ============================================================================================
// CLASS DECLARATION
// ============================================================================================

template<class T> class Stack:
  virtual public DataStructureExceptions,
  virtual public LinearStructure<T>
{
  public:

    /*
    There are three public virtual methods:  "push()", "pop()" and "peek()".  "push()" and
    "pop()" must be defined as the operations that add and remove elements to and from the
    stack, respectively, in last-in-first-out order.  "peek()" must be defined as an operation
    that gets the next element to be popped off of the stack without actually popping it off.
    */

    virtual void     push(const T&)       throw (Full, OperationFailed)  = 0;
    virtual void     pop(T&)              throw (Empty, OperationFailed) = 0;
    virtual void     peek(T&) const       throw (Empty, OperationFailed) = 0;

    inline Stack<T>& operator<<(const T&) throw (Full, OperationFailed);
    inline Stack<T>& operator>>(T&)       throw (Empty, OperationFailed);
};

// ============================================================================================
// CONVENIENCE OPERATORS
// ============================================================================================

/*********************************************************************************************/

template<class T> inline Stack<T>& Stack<T>::operator<<
(
  const T& elementToPush
)
throw (DataStructureExceptions::Full, DataStructureExceptions::OperationFailed)

/*
This function makes the shift-in operator work like a "Stack<T>'s" "push()" method and thus
allows several elements to be pushed onto a "Stack<T>" on a single line (like with streams).
For example (with apologies & respects to Dr. Seuss):

  // The usual way:

  aStack.push(oneFish);
  aStack.push(twoFish);
  aStack.push(redFish);
  aStack.push(blueFish);

  // The shift-in way:

  aStack << oneFish << twoFish << redFish << blueFish;

PRECONDITIONS:
Same as for "push()" for descendents of "Stack<T>".

POSTCONDITIONS:
Same as for "push()" for descendents of "Stack<T>".
*/

{
  push(elementToPush);
  return *this;
}

/*********************************************************************************************/

template<class T> inline Stack<T>& Stack<T>::operator>>
(
  T& poppedElement
)
throw (DataStructureExceptions::Empty, DataStructureExceptions::OperationFailed)

/*
This function makes the shift-out operator work like a "Stack<T>'s" "pop()" method and thus
allows several elements to be popped off of a "Stack<T>" on a single line (like with streams).
For example:

  // The usual way:

  aStack.pop(first);
  aStack.pop(second);
  aStack.pop(third);
  aStack.pop(last);

  // The shift-out way:

  aStack >> first >> second >> third >> last;

PRECONDITIONS:
Same as for "pop()" for descendents of "Stack<T>".

POSTCONDITIONS:
Same as for "pop()" for descendents of "Stack<T>".
*/

{
  pop(poppedElement);
  return *this;
}

#endif