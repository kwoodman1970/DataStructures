#ifndef DSTRUCTS_DLINKEDLIST_H
#define DSTRUCTS_DLINKEDLIST_H

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <assert.h>

#ifdef FAT_FILENAMES
  #include <dstructs/linkedli.h>
#else
  #include <dstructs/linkedlist.h>
#endif

// ============================================================================================
// DLINKEDLIST<T> CLASS DECLARATION
// ============================================================================================

template <class T> class DLinkedList:
  virtual public DLinearStructure<T>,
  virtual public LinkedList<T>
{
  public:
                       DLinkedList<T>() throw ():
                         _prev(NULL), _current(NULL) {return;}

    DLinkedList<T>&    operator=(const DataStructure<T>&);
    DLinkedList<T>&    operator+=(const DataStructure<T>&);

    // LinkedList<T> methods

    virtual void       findFirst() throw (Empty);
    virtual void       findNext() throw (Empty, OperationFailed);
    virtual T *const   retrieve() throw (Empty);
    virtual void       insert(const T&);
    virtual void       append(const T&);
    virtual void       remove();
    virtual const bool isLast() const throw (Empty);

  private:

    Node* _prev;
    Node* _current;

    #ifndef NDEBUG
      void assertInvariants() const throw ();
    #endif
};

// ============================================================================================
// METHOD DEFINITIONS
// ============================================================================================

template<class T> DLinkedList<T>& DLinkedList<T>::operator=
(
  const DataStructure<T>& source
)

{
  empty();
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template<class T> DLinkedList<T>& DLinkedList<T>::operator+=
(
  const DataStructure<T>& source
)

{
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template <class T> void DLinkedListT<T>::findFirst()

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  _prev    = NULL;
  _current = _first;

  return;
}

/*********************************************************************************************/

template <class T> void DLinkedListT<T>::findNext() throw (DataStructure::OperationFailed)

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  if (_current == NULL)
    throw OperationFailed("There is no next element.", __FILE__, __LINE__);

  _prev    = _current;
  _current = _current->next();

  return;
}

/*********************************************************************************************/

template <class T> T *const DLinkedListT<T>::current() const throw ()

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  return (_current != NULL ? _current->element() : NULL);
}

/*********************************************************************************************/

template <class T> void DLinkedListT<T>::insert(const T& newElement) throw
  (DataStructure::Full, DataStructure::OperationFailed)

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  Node* newNode;

  try
  {
    newNode = new Node(newElement, _current)
  }
  catch (...)
  {
    throw OperationFailed("Unable to copy an element while inserting.", __FILE__, __LINE__);
  }

  if (newNode == NULL)
    throw Full(__FILE__, __LINE__);

  if (_first == NULL)
  {
    _first = _newNode;
    _last  = _newNode;
  }

  if (_prev != NULL)
  {
    if (_prev->next() == NULL)
      _last = _newNode;

    _prev->setNext(_newNode);
  }

  _current = _newNode;

  ++_numElements;
  return;
}

/*********************************************************************************************/

template <class T> void DLinkedListT<T>::append(const T& newElement) throw
  (DataStructure::Full, DataStructure::OperationFailed)

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  Node* newNode;

  try
  {
    newNode = new Node(newElement, _current)
  }
  catch (...)
  {
    throw OperationFailed("Unable to copy an element while appending.", __FILE__, __LINE__);
  }

  if (newNode == NULL)
    throw Full(__FILE__, __LINE__);

  if (_last == NULL)
    _first = _newNode;
  else
  {
    _prev = _last;
    _prev->setNext(newNode);
  }

  _last    = _newNode;
  _current = _newNode;

  ++_numElements;
  return;
}

/*********************************************************************************************/

template <class T> void DLinkedListT<T>::deleteCurrent() throw (DataStructure::OperationFailed)

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  if (_current == NULL)
    throw OperationFailed("There is no current element to delete.", __FILE__, __LINE__);

  if (_last == _current)
    _last = _prev;

  if (_first == _current)
  {
    _first = _first->next();

    try
    {
      delete _current;
    }
    catch (...)
    {}

    _current = _first;
  }
  else
  {
    _prev->setNext(_current->next());

    try
    {
      delete _current;
    }
    catch (...)
    {}

    _current = _prev->next();
  }

  --_numElements;
  return;
}

/*********************************************************************************************/

template <class T> void DLinkedListT::empty() throw ()

{
  #ifndef NDEBUG
    assertInvariants();
  #endif

  DLinearStructureT<T>::empty();

  _prev    = NULL;
  _current = NULL;

  return;
}

/*********************************************************************************************/

#ifndef NDEBUG
  template <class T> void DLinkedListT<T>::assertInvariants() const throw ()

  {
    assert(_first != NULL || (_current == NULL && _prev == NULL));
    assert(_prev == NULL || _prev->next() == _current);
    assert(_prev != NULL || _current == _first);
    assert(_prev == NULL || _prev != _last || _prev->next() == NULL);

    return;
  }
#endif

#endif