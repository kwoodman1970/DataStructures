#ifndef DSTRUCT_SDATASTRUCTURE_H
#define DSTRUCT_SDATASTRUCTURE_H

// ============================================================================================
//
// adatastructure.h -- Staticly-Allocated Data Structure Base Class
//
// ============================================================================================

/*
This class is an abstract data type base class for all staticly-allocated data structures.
*/

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <dstructs/datastructure.h>

// ============================================================================================
// SDATASTRUCTURE_ CLASS DECLARATION
// ============================================================================================

class SDataStructure_
{
  public:
                       SDataStructure_(const unsigned int initialSize):
                         _size(initialSize)
                         {return;}

    const unsigned int size() const throw ()
                         {return _size;}
    const bool         isFull() const throw ()
                         {return _numElements == _size;}

  private:
    unsigned int _size;          // maximum no. of elements that the data structure can contain
};

// ============================================================================================
// SDATASTRUCTURE<T> CLASS DECLARATION
// ============================================================================================

template<class T> class SDataStructure:
  virtual public SDataStructure_
{
  public:
                   SDataStructure(const unsigned int) throw
                     (DataStructureExceptions::OperationFailed);
    virtual        ~SDataStructure()
                     {delete[] _elements; return;}

  protected:
    T const*       elements()
                     {return elements_;}

    #ifndef NDEBUG
      virtual void assertInvariants() const throw ()
                     {assert((_size == 0U) == (_elements == null)); return;}
    #endif

  private:
    T* elements;
};

// ============================================================================================
// SDATASTRUCTURE<T> METHODS
// ============================================================================================

template<class T> SDataStructure<T>::SDataStructure(const unsigned int initialSize):

/*
*/

SDataStructure_(initialSize),
_elements(initialSize == 0U ? null : new T[initialSize])

{
  if (initialSize > 0U && _elements == null)
  {
    throw DataStructureExceptions::OperationFailed("Could not allocate enough memory for this "
      "data structure.", __FILE__, __LINE__);
  }

  return;
}