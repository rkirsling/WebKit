/*
 * Copyright (C) 2013 Google, Inc. All rights reserved.
 * Copyright (C) 2015-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <wtf/CompactRefPtrTuple.h>
#include <wtf/Packed.h>
#include <wtf/WeakPtrFactory.h>
#include <wtf/WeakRef.h>

namespace WTF {

template<typename WeakPtrFactoryType, WeakPtrFactoryInitialization initializationMode = WeakPtrFactoryInitialization::Lazy>
class CanMakeWeakPtrBase {
public:
    using WeakValueType = typename WeakPtrFactoryType::ObjectType;
    using WeakPtrImplType = typename WeakPtrFactoryType::WeakPtrImplType;

    WeakPtrImplType* weakImplIfExists() const { return m_weakPtrFactory.impl(); }
    WeakPtrImplType& weakImpl() const
    {
        initializeWeakPtrFactory();
        return *m_weakPtrFactory.impl();
    }
    unsigned weakCount() const { return m_weakPtrFactory.weakPtrCount(); }

protected:
    CanMakeWeakPtrBase()
    {
        if (initializationMode == WeakPtrFactoryInitialization::Eager)
            initializeWeakPtrFactory();
    }

    CanMakeWeakPtrBase(const CanMakeWeakPtrBase&)
    {
        if (initializationMode == WeakPtrFactoryInitialization::Eager)
            initializeWeakPtrFactory();
    }

    CanMakeWeakPtrBase& operator=(const CanMakeWeakPtrBase&) { return *this; }

    void initializeWeakPtrFactory() const
    {
        m_weakPtrFactory.initializeIfNeeded(static_cast<const WeakValueType&>(*this));
    }

    const WeakPtrFactoryType& weakPtrFactory() const { return m_weakPtrFactory; }
    WeakPtrFactoryType& weakPtrFactory() { return m_weakPtrFactory; }

private:
    WeakPtrFactoryType m_weakPtrFactory;
};

template<typename T, WeakPtrFactoryInitialization initializationMode = WeakPtrFactoryInitialization::Lazy, typename WeakPtrImpl = DefaultWeakPtrImpl>
using CanMakeWeakPtr = CanMakeWeakPtrBase<WeakPtrFactory<T, WeakPtrImpl>, initializationMode>;

template<typename T, WeakPtrFactoryInitialization initializationMode = WeakPtrFactoryInitialization::Lazy, typename WeakPtrImpl = DefaultWeakPtrImpl>
using CanMakeWeakPtrWithBitField = CanMakeWeakPtrBase<WeakPtrFactoryWithBitField<T, WeakPtrImpl>, initializationMode>;

template<typename T, WeakPtrFactoryInitialization initializationMode = WeakPtrFactoryInitialization::Lazy>
using CanMakeSingleThreadWeakPtr = CanMakeWeakPtr<T, initializationMode, SingleThreadWeakPtrImpl>;

} // namespace WTF

using WTF::CanMakeWeakPtr;
using WTF::CanMakeWeakPtrWithBitField;
using WTF::CanMakeSingleThreadWeakPtr;
