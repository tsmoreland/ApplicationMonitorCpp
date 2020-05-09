//
// Copyright © 2020 Terry Moreland
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
 
#pragma once

#include <boost/test/included/unit_test.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

// adapted from https://www.semipol.de/2010/04/13/using-googlemock-with-the-boost-test-library.html
class BoostTestAdapter: public EmptyTestEventListener {
public:
    void OnTestStart(const TestInfo&) override { }
    void OnTestPartResult(const TestPartResult& testPartResult) override {
        if (testPartResult.failed()) {
            stringstream s;
            s << "Mock test failed (file = '"
              << testPartResult.file_name()
              << "', line = "
              << testPartResult.line_number()
              << "): "
              << testPartResult.summary();
            BOOST_FAIL(s.str());
        }
    }
    void OnTestEnd(const ::testing::TestInfo&) override { }
};

