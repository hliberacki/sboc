#ifndef INCLUDE_EXPERIMENTAL_CONCEPTS_TS_HPP
#define INCLUDE_EXPERIMENTAL_CONCEPTS_TS_HPP
#include <type_traits>
#include <tuple>
#include <range/v3/core.hpp>
#include <range/v3/utility/common_type.hpp>
#include <range/v3/range_fwd.hpp>

namespace std {

    template< class T >
    struct remove_cvref {
        typedef std::remove_cv_t<std::remove_reference_t<T>> type;
    };

    template< class T>
    using remove_cvref_t = typename std::remove_cvref<T>::type;

    // Core language concepts

    template < class T, class U >
    concept Same = std::is_same_v<T, U>;

    template< class Derived, class Base >
    concept DerivedFrom =
      std::is_base_of_v<Base, Derived> &&
      std::is_convertible_v<const volatile Derived*, const volatile Base*>;

    template <class From, class To>
    concept ConvertibleTo =
      std::is_convertible_v<From, To> &&
      requires(From (&f)()) {
        static_cast<To>(f());
      };

    template < class T, class U >
    concept CommonReference =
      std::Same<ranges::common_reference_t<T, U>, ranges::common_reference_t<U, T>> &&
      std::ConvertibleTo<T, ranges::common_reference_t<T, U>> &&
      std::ConvertibleTo<U, ranges::common_reference_t<T, U>>;

    template <class T, class U>
    concept Common =
      std::Same<std::common_type_t<T, U>, std::common_type_t<U, T>> &&
      requires {
        static_cast<std::common_type_t<T, U>>(std::declval<T>());
        static_cast<std::common_type_t<T, U>>(std::declval<U>());
      } &&
      std::CommonReference<
        std::add_lvalue_reference_t<const T>,
        std::add_lvalue_reference_t<const U>> &&
      std::CommonReference<
        std::add_lvalue_reference_t<std::common_type_t<T, U>>,
        ranges::common_reference_t<
          std::add_lvalue_reference_t<const T>,
          std::add_lvalue_reference_t<const U>>>;

    template < class T >
    concept Integral = std::is_integral_v<T>;

    template < class T >
    concept SignedIntegral = std::Integral<T> && std::is_signed_v<T>;

    template < class T >
    concept UnsignedIntegral = std::Integral<T> && !std::SignedIntegral<T>;

    template< class LHS, class RHS >
    concept Assignable =
      std::is_lvalue_reference_v<LHS> &&
      std::CommonReference<
        const std::remove_reference_t<LHS>&,
        const std::remove_reference_t<RHS>&> &&
      requires(LHS lhs, RHS&& rhs) {
        lhs = std::forward<RHS>(rhs);
        requires std::Same<decltype(lhs = std::forward<RHS>(rhs)), LHS>;
      };

    template< class T >
    concept Swappable = std::is_swappable_v<T>;

    template< class T, class U >
    concept SwappableWith =
      std::is_swappable_with_v<T, T> &&
      std::is_swappable_with_v<U, U> &&
      std::CommonReference<
        const std::remove_reference_t<T>&,
        const std::remove_reference_t<U>&> &&
      std::is_swappable_with_v<T, U> &&
      std::is_swappable_with_v<U, T>;

    template < class T >
    concept Destructible = std::is_nothrow_destructible_v<T>;

    template < class T, class... Args >
    concept Constructible =
      std::Destructible<T> && std::is_constructible_v<T, Args...>;

    template < class T >
    concept DefaultConstructible = std::Constructible<T>;

    template< class T >
    concept MoveConstructible = std::Constructible<T, T> && std::ConvertibleTo<T, T>;

    template <class T>
    concept CopyConstructible =
      std::MoveConstructible<T> &&
      std::Constructible<T, T&> && std::ConvertibleTo<T&, T> &&
      std::Constructible<T, const T&> && std::ConvertibleTo<const T&, T> &&
      std::Constructible<T, const T> && std::ConvertibleTo<const T, T>;

    template < class T >
    concept Movable =
      std::is_object_v<T> &&
      std::MoveConstructible<T> &&
      std::Assignable<T&, T> &&
      std::Swappable<T>;

