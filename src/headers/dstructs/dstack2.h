#ifndef DSTRUCTS_DSTACK2_H
#define DSTRUCTS_DSTACK2_H

#include <stdarg.h>
#include <exception.h>

// ============================================================================================
// DATASTRUCTUREEXCEPTIONS CLASS DECLARATION
// ============================================================================================

class DataStructureExceptions
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
};

// ============================================================================================
// DATASTRUCTURE_ CLASS DECLARATION
// ============================================================================================

class DataStructure_
{
  public:
                       DataStructure_():
                         _numElements(0U) {return;}
    virtual            ~DataStructure_()
                         {return;}

    const unsigned int numElements() const throw ()
                         {return _numElements;}
    const bool         isEmpty() const throw ()
                         {return _numElements == 0U;}
    virtual const bool isFull() const throw () = 0;

  protected:
    unsigned int _numElements;              // no. of elements that the data structure contains
};

// ============================================================================================
// DATASTRUCTURE<T> CLASS DECLARATION
// ============================================================================================

template<class T> class DataStructure:
  virtual public DataStructureExceptions,
  virtual public DataStructure_
{
  public:
    virtual void empty() throw () = 0;

  protected:
    class Iterator_
    {
      public:
        virtual                ~Iterator_()
                                 {return;}

        virtual void           start()         throw () = 0;
        virtual bool           more()    const throw () = 0;
        virtual void           next()          throw () = 0;
        virtual const T *const current() const throw () = 0;
    };

    virtual Iterator_ *const iterator() const throw () = 0;
};

// ============================================================================================
// LINEARSTRUCTURE CLASS DECLARATION
// ============================================================================================

template<class T> class LinearStructure:
  virtual public DataStructureExceptions,
  virtual public DataStructure<T>
{
  protected:
    virtual void concatenate(const DataStructure<T>&) throw (Full, OperationFailed) = 0;
};

// ============================================================================================
// DLINEARSTRUCTURE_ CLASS DECLARATION
// ============================================================================================

class DLinearStructure_:
  virtual public DataStructure_
{
  public:
    virtual const bool isFull() const throw ()
                         {return false;}

  protected:
    class Node_
    {
      public:
                     Node_(Node_ *const next):
                       _next(next) {return;}
        virtual      ~Node_()
                       {return;}

        Node_ *const next() const throw ()
                       {return _next;}
        void         setNext(Node_ *const next) throw ()
                       {_next = next; return;}

      private:
        Node_* _next;              // next Node_ in the list, or NULL if there is no next Node_
    };
};

// ============================================================================================
// DLINEARSTRUCTURE<T> CLASS DECLARATION
// ============================================================================================

template<class T> class DLinearStructure:
  virtual public DataStructureExceptions,
  virtual public LinearStructure<T>,
  virtual public DLinearStructure_
{
  public:
                 DLinearStructure();
    virtual      ~DLinearStructure();

    virtual void empty() throw ();

  protected:
    class Node:
      virtual public Node_
    {
      public:
                 Node(const T&, Node_ *const);

        T *const element() const throw ()
                   {return &_element;}

      private:
        T _element;
    };

    Node* _first;
    Node* _last;

    void initializeVariableArguments(const unsigned int, va_list&);

    #ifndef NDEBUG
      void assertInvariants() const;
    #endif

    virtual void concatenate(const DataStructure<T>&) throw (Full, OperationFailed);

    virtual DataStructure<T>::Iterator_ *const iterator() const throw ();

  private:
    class Iterator:
      virtual public DataStructure<T>::Iterator_
    {
      public:
                               Iterator(const Node *const);

        virtual void           start()         throw ();
        virtual bool           more()    const throw ();
        virtual void           next()          throw ();
        virtual const T *const current() const throw ();

      private:
        const Node *const _first;
        const Node*       _current;
    };
};

// ============================================================================================
// DSTACK CLASS DECLARATION
// ============================================================================================

template<class T> class Stack:
  virtual public DataStructureExceptions
{
  public:

    /*
    There are three public virtual methods:  "push()", "pop()" and "peek()".  "push()" and
    "pop()" must be defined as the operations that add and remove elements to and from the
    stack, respectively, in last-in-first-out order.  "peek()" must be defined as an operation
    that gets the next element to be popped off of the stack without actually popping it off.
    */

    virtual void push(const T&) throw (Full, OperationFailed)  = 0;
    virtual void pop(T&)        throw (Empty, OperationFailed) = 0;
    virtual void peek(T&) const throw (Empty, OperationFailed) = 0;
};

// ============================================================================================
// DSTACK CLASS DECLARATION
// ============================================================================================

template<class T> class DStack:
  virtual public DataStructureExceptions,
  virtual public DLinearStructure<T>,
  virtual public Stack<T>
{
  public:
         DStack()
           {return;}

    virtual void push(const T&) throw (Full, OperationFailed);
    virtual void pop(T&)        throw (Empty, OperationFailed);
    virtual void peek(T&) const throw (Empty, OperationFailed);
};

#endif