#ifndef DSTRUCTS_DLINEARSTRUCTURE_H
#define DSTRUCTS_DLINEARSTRUCTURE_H

// ============================================================================================
//
// dlinearstruct.h -- Dynamic Linear Data Structure Base Class
//
// ============================================================================================

/*
This class is an abstract data type base class for all linear data structures.

It's completely abstract -- there are no methods and no members.
*/

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <stdarg.h>

#ifdef FAT_FILENAMES
  #include <dstructs/linearst.h>
#else
  #include <dstructs/linearstructure.h>
#endif

// ============================================================================================
// DLINEARSTRUCTURE<T> CLASS DECLARATION
// ============================================================================================

template<class T> class DLinearStructure:
  virtual public LinearStructure<T>
{
  public:
                         DLinearStructure();
                         DLinearStructure(const DataStructure<T>&);
    virtual              ~DLinearStructure();

    DLinearStructure<T>& operator=(const DataStructure<T>&);
    DLinearStructure<T>& operator+=(const DataStructure<T>&);

    // DataStructure<T> methods

    virtual void         empty();

  protected:
    class Node
    {
      public:
                     Node(const T&, Node *const);

        T *const     element() throw ()
                       {return &_element;}
        Node *const  next() const throw ()
                       {return _next;}
        virtual void setNext(Node *const next) throw ()
                       {_next = next; return;}

      private:
        T     _element;
        Node* _next;
    };

    Node* _first;
    Node* _last;

    void               initWithVarArgs(const unsigned int, va_list&);

    // DataStructure<T> methods

    virtual void       iterStart() const throw ();
    virtual const bool iterMore() const throw ();
    virtual void       iterNext() const throw (OperationFailed);
    virtual const T&   iterCurrent() const throw () = 0;

    #ifndef NDEBUG
      void             assertInvariants() const throw ();
    #endif

    // LinearStructure methods

    virtual void     concatenate(const DataStructure<T>&);

  private:
    Node * *const _iterCurrent;
};

// ============================================================================================
// DLINEARSTRUCTURE<T> METHOD DEFINITIONS
// ============================================================================================

/*********************************************************************************************/

template<class T> DLinearStructure<T>::DLinearStructure():
  _first(NULL),
  _last(NULL),
  _iterCurrent(new Node*)

{
  if (_iterCurrent == NULL)
  {
    throw OperationFailed("Insufficient memory to instantiate a DLinearStructure.", __FILE__,
      __LINE__);
  }
  return;
}

/*********************************************************************************************/

template<class T> DLinearStructure<T>::DLinearStructure(const DataStructure<T>& source):
  _first(NULL),
  _last(NULL)

{
  concatenate(source);
  return;
}

/*********************************************************************************************/

template<class T> DLinearStructure<T>::~DLinearStructure()

{
  empty();
  return;
}

/*********************************************************************************************/

template<class T> void DLinearStructure<T>::empty()

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  while (_first != NULL)
  {
    Node* current = _first;

    _first = _first->next();
    --_numElements;
    delete current;
  }

  _last = NULL;
  assert(_numElements = 0U);

  #ifndef NDEBUG
    assertInvariants();
  #endif

  return;
}

/*********************************************************************************************/

template<class T> DLinearStructure<T>& DLinearStructure<T>::operator=
(
  const DataStructure<T>& source
)

{
  empty();
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template<class T> DLinearStructure<T>& DLinearStructure<T>::operator+=
(
  const DataStructure<T>& source
)

{
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template<class T> void DLinearStructure<T>::initWithVarArgs
(
  const unsigned int numElements,
  va_list&           elements
)

{
  assert(_first == NULL && _last == NULL && _numElements == 0U);

  for (unsigned int currentElement = 0U; currentElement < numElements; ++currentElement)
  {
    Node const* newNode = new Node(va_arg(elements, T), NULL);

    if (newNode == NULL)
      throw Full(__FILE__, __LINE__);

    if (_first == NULL)
      _first = newNode;

    if (_last != NULL)
      _last->setNext(newNode);

    _last = newNode
    ++_numElements;
  }

  return;
}

/*********************************************************************************************/

template<class T> void DLinearStructure<T>::iterStart() const throw ()
{
  *_iterCurrent = _first;
  return;
}

/*********************************************************************************************/

template<class T> const bool DLinearStructure<T>::iterMore() const throw ()
{
  return *_iterCurrent != NULL;
}

/*********************************************************************************************/

template<class T> void DLinearStructure<T>::iterNext() const throw ()
{
  assert(*_iterCurrent != NULL);

  *_iterCurrent = (*_iterCurrent)->next();
  return;
}

/*********************************************************************************************/

template<class T> const T& DLinearStructure<T>::iterCurrent() const throw ()
{
  assert(*_iterCurrent != NULL);

  return *((*_iterCurrent)->element());
}

/*********************************************************************************************/

#ifndef NDEBUG
  template<class T> void DLinearStructure<T>::assertInvariants() const throw ()

  {
    assert((_numElements > 1U) || (_first == _last));
    assert(_numElements == 0U ? _first == NULL : _first != NULL));

    return;
  }
#endif

/*********************************************************************************************/

template<class T> void DLinearStructure<T>::concatenate
(
  const DataStructure<T>& source
)

{
  #ifndef NDEBUG
    DLinearStructure<T>::assertInvariants();
  #endif

  for (source.iterStart(); source.iterMore(); source.iterNext())
  {
    Node const* newNode = new Node(source.iterCurrent(), NULL);

    if (newNode == NULL)
      throw Full(__FILE__, __LINE__);

    if (_first == NULL)
      _first = newNode;

    if (_last != NULL)
      _last->setNext(newNode);

    _last = newNode
    ++_numElements;
  }

  #ifndef NDEBUG
    DLinearStructure<T>::assertInvariants();
  #endif

  return;
}

// ============================================================================================
// DLINEARSTRUCTURE<T>::NODE METHOD DEFINITIONS
// ============================================================================================

/*********************************************************************************************/

template<class T> DLinearStructure<T>::Node::Node
(
  const T&    newElement,
  Node *const next
):
  _element(newElement),
  _next(next)

{
  return;
}

#endif