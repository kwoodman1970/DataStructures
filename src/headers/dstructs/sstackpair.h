#ifndef SSTACKPAIR_H
#define SSTACKPAIR_H

// ============================================================================
// DESIGN NOTES:
// ============================================================================

/*
The structure of an "SStackPair" object looks like this:

   0:        1:        2:              _size-3:  _size-2:  _size-1:
  --------------------------------   --------------------------------
  | Element | Element | Element | ... | Element | Element | Element |
  --------------------------------   --------------------------------
		       ^               ^
		       |               |
_top -------------------               |
				       |
_top1 ----------------------------------

There are two conceptual stacks here:  stack #0 and stack #1.

When a new element is added to stack #0, it's copied to the cell indicated by
"_top" (in this case, cell 2); "_top" is then set to point to the next cell (in
this case, cell 3).

Similarly, when an element is removed from stack #0, "_top" is set to the
previous cell (in the above example, cell 1).  If "_top" is 0 then that means
that stack #0 is empty.

When a new element is added to stack #1, "_top1" is set to point to the
previous cell (in this case, cell _size - 4); the new element is then copied to
that cell.

Similarly, when an element is removed from stack #1, "_top1" is set to the
next cell (in the above example, cell _size - 2).  If "_top1" is "_size" then
that means that stack #1 is empty.

If "_top" and "_top1" should ever become equal then that means that both stacks
are full.
*/

// ============================================================================
// INCLUDE FILES:
// ============================================================================

#include "sstack.h"

// ============================================================================
// CLASS DECLARATIONS:
// ============================================================================

template <class T> class SStackPair:
  public SStack<T>
 {
  public:
    class BadSelection {};         // exception:  an unknown stack was selected

			   SStackPair(const size_t);
			   SStackPair(const size_t, const LinearStruct<T>&,
                             const LinearStruct<T>&);
			   SStackPair(const size_t, SStackPair<T>&);
			   SStackPair(const size_t, const size_t, const size_t,
                             ...);
    virtual void           push(const T&)
    virtual void           pop(T&);
    virtual unsigned int   isEmpty() const;
    virtual unsigned int   isFull() const;
    virtual size_t         numElements() const;
    virtual SStackPair<T>& operator=(const LinearStruct<T>&);
    virtual void           select(const unsigned int);
    virtual unsigned int   selected() const
                             {checkInvariants(); return _selectedStack;}
  protected:
    virtual void iterStart() const;
    virtual int  iterMore() const;
    virtual void iterNext() const;
    virtual T&   iterElement() const;
  private:
    size_t       _top1;          // index of the topmost element of stack 1
    unsigned int _selectedStack  // the stack to refer to

                SStackPair();
    inline void checkInvariants() const;
 };

// ============================================================================
// METHOD DEFINITIONS:
// ============================================================================

template <class T> SStackPair<T>::SStackPair
 (
  const size_t size                    // # of elements that the stack can hold
 ):

/*
This constructor instanciates an empty pair of stacks that can contain "size"
elements.

PRECONDITIONS:
"size" must be greater than zero.

POSTCONDITIONS:
An empty stack with room for "size" elements is created.  Stack #0 will be the
curently selected stack.
*/

  SStack<T>(size)
 {
  _top1 = size;
  _selectedStack = 0;
  checkInvariants();
  return;
 }

/*****************************************************************************/

template <class T> SStackPair<T>::SStackPair
 (
  const size_t           size,         // # of elements that the stack can hold
  const LinearStruct<T>& source0,      // the Stack to be copied into stack #0
  const LinearStruct<T>& source1       // the Stack to be copied into stack #1
 ):

/*
This constructor instanciates a stack and copies the contents of "source0" and
"source1" to the new stack object.  The copy is deep -- that is, each element
in "source0" and "source1" is copied -- so that the new stack's structures are
independent of "source0" and "source1's" structures.

PRECONDITIONS:
"size" must be greater than zero.

POSTCONDITIONS:
A new pair of stacks are created.  Their contents are a copy of "source0" and
"source1's".  Stack #0 will be the currently selected stack
*/

  SStack<T>(size, source0)
 {
  _top1 = size;
  _selectedStack = 1;
  operator=(source1);
  _selectedStack = 0;
  checkInvariants();
  return;
 }

/*****************************************************************************/

template <class T> SStackPair<T>::SStackPair
 (
  const size_t   size,    // # of elements that the stack can hold
  SStackPair<T>& source   // that SStackPair from which elements will be copied
 ):

