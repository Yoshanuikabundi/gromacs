/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2019, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed in the AUTHORS file in the
 * top-level source directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */
/*! \internal \file
 * \brief
 * Tests multidimensional arrays
 *
 * \author Christian Blau <cblau@gwdg.de>
 * \ingroup module_math
 */
#include "gmxpre.h"

#include "gromacs/math/multidimarray.h"

#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "testutils/testasserts.h"

namespace gmx
{

namespace test
{

namespace
{

class MultiDimArrayTest : public ::testing::Test
{
    public:
        MultiDimArrayTest()
        {
            std::fill(begin(staticArray_), end(staticArray_), testNumber_ - 1);
            std::fill(begin(dynamicArray_), end(dynamicArray_), testNumber_ - 1);
        }
    protected:

        using static_container_type = std::array<float, 3*3>;
        using static_extents_type   = extents<3, 3>;
        using static_array_type     = MultiDimArray<static_container_type, static_extents_type>;

        using dynamic_container_type = std::vector<float>;
        using dynamic_extents_type   = extents<dynamic_extent, dynamic_extent>;
        using dynamic_array_type     = MultiDimArray<dynamic_container_type, dynamic_extents_type>;

        static_array_type  staticArray_;
        dynamic_array_type dynamicArray_ {2, 2};

        float              testNumber_      = 42;
};


TEST_F(MultiDimArrayTest, canConstructAndFillStatic)
{
    for (const auto &x : staticArray_)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, canConstructAndFillDynamic)
{
    for (const auto &x : dynamicArray_)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, canSetValuesInStatic)
{
    staticArray_(1, 1) = testNumber_;
    EXPECT_EQ(testNumber_, staticArray_(1, 1) );
}

TEST_F(MultiDimArrayTest, canSetValuesInDynamic)
{
    dynamicArray_(1, 1) = testNumber_;
    EXPECT_EQ(testNumber_, dynamicArray_(1, 1));
}

TEST_F(MultiDimArrayTest, canMoveConstructStatic)
{
    auto other(std::move(staticArray_));
    for (const auto &x : other)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, canMoveConstructDynamic)
{
    auto other(std::move(dynamicArray_));
    for (const auto &x : other)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, canMoveAssignStatic)
{
    static_array_type other;
    other = std::move(staticArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, canMoveAssignDynamic)
{
    dynamic_array_type other;
    other = std::move(dynamicArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, canCopyConstructStatic)
{
    auto other       = staticArray_;
    auto twoDArrayIt = begin(staticArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(*twoDArrayIt, x);
        ++twoDArrayIt;
    }
}

TEST_F(MultiDimArrayTest, canCopyConstructDynamic)
{
    auto other       = dynamicArray_;
    auto twoDArrayIt = begin(dynamicArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(*twoDArrayIt, x);
        ++twoDArrayIt;
    }
}

TEST_F(MultiDimArrayTest, canCopyAssignStatic)
{
    static_array_type other;
    other = staticArray_;
    auto              twoDArrayIt = begin(staticArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(*twoDArrayIt, x);
        ++twoDArrayIt;
    }
}

TEST_F(MultiDimArrayTest, canCopyAssignDynamic)
{
    dynamic_array_type other;
    other = dynamicArray_;
    auto               twoDArrayIt = begin(dynamicArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(*twoDArrayIt, x);
        ++twoDArrayIt;
    }
}

TEST_F(MultiDimArrayTest, canSwapStatic)
{
    static_array_type other;
    other.swap(staticArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, canSwapDynamic)
{
    dynamic_array_type other;
    other.swap(dynamicArray_);
    for (const auto &x : other)
    {
        EXPECT_EQ(testNumber_ - 1, x);
    }
}

TEST_F(MultiDimArrayTest, staticMultiDimArrayExtent)
{
    EXPECT_EQ(staticArray_.extent(0), 3);
    EXPECT_EQ(staticArray_.extent(1), 3);
}

TEST_F(MultiDimArrayTest, dynamicMultiDimArrayExtent)
{
    EXPECT_EQ(dynamicArray_.extent(0), 2);
    EXPECT_EQ(dynamicArray_.extent(1), 2);
}

TEST_F(MultiDimArrayTest, dynamicMultiDimArrayResizesToCorrectExtent)
{
    dynamicArray_.resize(5, 4);
    EXPECT_EQ(dynamicArray_.extent(0), 5);
    EXPECT_EQ(dynamicArray_.extent(1), 4);
}

TEST_F(MultiDimArrayTest, dynamicMultiDimArrayResizeAndSetValue)
{
    dynamicArray_.resize(5, 4);
    dynamicArray_(4, 3) = testNumber_;
    EXPECT_EQ(dynamicArray_(4, 3), testNumber_);
}

TEST_F(MultiDimArrayTest, staticMultiDimArrayFromArray)
{
    static_array_type arr = {{1, 2, 3, 4, 5, 6, 7, 8, 9}};
    EXPECT_EQ(arr(0, 0), 1);
    EXPECT_EQ(arr(0, 1), 2);
    EXPECT_EQ(arr(0, 2), 3);
    EXPECT_EQ(arr(1, 0), 4);
    EXPECT_EQ(arr(1, 1), 5);
    EXPECT_EQ(arr(1, 2), 6);
    EXPECT_EQ(arr(2, 0), 7);
    EXPECT_EQ(arr(2, 1), 8);
    EXPECT_EQ(arr(2, 2), 9);
}

} // namespace

} // namespace test

} // namespace gmx
