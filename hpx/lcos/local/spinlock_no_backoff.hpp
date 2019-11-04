////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011 Bryce Lelbach
//  Copyright (c) 2011-2012 Hartmut Kaiser
//  Copyright (c) 2008 Peter Dimov
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(HPX_LCOS_LOCAL_SPINLOCK_NO_BACKOFF)
#define HPX_LCOS_LOCAL_SPINLOCK_NO_BACKOFF

#include <hpx/config.hpp>
#include <hpx/basic_execution/register_locks.hpp>
#include <hpx/concurrency/itt_notify.hpp>
#include <hpx/runtime/threads/thread_helpers.hpp>

#if defined(HPX_WINDOWS)
#  include <boost/smart_ptr/detail/spinlock.hpp>
#  if !defined( BOOST_SP_HAS_SYNC )
#    include <hpx/config/compiler_fence.hpp>
#    include <boost/detail/interlocked.hpp>
#  endif
#else
#  if !defined(__ANDROID__) && !defined(ANDROID) && !defined(__arm__)
#    include <boost/smart_ptr/detail/spinlock.hpp>
#    if defined( __ia64__ ) && defined( __INTEL_COMPILER )
#      include <ia64intrin.h>
#    endif
#  endif
#endif

#include <cstddef>
#include <cstdint>

///////////////////////////////////////////////////////////////////////////////
namespace hpx { namespace lcos { namespace local
{
    /// boost::mutex-compatible spinlock class
    struct spinlock_no_backoff
    {
    public:
        HPX_NON_COPYABLE(spinlock_no_backoff);

    private:
        std::uint64_t v_;

    public:
        spinlock_no_backoff() : v_(0)
        {
            HPX_ITT_SYNC_CREATE(this, "hpx::lcos::local::spinlock_no_backoff", "");
        }

        ~spinlock_no_backoff()
        {
            HPX_ITT_SYNC_DESTROY(this);
        }

        void lock()
        {
            HPX_ITT_SYNC_PREPARE(this);

            for (std::size_t k = 0; !try_lock(); ++k)
            {
            }

            HPX_ITT_SYNC_ACQUIRED(this);
            util::register_lock(this);
        }

        bool try_lock()
        {
            HPX_ITT_SYNC_PREPARE(this);

#if !defined( BOOST_SP_HAS_SYNC )
            std::uint64_t r = BOOST_INTERLOCKED_EXCHANGE(&v_, 1);
            HPX_COMPILER_FENCE;
#else
            std::uint64_t r = __sync_lock_test_and_set(&v_, 1);
#endif

            if (r == 0) {
                HPX_ITT_SYNC_ACQUIRED(this);
                util::register_lock(this);
                return true;
            }

            HPX_ITT_SYNC_CANCEL(this);
            return false;
        }

        void unlock()
        {
            HPX_ITT_SYNC_RELEASING(this);

#if !defined( BOOST_SP_HAS_SYNC )
            HPX_COMPILER_FENCE;
            *const_cast<std::uint64_t volatile*>(&v_) = 0;
#else
            __sync_lock_release(&v_);
#endif

            HPX_ITT_SYNC_RELEASED(this);
            util::unregister_lock(this);
        }
    };
}}}

#endif // HPX_B3A83B49_92E0_4150_A551_488F9F5E1113