    template <class B>
    concept Boolean =
      std::Movable<remove_cvref_t<B>> &&
      requires(const std::remove_reference_t<B>& b1,
               const std::remove_reference_t<B>& b2, const bool a) {
        requires std::ConvertibleTo<const std::remove_reference_t<B>&, bool>;
        !b1;      requires std::ConvertibleTo<decltype(!b1), bool>;
        b1 && a;  requires std::Same<decltype(b1 && a), bool>;
        b1 || a;  requires std::Same<decltype(b1 || a), bool>;
        b1 && b2; requires std::Same<decltype(b1 && b2), bool>;
        a && b2;  requires std::Same<decltype(a && b2), bool>;
        b1 || b2; requires std::Same<decltype(b1 || b2), bool>;
        a || b2;  requires std::Same<decltype(a || b2), bool>;
        b1 == b2; requires std::ConvertibleTo<decltype(b1 == b2), bool>;
        b1 == a;  requires std::ConvertibleTo<decltype(b1 == a), bool>;
        a == b2;  requires std::ConvertibleTo<decltype(a == b2), bool>;
        b1 != b2; requires std::ConvertibleTo<decltype(b1 != b2), bool>;
        b1 != a;  requires std::ConvertibleTo<decltype(b1 != a), bool>;
        a != b2;  requires std::ConvertibleTo<decltype(a != b2), bool>;
      };


    template <class T, class U>
    concept __WeaklyEqualityComparableWith = // exposition only
      requires(const std::remove_reference_t<T>& t,
               const std::remove_reference_t<U>& u) {
        t == u; requires std::Boolean<decltype(t == u)>;
        t != u; requires std::Boolean<decltype(t != u)>;
        u == t; requires std::Boolean<decltype(u == t)>;
        u != t; requires std::Boolean<decltype(u != t)>;
      };

    template < class T >
    concept EqualityComparable = __WeaklyEqualityComparableWith<T, T>;
    template <class T, class U>
    concept EqualityComparableWith =
      std::EqualityComparable<T> &&
      std::EqualityComparable<U> &&
      std::CommonReference<
        const std::remove_reference_t<T>&,
        const std::remove_reference_t<U>&> &&
      std::EqualityComparable<
        ranges::common_reference_t<
          const std::remove_reference_t<T>&,
          const std::remove_reference_t<U>&>> &&
      __WeaklyEqualityComparableWith<T, U>;

    template <class T>
    concept StrictTotallyOrdered =
      std::EqualityComparable<T> &&
      requires(const std::remove_reference_t<T>& a,
               const std::remove_reference_t<T>& b) {
        a < b;  requires std::Boolean<decltype(a < b)>;
        a > b;  requires std::Boolean<decltype(a > b)>;
        a <= b; requires std::Boolean<decltype(a <= b)>;
        a >= b; requires std::Boolean<decltype(a >= b)>;
      };
    template <class T, class U>
    concept StrictTotallyOrderedWith =
      std::StrictTotallyOrdered<T> &&
      std::StrictTotallyOrdered<U> &&
      std::CommonReference<
        const std::remove_reference_t<T>&,
        const std::remove_reference_t<U>&> &&
      std::StrictTotallyOrdered<
        ranges::common_reference_t<
          const std::remove_reference_t<T>&,
          const std::remove_reference_t<U>&>> &&
      std::EqualityComparableWith<T, U> &&
      requires(const std::remove_reference_t<T>& t,
               const std::remove_reference_t<U>& u) {
        t < u;  requires std::Boolean<decltype(t < u)>;
        t > u;  requires std::Boolean<decltype(t > u)>;
        t <= u; requires std::Boolean<decltype(t <= u)>;
        t >= u; requires std::Boolean<decltype(t >= u)>;
        u < t;  requires std::Boolean<decltype(u < t)>;
        u > t;  requires std::Boolean<decltype(u > t)>;
        u <= t; requires std::Boolean<decltype(u <= t)>;
        u >= t; requires std::Boolean<decltype(u >= t)>;
      };

    // Object concepts

    template <class T>
    concept Copyable =
      std::CopyConstructible<T> &&
      std::Movable<T> &&
      std::Assignable<T&, const T&>;

    template <class T>
    concept Semiregular = std::Copyable<T> && std::DefaultConstructible<T>;

    template <class T>
    concept Regular = std::Semiregular<T> && std::EqualityComparable<T>;

    // Callable concepts

    template< class F, class... Args >
    concept Invocable =
      requires(F&& f, Args&&... args) {
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
          /* not required to be equality preserving */
      };

    template< class F, class... Args >
    concept RegularInvocable = std::Invocable<F, Args...>;

    template < class F, class... Args >
    concept Predicate =
      std::RegularInvocable<F, Args...> &&
      std::Boolean<std::invoke_result_t<F, Args...>>;

    template <class R, class T, class U>
    concept Relation =
      std::Predicate<R, T, T> && std::Predicate<R, U, U> &&
      std::Predicate<R, T, U> && std::Predicate<R, U, T>;

    template < class R, class T, class U >
    concept StrictWeakOrder = std::Relation<R, T, U>;
}


#endif //INCLUDE_EXPERIMENTAL_CONCEPTS_TS_HPP
