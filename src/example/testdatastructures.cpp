#include <iostream.h>
#include <fstream.h>
#include <string.h>

#include <testsuite.h>

#include <dstructs/dstack.h>

#include <platform.h>

/*********************************************************************************************/

#if 0

TEST(staticStack)
{
  #define MAX_ELEMENTS 20U

  size_t     numElements(0U);
  int        elements[MAX_ELEMENTS];
  size_t     currentElement;
  TestResult result(pass);

  do
  {
    int newElement;

    testCase.data() >> newElement;

    if (!testCase.data().eof())
      elements[numElements++] = newElement;
  } while (!testCase.data().eof() && (numElements < MAX_ELEMENTS));

  try
  {
    SStack<int> stack(MAX_ELEMENTS);

    for (currentElement = 0U; currentElement < numElements; currentElement++)
      stack << elements[currentElement];

    for (currentElement = numElements; currentElement > 0U; currentElement--)
    {
      int poppedElement;

      stack >> poppedElement;

      if (poppedElement != elements[currentElement - 1U])
      {
        log << "  Expected " << elements[currentElement - 1U] << " from stack but got " <<
          poppedElement << " instead." << endl;

        result = fail;
      }
    }

  }
  catch (...)
  {
    log << "  Oops -- caught an exception!" << endl;
    result = fail;
  }

  return result;
}

#endif

/*********************************************************************************************/

TEST(dynamicStack)
{
  #define MAX_ELEMENTS 20U

  size_t     numElements(0U);
  int        elements[MAX_ELEMENTS];
  size_t     currentElement;
  TestResult result(pass);

  do
  {
    int newElement;

    testCase.data() >> newElement;

    if (!testCase.data().eof())
      elements[numElements++] = newElement;
  }
  while (!testCase.data().eof() && (numElements < MAX_ELEMENTS));

  try
  {
    DStack<int> stack;

    for (currentElement = 0U; currentElement < numElements; ++currentElement)
      stack << elements[currentElement];

    for (currentElement = numElements; currentElement > 0U; --currentElement)
    {
      int poppedElement;

      stack >> poppedElement;

      if (poppedElement != elements[currentElement - 1U])
      {
        log << "  Expected " << elements[currentElement - 1U] << " from stack but got " <<
          poppedElement << " instead." << endl;

        result = fail;
      }
    }

  }
  catch (...)
  {
    log << "  Oops -- caught an exception!" << endl;
    result = fail;
  }

  return result;
}

// ============================================================================
// ROUTINE & FUNCTION DEFINITIONS
// ============================================================================

/*****************************************************************************/

void main
(
  int               argc,
  const char *const argv[]
)

{
  ifstream  testData("testData.txt");
  TestSuite testSuite(testData, cout);

  if (argc <= 1)
    testSuite.all();
  else
    testSuite.group(argc - 1, argv + 1);

  return;
}
