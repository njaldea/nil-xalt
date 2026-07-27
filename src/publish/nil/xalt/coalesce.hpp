#pragma once

#define NIL_XALT_COALESCE_TAG(ALIAS, ...)                                                          \
    struct ALIAS##_tag                                                                             \
    {                                                                                              \
        template <typename U>                                                                      \
            requires requires { typename U::ALIAS; }                                               \
        using alias = typename U::ALIAS;                                                           \
        using type = __VA_ARGS__;                                                                  \
    };

namespace nil::xalt
{
    // use NIL_XALT_COALESCE_TAG to setup your TAG
    // name should be ALIAS_tag

    template <typename T, typename Tag>
    concept has_alias = requires { typename Tag::template alias<T>; };

    template <typename T, typename Tag>
    struct coalesce;

    template <typename T, typename Tag>
        requires(!has_alias<T, Tag>)
    struct coalesce<T, Tag>
    {
        using type = typename Tag::type;
    };

    template <typename T, typename Tag>
        requires has_alias<T, Tag>
    struct coalesce<T, Tag>
    {
        using type = typename Tag::template alias<T>;
    };

    template <typename T, typename Tag>
    using coalesce_t = typename coalesce<T, Tag>::type;
}
