/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2016 Kyle Hollins Wray, University of Massachusetts
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in
 *  the Software without restriction, including without limitation the rights to
 *  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 *  the Software, and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 *  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <nova/mdp/utilities/mdp_model_cpu.h>

#include <gtest/gtest.h>

#include <nova/error_codes.h>
#include <nova/constants.h>

#include <unit/mdp/mdp_tests.h>

namespace nova {
namespace tests {

TEST(MDPModelCPU, initialization)
{
    nova::MDP *mdp = new nova::MDP();

    int result = nova::mdp_initialize_cpu(mdp, 10, 2, 3, 0.9f, 7, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_SUCCESS);

    EXPECT_EQ(mdp->n, 10);
    EXPECT_EQ(mdp->ns, 2);
    EXPECT_EQ(mdp->m, 3);
    EXPECT_NEAR(mdp->gamma, 0.9f, 1e-5f);
    EXPECT_EQ(mdp->horizon, 7);
    EXPECT_NEAR(mdp->epsilon, 0.01f, 1e-5f);
    EXPECT_EQ(mdp->s0, 4);
    EXPECT_EQ(mdp->ng, 1);

    EXPECT_NE(mdp->S, nullptr);
    EXPECT_NE(mdp->T, nullptr);
    EXPECT_NE(mdp->R, nullptr);
    EXPECT_NE(mdp->goals, nullptr);

    result = nova::mdp_uninitialize_cpu(mdp);
    EXPECT_EQ(result, NOVA_SUCCESS);

    delete mdp;
}

TEST(MDPModelCPU, badInitialization)
{
    nova::MDP *mdp = new nova::MDP();

    int result = nova::mdp_initialize_cpu(nullptr, 10, 2, 3, 0.9f, 7, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 0, 2, 3, 0.9f, 7, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 0, 3, 0.9f, 7, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 2, 0, 0.9f, 7, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 2, 3, 1.1f, 7, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 2, 3, -0.1f, 7, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 2, 3, 0.9f, 0, 0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 2, 3, 0.9f, 7, -0.01f, 4, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 2, 3, 0.9f, 7, 0.01f, 11, 1);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    result = nova::mdp_initialize_cpu(mdp, 10, 2, 3, 0.9f, 7, 0.01f, 4, 11);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);

    delete mdp;
}

TEST(MDPModelCPU, uninitialization)
{
    nova::MDP *mdp = create_simple_mdp(false);

    int result = nova::mdp_uninitialize_cpu(mdp);
    EXPECT_EQ(result, NOVA_SUCCESS);

    EXPECT_EQ(mdp->n, 0);
    EXPECT_EQ(mdp->ns, 0);
    EXPECT_EQ(mdp->m, 0);
    EXPECT_EQ(mdp->gamma, 0.0f);
    EXPECT_EQ(mdp->horizon, 0);
    EXPECT_EQ(mdp->epsilon, 0.0f);
    EXPECT_EQ(mdp->s0, 0);
    EXPECT_EQ(mdp->ng, 0);

    EXPECT_EQ(mdp->S, nullptr);
    if (mdp->S != nullptr) {
        delete [] mdp->S;
    }

    EXPECT_EQ(mdp->T, nullptr);
    if (mdp->T != nullptr) {
        delete [] mdp->T;
    }

    EXPECT_EQ(mdp->R, nullptr);
    if (mdp->R != nullptr) {
        delete [] mdp->R;
    }

    EXPECT_EQ(mdp->goals, nullptr);
    if (mdp->goals != nullptr) {
        delete [] mdp->goals;
    }

    delete mdp;
}

TEST(MDPModelCPU, badUninitializations)
{
    int result = nova::mdp_uninitialize_cpu(nullptr);
    EXPECT_EQ(result, NOVA_ERROR_INVALID_DATA);
}

}; // namespace tests
}; // namespace nova


