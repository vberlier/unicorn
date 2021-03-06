#ifndef NARWHAL_FIXTURE_H
#define NARWHAL_FIXTURE_H

#include <stdlib.h>

#include "narwhal/test/types.h"
#include "narwhal/types.h"

extern NarwhalCollection *_narwhal_current_fixtures;

struct NarwhalTestFixture
{
    const char *name;
    size_t size;
    void *value;
    NarwhalTestFixtureSetup setup;
    NarwhalTestFixtureCleanup cleanup;
    NarwhalTest *test;
    NarwhalCollection *accessible_fixtures;
    NarwhalCollection *accessible_params;
};

NarwhalTestFixture *narwhal_new_test_fixture(const char *name,
                                             size_t fixture_size,
                                             NarwhalTestFixtureSetup setup,
                                             NarwhalTest *test,
                                             NarwhalTestModifierRegistration *test_modifiers,
                                             size_t modifier_count);
NarwhalTestFixture *narwhal_get_test_fixture(const NarwhalCollection *fixtures,
                                             const char *fixture_name);
void narwhal_free_test_fixture(NarwhalTestFixture *test_fixture);

#define DECLARE_FIXTURE(fixture_name, fixture_type)            \
    typedef fixture_type _narwhal_fixture_type_##fixture_name; \
    extern NarwhalTestModifierRegistration fixture_name

#define TEST_FIXTURE(fixture_name, fixture_type, ...)                                            \
    DECLARE_FIXTURE(fixture_name, fixture_type);                                                 \
    void _narwhal_fixture_##fixture_name##_cleanup(                                              \
        _narwhal_fixture_type_##fixture_name *fixture_name,                                      \
        NarwhalTestFixture *_narwhal_test_fixture);                                              \
    void _narwhal_fixture_##fixture_name##_call_cleanup(void *value,                             \
                                                        NarwhalTestFixture *test_fixture);       \
    static void _narwhal_fixture_##fixture_name##_setup(                                         \
        _narwhal_fixture_type_##fixture_name *fixture_name,                                      \
        NarwhalTestFixture *_narwhal_test_fixture);                                              \
    static void _narwhal_fixture_##fixture_name##_call_setup(void *value,                        \
                                                             NarwhalTestFixture *test_fixture)   \
    {                                                                                            \
        _narwhal_fixture_##fixture_name##_setup((_narwhal_fixture_type_##fixture_name *)value,   \
                                                test_fixture);                                   \
    }                                                                                            \
    void _narwhal_fixture_registration_##fixture_name(NarwhalTest *test,                         \
                                                      _NARWHAL_UNUSED NarwhalCollection *params, \
                                                      NarwhalCollection *fixtures,               \
                                                      _NARWHAL_UNUSED void *args)                \
    {                                                                                            \
        NarwhalTestModifierRegistration modifiers[] = { __VA_ARGS__ };                           \
        narwhal_register_test_fixture(test,                                                      \
                                      fixtures,                                                  \
                                      #fixture_name,                                             \
                                      sizeof(_narwhal_fixture_type_##fixture_name),              \
                                      _narwhal_fixture_##fixture_name##_call_setup,              \
                                      modifiers,                                                 \
                                      sizeof(modifiers) / sizeof(*modifiers));                   \
    }                                                                                            \
    NarwhalTestModifierRegistration fixture_name = {                                             \
        _narwhal_fixture_registration_##fixture_name, NULL                                       \
    };                                                                                           \
    static void _narwhal_fixture_##fixture_name##_setup(                                         \
        _NARWHAL_UNUSED _narwhal_fixture_type_##fixture_name *fixture_name,                      \
        _NARWHAL_UNUSED NarwhalTestFixture *_narwhal_test_fixture)

#define GET_FIXTURE(fixture_name)                                                                 \
    _narwhal_fixture_type_##fixture_name fixture_name;                                            \
    do                                                                                            \
    {                                                                                             \
        NarwhalTestFixture *_narwhal_test_fixture_##fixture_name =                                \
            narwhal_get_test_fixture(_narwhal_current_fixtures, #fixture_name);                   \
        if (_narwhal_test_fixture_##fixture_name == NULL)                                         \
        {                                                                                         \
            FAIL("Fixture \"%s\" hasn't been applied to the current context.", #fixture_name);    \
        }                                                                                         \
        fixture_name =                                                                            \
            *(_narwhal_fixture_type_##fixture_name *)_narwhal_test_fixture_##fixture_name->value; \
    } while (0)

#define CLEANUP_FIXTURE(fixture_name)                                                            \
                                                                                                 \
    _narwhal_test_fixture->cleanup = _narwhal_fixture_##fixture_name##_call_cleanup;             \
    }                                                                                            \
    void _narwhal_fixture_##fixture_name##_call_cleanup(void *value,                             \
                                                        NarwhalTestFixture *test_fixture)        \
    {                                                                                            \
        _narwhal_fixture_##fixture_name##_cleanup((_narwhal_fixture_type_##fixture_name *)value, \
                                                  test_fixture);                                 \
    }                                                                                            \
    void _narwhal_fixture_##fixture_name##_cleanup(                                              \
        _NARWHAL_UNUSED _narwhal_fixture_type_##fixture_name *fixture_name,                      \
        _NARWHAL_UNUSED NarwhalTestFixture *_narwhal_test_fixture)                               \
    {
#endif
