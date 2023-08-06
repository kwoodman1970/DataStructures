#ifndef DSTRUCT_DATASTRUCTURE_H
#define DSTRUCT_DATASTRUCTURE_H

// ============================================================================================
//
// datastructure.h -- Data Structure Base Class
//
// ============================================================================================

/*
This class is an abstract data type base class for all data structures.

There is one public non-virtual method:  "numElements()".  It returns the number of elements
(of type T) that the structure currently contains (not necessarily the maximum number of
elements that the structure could contain).

There are four protected abstract virtual methods:  "iterStart()", "iterMore()", "iterNext()"
and "iterCurrent()".  Descendent classes must implement iteration algorithms such that all
elements in the data structure can be accessed as in the following examples:

  for (structure.iterStart(); structure.iterMore(); structure.iterNext())
  {
    structure.iterCurrent().doSomething();
  }

  structure.iterStart();
  while (!found && structure.iterMore())
  {
    if (structure.iterCurrent() == whatWeAreLookingFor)
      found = true;
    else
      structure.findNext();
  }

When declaring descendants of "DataStructure", it's recommended that "D" be prefixed to denote
"dynamicly-allocated" implementations and "S" be prefixed to denote "staticly-allocated"
implementations.  Consider the following class hierarchy, for example:

  DataStructure
    +--LinearStructure
    |    +--Stack
    |    |    +--SStack       <--- stack implemented in an array
    |    |    +--DStack       <--- stack implemented in free store
    |    +--Array
    |         +--SArray       <--- array implemented in an array (it sounds dumb but it isn't)
    |         +--DArray       <--- array implemented in free store
    |         +--SAssocArray  <--- associative array implemented in an array
    |         +--DAssocArray  <--- associative array implemented in free store
    +--Tree
         +--BinaryTree
              +--SBinaryTree  <--- binary tree implemented in an array
              +--DBinaryTree  <--- binary tree implemented in free store

Descendent classes are responsible for keeping the protected member "_numElements" accurate.
*/

// ============================================================================================
// DESIGN NOTES
// ============================================================================================

/*
The non-template components of this class have been separated out into the "DataStructure_"
class to reduce code bloat.  It's recommended that descendants of "DataStructure" do likewise.
*/

// ============================================================================================
// INCLUDE FILES
// ============================================================================================

#include <assert.h>

#ifdef FAT_FILENAMES
  #include <exceptn.h>
#else
  #include <exception.h>
#endif

// ============================================================================================
// DATASTRUCTURE_ CLASS DECLARATION
// ============================================================================================

class DataStructure_
{
  public:
    class OperationFailed:
      public Exception
    {
      public:
        OperationFailed(const char *const description, const char *const sourceFile,
          const unsigned int sourceFileLine):
          Exception(description, sourceFile, sourceFileLine)
          {return;}
    };

    class Empty:
      public Exception
    {
      public:
        Empty(const char *const sourceFile, const unsigned int sourceFileLine):
          Exception("Data structure is empty.", sourceFile, sourceFileLine)
          {return;}
    };

    class Full:
      public Exception
    {
      public:
        Full(const char *const sourceFile, const unsigned int sourceFileLine):
          Exception("Data structure is full.", sourceFile, sourceFileLine)
          {return;}
    };

                       DataStructure_() throw ():
                         _numElements(0U)
                         {return;}
    virtual            ~DataStructure_()
                         {return;}

    const unsigned int numElements() const throw ()
                         {return _numElements;}
    const bool         isEmpty() const throw ()
                         {return _numElements == 0U;}

  protected:
    unsigned int _numElements;              // no. of elements that the data structure contains

    #ifndef NDEBUG
      virtual void assertInvariants() const throw () = 0;
    #endif
};

// ============================================================================================
// DATASTRUCTURE<T> CLASS DECLARATION
// ============================================================================================

template<class T> class DataStructure:
  virtual public DataStructure_
{
  public:
    virtual void      empty() = 0;
    void              forAll(void (*)(T&));

    DataStructure<T>& operator=(const DataStructure<T>&);
    DataStructure<T>& operator+=(const DataStructure<T>&);

  protected:
    virtual void       iterStart() const throw () = 0;
    virtual const bool iterMore() const throw () = 0;
    virtual void       iterNext() const throw (OperationFailed) = 0;
    virtual const T&   iterCurrent() const throw () = 0;

    friend const bool operator==(const DataStructure<T>&, const DataStructure<T>&);
};

// ============================================================================================
// DATASTRUCTURE<T> METHODS
// ============================================================================================

void template<class T> DataStructure<T>::forAll(void (*operation)(T&))

/*
This method applies "operation" to every element in the data structure.  "operation" must
point to a function declared like "void doSomething(T&)".
*/

{
  for (iterStart(); iterMore(); iterNext())
  {
    operation((T&)iterCurrent());  // a rare time when const must be cast away from iterCurrent
  }

  return;
}

/*********************************************************************************************/

template<class T> DataStructure<T>& DataStructure<T>::operator=
(
  const DataStructure<T>& source
)

{
  empty();
  concatenate(source);
  return *this;
}

/*********************************************************************************************/

template<class T> DataStructure<T>& DataStructure<T>::operator+=
(
  const DataStructure<T>& source
)

{
  concatenate(source);
  return *this;
}

// ============================================================================================
// MEANINGFUL OPERATORS
// ============================================================================================

template<class T> const bool operator==
(
  const DataStructure<T>& lhs,                         // left-hand side of equality operation
  const DataStructure<T>& rhs                          // right-hand side of equality operation
)

/*
This function defines an equality operator for all data structures.

Two data structures are considered to be equal iff each element in the left-hand side is equal
to each element in the the right-hand side.  Thus, the two data structures need not even be of
the same type -- for example, a linked list can be compared to a binary tree.

If the two data structures each contain a different number of elements then they're considered
to be unequal.

"T" must have a defined equality operator.
*/

{
  #ifndef NDEBUG
    lhs.assertInvariants();
    rhs.assertInvariants();
  #endif

  /*
  The logic is straightforward enough:  if the two data structures have different numbers of
  elements then they're not equal; else, if each of their elements (in iterative order) are
  equal then they're equal; else, they're not equal.
  */

  bool areEqual(lhs.numElements() == rhs.numElements());     // are both data structures equal?

  if (areEqual)
  {
    lhs.iterStart();
    rhs.iterStart();

    /*
    This is the main loop, and it iterates through both sides' elements.  During each
    iteration, the two elements currently being iterated through are compared to see if they're
    equal -- if they're not then the two data structures are considered to be unequal.

    The loop terminates either when two unequal elements have been found or when all of the
    elements have been exhausted.
    */

    while (areEqual && lhs.iterMore())
    {
      assert(rhs.iterMore())

      areEqual = lhs.iterCurrent() == rhs.iterCurrent();

      lhs.iterNext();
      rhs.iterNext();
    }

    assert(!areEqual || !rhs.iterMore());
  }

  return areEqual;
}

/*********************************************************************************************/

template<class T> inline const bool operator!=
(
  const DataStructure<T>& lhs,                    // left-hand side of inequivilance operation
  const DataStructure<T>& rhs                     // right-hand side of inequivilance operation
)

/*
This function defines an inequality operator for all data structures.

Refer to the comments for "operator==()" (above) for further information.
*/

{
  /*
  Code re-use at its finest...
  */

  return !operator==(rhs);
}

#endif