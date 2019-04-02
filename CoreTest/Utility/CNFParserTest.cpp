#include "CoreTest/stdafx.h"
#include "CppUnitTest.h"

#include "Core/Utility/CNFParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreTest {
TEST_CLASS(CNFParserTest)
{
public:

    TEST_METHOD(TestParseCNF_Simple)
    {
        std::stringstream ss(
            "c\n"
            "c A sample.cnf file.\n"
            "p cnf 6 3\n"
            "1 -3 0\n"
            "2 3 -1 0\n"
            "5 -6 0\n");

        auto p = ParseCNF(ss);

        Assert::AreEqual<size_t>(3, p.GetClauses().size());
        Assert::AreEqual(6, p.GetNumberOfVariables());
    }

};
}