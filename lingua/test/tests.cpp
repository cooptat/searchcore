/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestStemmer_init = false;
#include "TestStemmer.h"

static TestStemmer suite_TestStemmer;

static CxxTest::List Tests_TestStemmer = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestStemmer( "TestStemmer.h", 6, "TestStemmer", suite_TestStemmer, Tests_TestStemmer );

static class TestDescription_suite_TestStemmer_testStemmer : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestStemmer_testStemmer() : CxxTest::RealTestDescription( Tests_TestStemmer, suiteDescription_TestStemmer, 9, "testStemmer" ) {}
 void runTest() { suite_TestStemmer.testStemmer(); }
} testDescription_suite_TestStemmer_testStemmer;

#include "TestWordDetJson.h"

static TestWordDetJson suite_TestWordDetJson;

static CxxTest::List Tests_TestWordDetJson = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestWordDetJson( "TestWordDetJson.h", 6, "TestWordDetJson", suite_TestWordDetJson, Tests_TestWordDetJson );

static class TestDescription_suite_TestWordDetJson_testSpell : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestWordDetJson_testSpell() : CxxTest::RealTestDescription( Tests_TestWordDetJson, suiteDescription_TestWordDetJson, 11, "testSpell" ) {}
 void runTest() { suite_TestWordDetJson.testSpell(); }
} testDescription_suite_TestWordDetJson_testSpell;

#include "TestSpeller.h"

static TestSpeller suite_TestSpeller;

static CxxTest::List Tests_TestSpeller = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSpeller( "TestSpeller.h", 13, "TestSpeller", suite_TestSpeller, Tests_TestSpeller );

static class TestDescription_suite_TestSpeller_testSpeller : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSpeller_testSpeller() : CxxTest::RealTestDescription( Tests_TestSpeller, suiteDescription_TestSpeller, 32, "testSpeller" ) {}
 void runTest() { suite_TestSpeller.testSpeller(); }
} testDescription_suite_TestSpeller_testSpeller;

#include "TestSpellerClass.h"

static TestSpellerClass suite_TestSpellerClass;

static CxxTest::List Tests_TestSpellerClass = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSpellerClass( "TestSpellerClass.h", 12, "TestSpellerClass", suite_TestSpellerClass, Tests_TestSpellerClass );

static class TestDescription_suite_TestSpellerClass_testSpeller : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSpellerClass_testSpeller() : CxxTest::RealTestDescription( Tests_TestSpellerClass, suiteDescription_TestSpellerClass, 32, "testSpeller" ) {}
 void runTest() { suite_TestSpellerClass.testSpeller(); }
} testDescription_suite_TestSpellerClass_testSpeller;

#include "TestSemanticSex.h"

static TestSemanticSex suite_TestSemanticSex;

static CxxTest::List Tests_TestSemanticSex = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSemanticSex( "TestSemanticSex.h", 7, "TestSemanticSex", suite_TestSemanticSex, Tests_TestSemanticSex );

static class TestDescription_suite_TestSemanticSex_testSemanticSamples : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSemanticSex_testSemanticSamples() : CxxTest::RealTestDescription( Tests_TestSemanticSex, suiteDescription_TestSemanticSex, 11, "testSemanticSamples" ) {}
 void runTest() { suite_TestSemanticSex.testSemanticSamples(); }
} testDescription_suite_TestSemanticSex_testSemanticSamples;

#include "TestSemanticSort.h"

static TestSemanticSort suite_TestSemanticSort;

static CxxTest::List Tests_TestSemanticSort = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestSemanticSort( "TestSemanticSort.h", 7, "TestSemanticSort", suite_TestSemanticSort, Tests_TestSemanticSort );

static class TestDescription_suite_TestSemanticSort_testSemanticSamples : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestSemanticSort_testSemanticSamples() : CxxTest::RealTestDescription( Tests_TestSemanticSort, suiteDescription_TestSemanticSort, 11, "testSemanticSamples" ) {}
 void runTest() { suite_TestSemanticSort.testSemanticSamples(); }
} testDescription_suite_TestSemanticSort_testSemanticSamples;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
