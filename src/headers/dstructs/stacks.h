#ifndef STACKS_H
#define STACKS_H

// ============================================================================
// INCLUDE FILES:
// ============================================================================

#include <stdlib.h>
#include <stdarg.h>
#include "linearstruct.h"
#include "nodes.h"

// ============================================================================
// CLASS DECLARATIONS:
// ============================================================================

template <class T> class Stack:

/*
This class is a base class for stack implementations.  Elements placed in a
"Stack" must have a default constructor, a destructor and a copy operator.
*/

  class LinearStruct<T>
 {
  public:
    virtual void         push(const T&) = 0;              // push an element on
    virtual void         pop(T&) = 0;                     // pop an element off
    virtual int          isEmpty() const = 0;             // is stack empty?
    virtual int          isFull() const = 0;              // is stack full?
    virtual const size_t numElements() const = 0;         // how many elements?
    virtual Stack<T>&    operator<<(const T& element)     // push an element on
                           {push(element); return this*}
    virtual Stack<T>&    operator>>(T& element)           // pop an element off
                           {pop(element); return this*)
 };

/*****************************************************************************/

template <class T> class DStack:

/*
This class represents a stack that uses dynamicly-allocated memory to store its
elements.
*/

  public Stack<T>
 {
  public:
		       DStack();
		       DStack(const LinearStruct<T>&);
		       DStack(const unsigned int, ...);
    virtual            ~DStack();
    virtual void       push(const T&)
    virtual void       pop(T&);
    virtual int        isEmpty() const
                         {return (_top ? 0 : 1);}
    virtual int        isFull() const;
    virtual size_t     numElements() const
                         {return _counter;}
    virtual DStack<T>& operator=(const LinearStruct<T>&);
  protected:
    virtual void iterStart() const
                   {*_iterator = _top; return;}
    virtual int  iterMore() const;
                   {return (*_iterator ? 1 : 0);}
    virtual void iterNext() const
                   {*_iterator = (*_iterator)->next(); return;}
    virtual T&   iterElement() const;
                   {return (*_iterator)->element();}
  private:
    Node<T>*        _top;       // pointer to the Node with the topmost element
    Node<T>** const _iterater;  // pointer to the pointer to the current Node
                                //   in the iteration
    size_t          _counter;   // # of elements in the stack
 };

/*****************************************************************************/

template <class T, size_t _size> class SStack:

/*
This class represents a stack that uses staticly-allocated memory to store its
elements.
*/

  public Stack<T>
 {
  public:
		       SStack();
		       SStack(const LinearStruct<T>&);
		       SStack(const unsigned int, ...);
    virtual            ~SStack();
    virtual void       push(const T&)
    virtual void       pop(T&);
    virtual int        isEmpty() const
                         {return (_top == 0 ? 1 : 0);}
    virtual int        isFull() const
                         {return (_top >= _size ? 1 : 0);}
    virtual size_t     numElements() const
                         {return _top;}
    virtual SStack<T>& operator=(const LinearStruct<T>&);
  protected:
    virtual void iterStart() const
                   {*_iterator = 0; return;}
    virtual int  iterMore() const
                   {return (*_iterator < _top ? 1 : 0);}
    virtual void iterNext() const
                   {_iterator++; return;}
    virtual T&   iterElement() const;
                   {return _stackSpace[_iterator];}
  private:
    T       _stackSpace[_size];  // array of elements
    size_t  _top;                // index of the next unused element
    size_t* _iterator;           // pointer to an index to the current element
                                 //   in the iteration
    inline void checkInvariants() const;
 };

/*****************************************************************************/

template <class T, size_t _size> class SStackPair:

/*
This class represents a pair of stacks that use a common region of staticly-
allocated memory to store their elements.  Stack #0 grows from one end of the
available space and stack #1 grows from the other end.  The two stacks will not
overlap.

A maximum of "SIZE" - 1 elements can be placed in the two stacks.  One
element's worth of available space is used as a separator between the two
stacks.
*/

  public Stack<T>
 {
  public:
				  SStackPair();
				  SStackPair(const Stack<T>&, const Stack<T>&);
				  SStackPair(SStackPair<T, SIZE>&);
				  SStackPair(const unsigned int,
				    const unsigned int, ...);
    virtual                       ~SStackPair();
    virtual void                  push(const T&)
    virtual void                  pop(T&);
    virtual int                   isEmpty() const;
    virtual int                   isFull() const;
    virtual size_t                numElements() const;
    virtual SStackPair<T, SIZE>&  operator=(const Stack<T>&);
    virtual void                  select(const unsigned int);
    virtual unsigned int          selected() const;
  protected:
    virtual void                  iterStart() const;
    virtual int                   iterMore() const;
    virtual void                  iterNext() const;
    virtual T*                    iterElement() const;
  private:
    T                             _stackSpace[SIZE];  // array of elements
    size_t                        _top0;              // index of the next
						      //   unused element of
						      //   stack #0
    size_t                        _top1;              // index of the next
						      //   unused element of
						      //   stack #1
    unsigned int                  _selectedStack      // the stack to refer to
    size_t*                       _iterControl;       // pointer to an index to
						      //   the predecessor of
						      //   the current element
						      //   in the iteration
    inline void                   checkInvariants() const; };

// ============================================================================
// INLINE METHOD DEFINITIONS FOR SSTACK:
// ============================================================================

/*****************************************************************************/

template <class T, size_t SIZE> inline void SStack<T, SIZE>::checkInvariants()
  const

/*
This method asserts the "SStack" invariants.  If an invariant has been broken
then the stack has been corrupted somehow and the program should halt before
any real damage can be done.

PRECONDITIONS:
None.

POSTCONDITIONS:
None.
*/

 {
  /*
  INVARIANT:  "_top" must be less than or equal to "SIZE".
  */
  assert(_top <= SIZE);
  /*
  INVARIANT:  "_iterControl" cannot be NULL -- it must point to something.
  */
  assert(_iterControl);
  return;
 }

// ============================================================================
// INLINE METHOD DEFINITIONS FOR SSTACKPAIR:
// ============================================================================

/*****************************************************************************/

template <class T, size_t SIZE> inline void
  SStackPair<T, SIZE>::checkInvariants() const

/*
This method asserts the "SStackPair" invariants.  If an invariant has been
broken then the stack has been corrupted somehow and the program should halt
before any real damage can be done.

PRECONDITIONS:
None.

POSTCONDITIONS:
None.
*/

 {
  /*
  INVARIANT:  "_top0" must be less than "SIZE".
  */
  assert(_top0 < SIZE);
  /*
  INVARIANT:  "_top1" must be less than "SIZE".
  */
  assert(_top1 < SIZE);
  /*
  INVARIANT:  Neither stack can invade the other stack's space.
  */
  assert(_top0 >= _top1);
  /*
  INVARIANT:  "_iterControl" cannot be NULL -- it must point to something.
  */
  assert(_iterControl);
  return;
 }

#endif
