/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef EQ_HELPER_H
#define EQ_HELPER_H

namespace ccup {

//////////////////////////////////////////////////////////////
#define __DECL_EQUALS(cls)                \
bool operator!=(const cls& rhs) const;    \
bool operator==(const cls& rhs) const

//////////////////////////////////////////////////////////////
#define __FIELD_EQ(name) this->name == rhs.name
#define __FIELD_LT(name) this->name < rhs.name

//////////////////////////////////////////////////////////////
#define __DEF_EQUALS(cls)                                   \
bool cls::operator!=(const cls& rhs) const                  \
{                                                           \
    return !(*this == rhs);                                 \
}                                                           \
bool cls::operator==(const cls& rhs) const

/////////////////////////////////////////////////////////////
#define __INLINE_EQUALS(cls)                                \
bool operator!=(const cls& rhs) const                       \
{                                                           \
    return !(*this == rhs);                                 \
}                                                           \
bool operator==(const cls& rhs) const

/////////////////////////////////////////////////////////////
#define __DECL_COMP(cls)                                   \
    __DECL_EQUALS(cls);                                    \
    bool operator<(const cls&) const;                      \
    bool operator>(const cls&) const;                      \
    bool operator<=(const cls&) const;                     \
    bool operator>=(const cls&) const

/////////////////////////////////////////////////////////////
#define __DEF_COMP(cls)                                    \
bool cls::operator>(const cls& rhs) const                  \
{                                                          \
    return !(*this <= rhs);                                \
}                                                          \
bool cls::operator>=(const cls& rhs) const                 \
{                                                          \
    return !(*this < rhs);                                 \
}                                                          \
bool cls::operator<=(const cls& rhs) const                 \
{                                                          \
    return (*this < rhs) || (*this == rhs);                \
}                                                          \
bool cls::operator<(const cls& rhs) const

}

#endif
