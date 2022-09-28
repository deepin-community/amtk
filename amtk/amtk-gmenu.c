/*
 * This file is part of Amtk - Actions, Menus and Toolbars Kit
 *
 * Copyright 2018 - Sébastien Wilmet <swilmet@gnome.org>
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

#include "amtk-gmenu.h"

/**
 * SECTION:amtk-gmenu
 * @Short_description: #GMenu convenience functions
 * @Title: AmtkGmenu
 *
 * #GMenu convenience functions.
 */

/**
 * amtk_gmenu_append_item:
 * @menu: a #GMenu.
 * @item: (transfer full): a #GMenuItem to append.
 *
 * Like g_menu_append_item() but with (transfer full) for the @item parameter.
 *
 * Since: 5.0
 */
void
amtk_gmenu_append_item (GMenu     *menu,
			GMenuItem *item)
{
	g_return_if_fail (G_IS_MENU (menu));
	g_return_if_fail (G_IS_MENU_ITEM (item));

	g_menu_append_item (menu, item);
	g_object_unref (item);
}

/**
 * amtk_gmenu_append_section:
 * @menu: a #GMenu.
 * @label: (nullable): the section label, or %NULL.
 * @section: (transfer full): a #GMenu with the items of the section.
 *
 * Like g_menu_append_section() but with (transfer full) and a different type
 * for the @section parameter, and calls g_menu_freeze() on @section.
 *
 * Since: 5.0
 */
void
amtk_gmenu_append_section (GMenu       *menu,
			   const gchar *label,
			   GMenu       *section)
{
	g_return_if_fail (G_IS_MENU (menu));
	g_return_if_fail (G_IS_MENU (section));

	g_menu_freeze (section);
	g_menu_append_section (menu, label, G_MENU_MODEL (section));
	g_object_unref (section);
}
