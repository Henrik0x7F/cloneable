#pragma once

#include <memory>
#include <type_traits>


namespace Impl
{

template<typename Derived, typename InheritFrom, typename Base>
struct Cloner : InheritFrom
{
    friend Derived;

    static_assert(std::is_base_of_v<Base, InheritFrom> || std::is_same_v<Derived, Base>);

    template<typename T>
    using Derive = Cloner<T,  Derived, Base>;

    [[nodiscard]]
    std::shared_ptr<Derived> clone() const
    {
        return std::static_pointer_cast<Derived>(clone_impl());
    }

protected:
    [[nodiscard]]
    virtual std::shared_ptr<Base> clone_impl() const
    {
        return std::make_shared<Derived>(static_cast<const Derived&>(*this));
    }

private:
    template<typename... Args>
    Cloner(Args&&... args):
    InheritFrom(std::forward<Args>(args)...)
    {}
};

struct Empty
{};

}

template<typename T>
using Cloneable = Impl::Cloner<T, Impl::Empty, T>;
