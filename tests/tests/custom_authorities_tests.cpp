/*
 * Copyright (c) 2017 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <boost/test/unit_test.hpp>

#include <graphene/chain/custom_authority_object.hpp>
#include <graphene/chain/protocol/restriction.hpp>
#include <graphene/chain/protocol/operations.hpp>

using namespace graphene::chain;

BOOST_AUTO_TEST_SUITE( custom_authority )

BOOST_AUTO_TEST_CASE( validation_for_correct_operation_name_is_passed )
{
    custom_authority_object authority;
    
    authority.operation_name = "graphene::chain::transfer_operation";
    BOOST_CHECK(authority.validate(transfer_operation(), time_point_sec(0)));
    
    authority.operation_name = "graphene::chain::asset_create_operation";
    BOOST_CHECK(authority.validate(asset_create_operation(), time_point_sec(0)));
}

BOOST_AUTO_TEST_CASE( validation_for_wrong_operation_name_is_failed )
{
    custom_authority_object authority;
    
    authority.operation_name = "graphene::chain::asset_create_operation";
    BOOST_CHECK(!authority.validate(transfer_operation(), time_point_sec(0)));
    
    authority.operation_name = "graphene::chain::transfer_operation";
    BOOST_CHECK(!authority.validate(asset_create_operation(), time_point_sec(0)));
}

BOOST_AUTO_TEST_CASE( validation_fails_when_now_is_after_valid_period )
{
    custom_authority_object authority;
    
    authority.operation_name = "graphene::chain::transfer_operation";
    authority.valid_from = time_point_sec(0);
    authority.valid_to = time_point_sec(5);
    BOOST_CHECK(!authority.validate(transfer_operation(), time_point_sec(6)));
}

BOOST_AUTO_TEST_CASE( validation_fails_when_now_is_before_valid_period )
{
    graphene::chain::custom_authority_object authority;
    
    authority.operation_name = "graphene::chain::transfer_operation";
    authority.valid_from = time_point_sec(3);
    authority.valid_to = time_point_sec(5);
    BOOST_CHECK(!authority.validate(transfer_operation(), time_point_sec(1)));
}

BOOST_AUTO_TEST_CASE( validation_passes_when_now_is_in_valid_period )
{
    custom_authority_object authority;
    
    authority.operation_name = "graphene::chain::transfer_operation";
    authority.valid_from = time_point_sec(3);
    authority.valid_to = time_point_sec(5);
    BOOST_CHECK(authority.validate(transfer_operation(), time_point_sec(4)));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( restrictions )

template <class Operation>
struct agrument_comparer
{
    std::string argument;
    
    template<typename Member, class Class, Member (Class::*member)>
    void operator()( const char* name )const
    {
       
    }
};

struct operation_argument_comparer
{
    typedef void result_type;
    
    template <class OperationT>
    void operator () (const OperationT& op)
    {
        
    }
};

struct eq_restriction
{
    fc::static_variant<asset> value;
    std::string argument;
    
    bool validate( const operation& op ) const
    {
        return false;
    }
};

BOOST_AUTO_TEST_CASE( validation_passes_for_eq_restriction_when_assets_are_equal )
{
    transfer_operation operation;
    operation.amount = asset(5);
    
    eq_restriction restriction;
    restriction.value = asset(5);
    restriction.argument = "amount";
    
    BOOST_CHECK(restriction.validate(operation));
}

BOOST_AUTO_TEST_SUITE_END()
