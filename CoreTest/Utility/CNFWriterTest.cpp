#include "CoreTest/stdafx.h"
#include "CppUnitTest.h"

#include "Core/Utility/CNFWriter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreTest {
TEST_CLASS(CNFWriterTest)
{
public:

    TEST_METHOD(TestWriteCNF_Simple)
    {
        Problem p(6, {{1, -3}, {2, 3, -1}, {5, -6}});
        std::stringstream ss;

        WriteCNF(p, ss);

        std::string expected(
            "p cnf 6 3\n"
            "1 -3 0\n"
            "2 3 -1 0\n"
            "5 -6 0\n");
        Assert::AreEqual(expected, ss.str());
    }

    TEST_METHOD(TestWriteCNF_Empty)
    {
        Problem p;
        std::stringstream ss;

        WriteCNF(p, ss);

        std::string expected("p cnf 0 0\n");
        Assert::AreEqual(expected, ss.str());
    }

};
}