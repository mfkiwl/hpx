//  Copyright (c) 2021 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/config.hpp>

#if defined(HPX_HAVE_NETWORKING)
#include <hpx/modules/errors.hpp>

#include <hpx/parcelset/detail/message_handler_interface_functions.hpp>
#include <hpx/parcelset/parcelset_fwd.hpp>
#include <hpx/parcelset_base/policies/message_handler.hpp>

#include <cstddef>

///////////////////////////////////////////////////////////////////////////////
namespace hpx::parcelset::detail {

    policies::message_handler* (*get_message_handler)(char const* action,
        char const* type, std::size_t num, std::size_t interval,
        locality const& loc, error_code& ec) = nullptr;

    void (*register_message_handler)(char const* message_handler_type,
        char const* action, error_code& ec) = nullptr;

    parcelset::policies::message_handler* (*create_message_handler)(
        char const* message_handler_type, char const* action,
        parcelset::parcelport* pp, std::size_t num_messages,
        std::size_t interval, error_code& ec) = nullptr;

}    // namespace hpx::parcelset::detail

#endif
