/*
 * Copyright (c) 2018 Abit More, and contributors.
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
#pragma once

namespace graphene { namespace chain {

typedef fc::static_variant<asset, account_id_type, extensions_type, future_extensions, public_key_type, time_point_sec, bool > generic_member;
	
template <typename T, typename Action>
struct member_visitor
{
	member_visitor(const std::string& member_name,  const Action& action, const T& object)
	: m_member_name(member_name)
	, m_action(action)
	, m_object(object)
	{}
	
	typedef void result_type;
	
	template<typename Member, class Class, Member (Class::*member)>
	void operator () ( const char* name ) const
	{
		if (name == m_member_name)
		{
			m_action(m_object.*member);
		}
	}
	
private:
	const std::string m_member_name;
	Action m_action;
	T m_object;
};

template <typename Action>
class operation_member_visitor
{
public:
	operation_member_visitor(const std::string& member_name,  const Action& action)
	: m_member_name(member_name)
	, m_action(action)
	{}
	
	typedef void result_type;
	
	template <typename Operation>
	void operator () (const Operation& op) const
	{
		member_visitor<Operation, Action> vistor(m_member_name, m_action, op);
		fc::reflector<Operation>::visit(vistor);
	}
	
private:
	const std::string m_member_name;
	Action m_action;
};

template <class T>
bool is_equal(const T& left, const T& right)
{
    FC_THROW("Can't compare types. Type '${type_name}' don't support == operator.", ("type_name", fc::get_typename<T>::name()));
}
	
inline bool is_equal(const asset& left, const asset& right)
{
	return left == right;
}

inline bool is_equal(const account_id_type& left, const account_id_type& right)
{
	return left == right;
}
	
template <typename T>
const T& get(const generic_member& a_variant)
{
    FC_THROW("Can't fetch value. Type '${type_name}' is not supported for now.", ("type_name", fc::get_typename<T>::name()));
}

template <>
const asset& get<asset>(const generic_member& a_variant)
{
	return a_variant.get<asset>();
}

template <>
const account_id_type& get<account_id_type>(const generic_member& a_variant)
{
	return a_variant.get<account_id_type>();
}
    
template <typename T>
const T& to_integer(const generic_member& a_variant)
{
    FC_THROW("Can't fetch value. Type '${type_name}' is not supported for now.", ("type_name", fc::get_typename<T>::name()));
}

struct number_to_integer
{
    template <typename T>
    static int64_t convert(const T& number)
    {
        return static_cast<int64_t>(number);
    }
};

struct not_number_to_integer
{
    template <typename T>
    static int64_t convert(const T& number)
    {
        FC_THROW("Can't conver type to integer.");
    }
};
    
template <typename T>
int64_t to_integer(const T& value)
{
    return boost::mpl::if_<std::is_convertible<T, int64_t>,
                            number_to_integer,
                            not_number_to_integer>::type::convert(value);
}
    
} } 
