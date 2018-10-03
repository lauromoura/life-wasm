
#include <iostream>

#include <cstdlib>
#include <check.h>

#include "life.hh"



START_TEST(blinker_test)
{
    Game g(4, 4);

    g.Activate(0, 1);
    g.Activate(1, 1);
    g.Activate(2, 1);

    g.Evolve();


    std::cout << "FAIL!!!!" << std::endl;

    ck_assert_int_eq(g.At(0, 1), false);
    ck_assert_int_eq(g.At(1, 1), true);
    ck_assert_int_eq(g.At(2, 1), false);

    ck_assert_int_eq(g.At(1, 0), true);
    ck_assert_int_eq(g.At(1, 2), true);

    g.Evolve();


    ck_assert_int_eq(g.At(0, 1), true);
    ck_assert_int_eq(g.At(1, 1), true);
    ck_assert_int_eq(g.At(2, 1), true);

    ck_assert_int_eq(g.At(1, 0), false);
    ck_assert_int_eq(g.At(1, 2), false);
}
END_TEST

Suite *game_suite()
{
    Suite *s;
    TCase *tc = NULL;

    tc = tcase_create("Blinker");

    s = suite_create("Game");

    tcase_add_test(tc, blinker_test);
    suite_add_tcase(s, tc);

    return s;
}

int main(int argc, char* argv[])
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = game_suite();

    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
