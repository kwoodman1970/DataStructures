#ifndef LISTS_H
#define LISTS_H

// ============================================================================================
// INCLUDE FILES:
// ============================================================================================

#include <stdarg.h>
#include "nodes.h"

// ============================================================================================
// CLASS DECLARATIONS:
// ============================================================================================

/*****************************************************************************/

template <class T> class List
 {
  public:
    virtual              ~List()                               {return;}
    virtual void         insert(const T& anything)             {return;}
    virtual void         append(const T& anything)             {return;}
    virtual void         delCurrent() 		               {return;}
    virtual void         findFirst() 		               {return;}
    virtual void         findLast() 		               {return;}
    virtual void         findNext() 		               {return;}
    virtual void         findPrev() 		               {return;}
    virtual T*           getCurrent() const 	               {return NULL;}
    virtual int          isEmpty() const	               {return 1;}
    virtual int          isFull() const 		       {return 1;}
    virtual unsigned int numElements() const		       {return 0}
    virtual unsigned int index() const                         {return 0;}
    virtual T&           operator[](const unsigned int index)  {return *NULL;}
    virtual const T&     operator[](const unsigned int index)
			   const			       {return *NULL;}
  protected:
    virtual void      iterStart() const                        {return;}
    virtual int       iterMore() const                         {return 0;}
    virtual void      iterNext() const                         {return;}
    virtual T*        iterElement() const                      {return NULL;}
 }

/*****************************************************************************/

template <class T> class DList:
  public List<T>

/*
This class represents a simple singly-linked linked list.  Objects placed in an
instance of this class must have a copy constructor, a destructor and a copy
operator.
*/

 {
  public:
			 DList();
			 DList(const List<T>&);
			 DList(const unsigned int, ...);
    virtual              ~DList();
    virtual void         insert(const T&);
    virtual void         append(const T&);
    virtual void         delCurrent();
    virtual void         findFirst();
    virtual void         findLast();
    virtual void         findNext();
    virtual void         findPrev();
    virtual T*           getCurrent() const;
    virtual int          isEmpty() const;
    virtual int          isFull() const;
    virtual unsigned int numElements() const;
    virtual unsigned int curElement() const;
    virtual List&        operator=(const List<T>&);
    virtual T&           operator[](const unsigned int);
  protected:
    virtual void         iterStart() const;
    virtual int          iterMore() const;
    virtual void         iterNext() const;
    virtual T*           iterElement() const;
  private:
    Node<T>*             _first;          // the first node in the list
    Node<T>*             _current;        // the current node in the list
    Node<T>*             _previous;       // the node before the current node
    unsigned int         _numElements;    // the no. of elements in the list
    unsigned int         _index;          // current node's index into the list
    Node<T>**            _iterControl     // pointer to a pointer to the
					  //   current Node in the iteration

    inline void          checkInvariants();
 };

/*****************************************************************************/

template <class T> inline void DList<T>::checkInvariants()

 {
  /*
  INVARIANT:  if there is no first element then there can be no current or
  previous elements.
  */
  assert(_first || (!_current && !_previous));
  /*
  INVARIANT:  if there is a previous element then there must be a current
  element and the previous element's next element must be the current element;
  if there is no previous element then the current element must either not
  exist or be the first element.
  */
  assert
   (
    (_previous ?
     _current && _previous->next == _current :
     !_current || _current == _first
    )
   );
 }



#endif