/*
This constructor instanciates a SStackPair and copies the contents of "source"
to the new SStackPair object.  The copy is deep -- that is, each element in
"source" is copied -- so that the new SStackPair's array is independent of
"source's" array.

PRECONDITIONS:
"size" must be greater than zero.

POSTCONDITIONS:
A new pair of stacks are created.  Their contents are a copy of "source's".
Stack #0 will be the currently selected stack.
*/

  SStack<T>(size)
 {
  unsigned int originalStack = source.selected();

  _top1 = size;
  _selectedStack = 1;
  source.select(1)
  operator=(source);
  _selectedStack = 0;
  source.select(0);
  operator=(source);
  source.select(originalStack);
  checkInvariants();
  return;
 }

/*****************************************************************************/

template <class T> SStackPair<T>::SStackPair
 (
  const size_t size,             // # of elements that the stack can hold
  const size_t numElements0,     // # of elements in the parameter list for the
                                 //   the first stack
  const size_t numElements1,     // # of elements in the parameter list for the
				 //   second stack
  ...                            // the elements to be pushed onto the stacks
 ):

/*
This constructor creates a new pair of stacks that contain copies of the
elements passed in the parameter list.  The elements in the parameter list are
pushed onto each stack in the order in which they appear -- in other words, the
first element in the parameter list will be the last element to be popped from
the first stack and the last element in the parameter list will be the first
element to be poped from the second stack.

"T" cannot be "char", "unsigned char" or "float".

PRECONDITIONS:
"size" must be greater than zero.

POSTCONDITIONS:
A new pair of stacks are created.  Their contents are copies of the elements
passed in the parameter list.  Stack #0 will be the currently selected stack.
*/

  SStack<T>(size)
 {
  va_list argList;
  size_t  element;

  _top1 = size;
  _selectedStack = 0;
  va_start(argList, numElements1);
  for (element = 0; element < numElements0; element++)
    push(va_arg(argList, T));
  _selectedStack = 1;
  for (element = 0; element < numElements1; element++)
    push(va_arg(argList, T));
  _selectedStack = 0;
  va_end(argList);
  checkInvariants();
  return;
 }

/*****************************************************************************/

template <class T> void SStackPair<T>::push
 (
  const T& newElement                  // the element to be placed on the stack
 )

/*
This method pushes a copy of "newElement" onto the currently selected stack.

PRECONDITIONS:
The pair of stacks cannot be full.

POSTCONDITIONS:
The copy of "newElement" will be placed at the top of the currently selected
stack and will be the first element to be popped off.
*/

 {
  checkInvariants();
  if (isFull()) throw Full();
  if (_selectedStack)
    _stackSpace[--_top1] = newElement;
  else
    SStack<T>::push(newElement);
  checkInvariants();
  return;
 }

/*****************************************************************************/

template <class T> void SStackPair<T>::pop
 (
  T& element                      // the variable to receive the popped element
 )

/*
This method pops the topmost element off of the currently selected stack and
copies it to "element".

PRECONDITIONS:
The currently selected stack cannot be empty.

POSTCONDITIONS:
The next element to be popped off of the currently selected stack is copied to
"element".
*/

 {
  checkInvariants();
  if (isEmpty()) thow Empty();
  if (_selectedStack)
    element = _stackSpace[_top1++];
  else
    SStack<T>::pop(element);
  checkInvariants();
  return;
 }

/*****************************************************************************/

template <class T> int SStackPair<T>::isEmpty() const

/*
This method returns a non-zero value if there are no elements on the currently
selected stack and zero if there is at least one element on the currently
selected stack.

PRECONDITIONS:
None.

POSTCONDITIONS:
None.
*/

 {
  checkInvariants();
  return (_selectedStack ? _top1 == size : SStack<T>::isEmpty());
 }

/*****************************************************************************/

template <class T> int SStackPair<T>::isFull() const

/*
This method returns a non-zero value if there is no more room on either stack
and zero if there is room for at least one more element on either stack.

PRECONDITIONS:
None.

POSTCONDITIONS:
None.
*/

 {
  checkInvariants();
  return (_top == _top1);
 }

/*****************************************************************************/

template <class T> size_t SStackPair<T>::numElements() const

/*
This method returns the number of elements in the currently selected stack.

PRECONDITIONS:
None.

POSTCONDITIONS:
None.
*/

 {
  return (_selectedStack ? _size - _top1 : SStack<T>::numElements());
 }

/*****************************************************************************/

template <class T> SStackPair<T>& SStackPair<T>::operator=
 (
  const LinearStruct<T>& source     // the source linear structure to copy from
 )

