/*
 * This file is part of Amtk - Actions, Menus and Toolbars Kit
 *
 * Copyright 2017 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * Amtk is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * Amtk is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <amtk/amtk.h>

static void
check_remove_mnemonic (const gchar *str_with_mnemonic,
		       const gchar *expected_str_without_mnemonic)
{
	gchar *str_without_mnemonic;

	str_without_mnemonic = amtk_utils_remove_mnemonic (str_with_mnemonic);
	g_assert_cmpstr (str_without_mnemonic, ==, expected_str_without_mnemonic);
	g_free (str_without_mnemonic);
}

static void
test_remove_mnemonic (void)
{
	check_remove_mnemonic ("", "");
	check_remove_mnemonic ("a", "a");
	check_remove_mnemonic ("_a", "a");
	check_remove_mnemonic ("__a", "_a");
	check_remove_mnemonic ("___a", "_a");
	check_remove_mnemonic ("S_maller Text", "Smaller Text");

	/* With multi-byte UTF-8 chars. */
	check_remove_mnemonic ("___ß_é__c____d", "_ßé_c__d");

	/* Not valid mnemonic, but the function must not crash. */
	check_remove_mnemonic ("a_", "a");
}

int
main (int    argc,
      char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/utils/remove-mnemonic", test_remove_mnemonic);

	return g_test_run ();
}
