/*
 * This file is part of the Yogi Framework
 * https://github.com/yohummus/yogi-framework.
 *
 * Copyright (c) 2020 Johannes Bergmann.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <test/common.h>

#include <yogi_core.h>

class SignalsTest : public Test {
 protected:
  SignalSetPtr create_signal_set(Signals signals) {
    MOCK_SignalSetCreate([](void** sigset, void*, int) {
      *sigset = kPointer;
      return YOGI_OK;
    });

    return SignalSet::create(create_context(), signals);
  }
};

TEST_F(SignalsTest, RaiseSignal) {
  bool called = false;
  auto fn     = [&] { called = true; };

  MOCK_RaiseSignal([](int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg) {
    EXPECT_EQ(signal, YOGI_SIG_TERM);
    EXPECT_EQ(sigarg, nullptr);
    EXPECT_NE(fn, nullptr);
    EXPECT_NE(userarg, nullptr);
    fn(sigarg, userarg);
    return YOGI_OK;
  });

  yogi::raise_signal(Signals::kTerm, fn);
  EXPECT_TRUE(called);
}

TEST_F(SignalsTest, RaiseSignalError) {
  MOCK_RaiseSignal([](int, void*, void (*)(void*, void*), void*) { return YOGI_ERR_WRONG_OBJECT_TYPE; });
  EXPECT_THROW(yogi::raise_signal(Signals::kTerm, {}), FailureException);
}

TEST_F(SignalsTest, RaiseSignalWithArg) {
  bool called = false;

  auto p     = std::make_unique<int>(123);
  auto raw_p = p.get();

  auto fn = [&](std::unique_ptr<int>* p) {
    EXPECT_EQ(p->get(), raw_p);
    called = true;
  };

  MOCK_RaiseSignal([](int signal, void* sigarg, void (*fn)(void* sigarg, void* userarg), void* userarg) {
    EXPECT_EQ(signal, YOGI_SIG_TERM);
    EXPECT_NE(sigarg, nullptr);
    EXPECT_NE(fn, nullptr);
    EXPECT_NE(userarg, nullptr);
    fn(sigarg, userarg);
    return YOGI_OK;
  });

  yogi::raise_signal_with_arg<std::unique_ptr<int>>(Signals::kTerm, std::move(p), fn);
  EXPECT_TRUE(called);
}

TEST_F(SignalsTest, RaiseSignalWithArgError) {
  MOCK_RaiseSignal([](int, void*, void (*)(void*, void*), void*) { return YOGI_ERR_WRONG_OBJECT_TYPE; });
  EXPECT_THROW(yogi::raise_signal_with_arg<std::unique_ptr<int>>(Signals::kTerm, {}, {}), FailureException);
}

TEST_F(SignalsTest, CreateSignalSet) {
  MOCK_SignalSetCreate([](void** sigset, void* context, int signals) {
    EXPECT_NE(sigset, nullptr);
    EXPECT_EQ(context, kPointer);
    EXPECT_EQ(signals, YOGI_SIG_INT | YOGI_SIG_USR3);
    return YOGI_OK;
  });

  EXPECT_TRUE(!!SignalSet::create(create_context(), Signals::kInt | Signals::kUsr3));
}

TEST_F(SignalsTest, CreateSignalSetError) {
  MOCK_SignalSetCreate([](void**, void*, int) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(SignalSet::create(create_context(), Signals::kInt), FailureException);
}

TEST_F(SignalsTest, AwaitSignal) {
  auto sigset = create_signal_set(Signals::kUsr2);

  bool called = false;
  auto fn     = [&](const Result& res, Signals signal) {
    EXPECT_EQ(res.error_code(), ErrorCode::kWrongObjectType);
    EXPECT_EQ(signal, Signals::kUsr6);
    called = true;
  };

  MOCK_SignalSetAwaitSignalAsync(
      [](void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg), void* userarg) {
        EXPECT_EQ(sigset, kPointer);
        EXPECT_NE(fn, nullptr);
        EXPECT_NE(userarg, nullptr);
        fn(YOGI_ERR_WRONG_OBJECT_TYPE, YOGI_SIG_USR6, kPointer, userarg);
        return YOGI_OK;
      });

  sigset->await_signal(fn);
  EXPECT_TRUE(called);
}

TEST_F(SignalsTest, AwaitSignalError) {
  auto sigset = create_signal_set(Signals::kUsr2);

  MOCK_SignalSetAwaitSignalAsync([](void*, void (*)(int, int, void*, void*), void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(sigset->await_signal({}), FailureException);
}

TEST_F(SignalsTest, AwaitSignalWithSigarg) {
  auto sigset = create_signal_set(Signals::kUsr2);

  bool called = false;
  auto fn     = [&](const Result& res, Signals signal, std::unique_ptr<int>* sigarg) {
    EXPECT_EQ(res.error_code(), ErrorCode::kWrongObjectType);
    EXPECT_EQ(signal, Signals::kUsr2);
    EXPECT_NE(sigarg, nullptr);
    EXPECT_TRUE(*sigarg);
    EXPECT_EQ(**sigarg, 123);
    called = true;
  };

  static void (*clb_fn)(int res, int sig, void* sigarg, void* userarg);
  static void* clb_userarg;

  clb_fn      = nullptr;
  clb_userarg = nullptr;
  MOCK_SignalSetAwaitSignalAsync(
      [](void* sigset, void (*fn)(int res, int sig, void* sigarg, void* userarg), void* userarg) {
        EXPECT_EQ(sigset, kPointer);
        EXPECT_NE(fn, nullptr);
        EXPECT_NE(userarg, nullptr);
        clb_fn      = fn;
        clb_userarg = userarg;
        return YOGI_OK;
      });

  sigset->await_signal<std::unique_ptr<int>>(fn);

  MOCK_RaiseSignal([](int, void* sigarg, void (*)(void*, void*), void*) {
    clb_fn(YOGI_ERR_WRONG_OBJECT_TYPE, YOGI_SIG_USR2, sigarg, clb_userarg);
    return YOGI_OK;
  });

  yogi::raise_signal_with_arg<std::unique_ptr<int>>(Signals::kTerm, std::make_unique<int>(123));
  EXPECT_TRUE(called);
}

TEST_F(SignalsTest, AwaitSignalWithSigargError) {
  auto sigset = create_signal_set(Signals::kUsr2);

  MOCK_SignalSetAwaitSignalAsync([](void*, void (*)(int, int, void*, void*), void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(sigset->await_signal<std::unique_ptr<int>>({}), FailureException);
}

TEST_F(SignalsTest, CancelAwaitSignal) {
  auto sigset = create_signal_set(Signals::kUsr2);

  MOCK_SignalSetCancelAwaitSignal([](void* sigset) {
    EXPECT_EQ(sigset, kPointer);
    return YOGI_OK;
  });
  EXPECT_TRUE(sigset->cancel_await_signal());

  MOCK_SignalSetCancelAwaitSignal([](void*) { return YOGI_ERR_OPERATION_NOT_RUNNING; });
  EXPECT_FALSE(sigset->cancel_await_signal());
}

TEST_F(SignalsTest, CancelAwaitSignalError) {
  auto sigset = create_signal_set(Signals::kUsr2);

  MOCK_SignalSetCancelAwaitSignal([](void*) { return YOGI_ERR_UNKNOWN; });
  EXPECT_THROW(sigset->cancel_await_signal(), FailureException);
}
