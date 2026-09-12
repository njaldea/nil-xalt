// Copyright (c) 2026, Neil Aldea <njaldea@gmail.com>
// SPDX-License-Identifier: BSL-1.0

#pragma once

namespace nil::xalt
{
    template <typename... T>
    void undefined() = delete;

    template <auto... T>
    void undefined() = delete;
}