/*
This operator copies the contents of "source" to the currently selected stack.
The copy is deep -- that is, each element in "source" is copied -- so that the
currently selected stack's structure is independent of "source's" structure.

PRECONDITIONS:
There must be enough room in the currently selected stack to hold the copies of
"source's" elements.

POSTCONDITIONS:
The contents of the currently selected stack are discarded and replaced by
copies of "source's" contents.
*/

 {
  checkInvariants();
  if ((_selectedStack ? _size - _top : _top1) < source.numElements())
    throw Full();
  if (_selectedStack)
    _top1 = _size - source.numElements();
    for(source.iterStart(); source.iterMore(); source.iterNext())
      _stackSpace[_top1++] = source.iterElement();
    _top1 = _size - source.numElements();
  else
    SStack<T>::operator=(source);
  return *this;
 }

/*****************************************************************************/

template <class T> SStackPair<T>& SStackPair<T>::operator=
 (
  SStackPair<T>& source                   // the source SStackPair to copy from
 )

/*
This operator copies the contents of "source" to the two stacks.  The copy is
deep -- that is, each element in "source" is copied -- so that the currently
selected stack's structure is independent of "source's" structure.

PRECONDITIONS:
There must be enough room in the stack pair to hold the copies of "source's"
elements.

POSTCONDITIONS:
The contents of both stacks are discarded and replaced by copies of "source's"
contents.
*/

 {
  const unsigned int lastSelected = _selected;

  checkInvariants();
  if (source._top + (source._size - source._top1) > _size) throw Full();
  _selected = source.selected();
  operator=((linearStruct)source);
  _selected = (_selected ? 0 : 1);
  source.select(_selected);
  operator=((linearStruct)source);
  source.select(_selected ? 0 : 1);
  _select = lastSelected;
  checkInvariants();
  return *this;
 }

/*****************************************************************************/

template <class T> void SStackPair<T>::select
 (
  const unsigned int newStack
 )

/*
This method directs the object to now refer to the stack indicated by
"newStack" (0 or 1).

PRECONDITIONS:
None.

POSTCONDITIONS:
The stack indicated by "newStack" will now be referenced whenever a stack
operation is requested  This stays in effect until "select()" is called again
or the object is destroyed.
*/

 {
  checkInvariants();
  if (_selectedStack != 0 && _selectedStack != 1) throw BadSelection();
  _selectedStack = newStack;
  checkInvariants();
  return;
 }

/*****************************************************************************/

template <class T> void SStackPair<T>::iterStart() const

/*
This method initializes the iteration control.  The order of the iteration is
topmost element to bottommost element.

DANGER!  Do not attempt to switch stacks in the middle of an iteration.

PRECONDITIONS:
None.

POSTCONDITIONS:
The iteration control has been initialized.
*/

 {
  checkInvariants();
  if (_selectedStack)
    *_iterator = _top1;
  else
    SStack<T>::iterStart();
  return;
 }

/*****************************************************************************/

template <class T> int SStackPair<T>::iterMore() const

/*
This method returns a non-zero value if there are more elements to iterate
through and zero if there aren't.

DANGER!  Do not attempt to switch stacks in the middle of an iteration.

PRECONDITIONS:
The iteration control must have been initialized.

POSTCONDITIONS:
None.
*/

 {
  checkInvariants();
  return (selectedStack ? *_iterator < _size : SStack<T>::iterMore());
 }

/*****************************************************************************/

template <class T> void SStackPair<T>::iterNext() const

/*
This method sets the iteration control to the next element to be iterated
through.  The order of the iteration is topmost element to bottommost element.

DANGER!  Do not attempt to switch stacks in the middle of an iteration.

PRECONDITIONS:
The iteration control must have been initialized.

POSTCONDITIONS:
The iteration control is set to the next element to be iterated through.
*/

 {
  checkInvariants();
  if (_selectedStack)
    (*_iterator)++;
  else
    SStack<T>::iterNext();
  return;
 }

/*****************************************************************************/

template <class T> T& SStackPair<T>::iterElement() const

/*
This method returns a pointer to the current element being iterated through

DANGER!  Do not attempt to switch stacks in the middle of an iteration.

PRECONDITIONS:
The iteration control must have been initialized.

POSTCONDITIONS:
None.
*/

 {
  checkInvariants();
  return (_selectedStack ? _stackSpace[*_iterator] : SStack<T>::iterElement());
 }

/*****************************************************************************/

template <class T> inline void SStackPair<T>::checkInvariants() const

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
  SStack::checkInvariants();
  /*
  INVARIANT:  "_top1" must be greater than or equal to 0 and less than or equal
  to "_size".
  */
  assert(_top1 >= 0 && _top1 <= _size);
  /*
  INVARIANT:  Neither stack can invade the other stack's space.
  */
  assert(_top < _top1);
  /*
  INVARAINT:  "_selectedStack" must be either 0 or 1.
  */
  assert(_selectedStack == 0 || _selectedStack == 1);
  return;
 }

