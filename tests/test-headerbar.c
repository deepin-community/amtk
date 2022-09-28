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

#include <amtk/amtk.h>

/* This should be stored instead in the Private struct of the app GtkApplication
 * subclass. But here to not make the code too complicated there is no
 * GtkApplication subclass.
 */
static AmtkActionInfoStore *action_info_store = NULL;

static void
add_action_info_entries (void)
{
	const gchar *accels[] = {NULL, NULL, NULL};
	AmtkActionInfo *action_info;

	const AmtkActionInfoEntry entries[] =
	{
		/* action, icon, label, accel, tooltip */
		{ "win.show-side-panel", NULL, "_Side Panel", "F9",
		  "Toggle side panel visibility" },
		{ "win.print", NULL, "_Print", "<Control>p" },
		{ "win.shortcuts-window", NULL, "_Keyboard Shortcuts" },
		{ NULL }
	};

	g_assert (action_info_store == NULL);
	action_info_store = amtk_action_info_store_new ();

	amtk_action_info_store_add_entries (action_info_store,
					    entries, -1,
					    NULL);

	accels[0] = "<Control>F1";
	accels[1] = "<Control>question";
	action_info = amtk_action_info_store_lookup (action_info_store, "win.shortcuts-window");
	amtk_action_info_set_accels (action_info, accels);
}

static void
startup_cb (GApplication *g_app,
	    gpointer      user_data)
{
	add_action_info_entries ();
}

static void
print_activate_cb (GSimpleAction *action,
		   GVariant      *parameter,
		   gpointer       user_data)
{
	g_print ("Print\n");
}

static void
shortcuts_window_activate_cb (GSimpleAction *action,
			      GVariant      *parameter,
			      gpointer       user_data)
{
	GtkApplicationWindow *app_window = GTK_APPLICATION_WINDOW (user_data);
	GtkShortcutsWindow *shortcuts_window;
	GtkContainer *section;
	GtkContainer *group;
	AmtkFactory *factory;

	/* Create group */
	group = amtk_shortcuts_group_new ("General");

	factory = amtk_factory_new (NULL);
	amtk_factory_set_default_flags (factory, AMTK_FACTORY_IGNORE_GACTION);
	gtk_container_add (group, amtk_factory_create_shortcut (factory, "win.show-side-panel"));
	gtk_container_add (group, amtk_factory_create_shortcut (factory, "win.print"));
	g_object_unref (factory);

	/* Create section and window */
	section = amtk_shortcuts_section_new (NULL);
	gtk_container_add (section, GTK_WIDGET (group));

	shortcuts_window = amtk_shortcuts_window_new (GTK_WINDOW (app_window));
	gtk_container_add (GTK_CONTAINER (shortcuts_window), GTK_WIDGET (section));

	gtk_widget_show_all (GTK_WIDGET (shortcuts_window));
}

static void
add_win_actions (GtkApplicationWindow *window,
		 GtkWidget            *side_panel)
{
	GPropertyAction *side_panel_action;

	const GActionEntry entries[] =
	{
		{ "print", print_activate_cb },
		{ "shortcuts-window", shortcuts_window_activate_cb },
		{ NULL }
	};

	amtk_action_map_add_action_entries_check_dups (G_ACTION_MAP (window),
						       entries, -1,
						       window);

	side_panel_action = g_property_action_new ("show-side-panel", side_panel, "visible");
	g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (side_panel_action));
	g_object_unref (side_panel_action);
}

static GMenuModel *
create_window_menu (void)
{
	GMenu *menu;
	AmtkFactory *factory;

	menu = g_menu_new ();

	factory = amtk_factory_new_with_default_application ();
	amtk_gmenu_append_item (menu, amtk_factory_create_gmenu_item (factory, "win.show-side-panel"));
	amtk_gmenu_append_item (menu, amtk_factory_create_gmenu_item (factory, "win.print"));
	amtk_gmenu_append_item (menu, amtk_factory_create_gmenu_item (factory, "win.shortcuts-window"));
	g_object_unref (factory);

	g_menu_freeze (menu);

	return G_MENU_MODEL (menu);
}

static GtkWidget *
create_header_bar (void)
{
	GtkHeaderBar *header_bar;
	GMenuModel *window_menu;
	GtkMenuButton *menu_button;

	header_bar = GTK_HEADER_BAR (gtk_header_bar_new ());
	gtk_header_bar_set_title (header_bar, "Amtk test headerbar");
	gtk_header_bar_set_show_close_button (header_bar, TRUE);

	/* Menu */
	menu_button = GTK_MENU_BUTTON (gtk_menu_button_new ());
	gtk_menu_button_set_direction (menu_button, GTK_ARROW_NONE);

	window_menu = create_window_menu ();
	gtk_menu_button_set_menu_model (menu_button, window_menu);
	g_object_unref (window_menu);

	gtk_header_bar_pack_end (header_bar, GTK_WIDGET (menu_button));

	return GTK_WIDGET (header_bar);
}

static void
activate_cb (GApplication *g_app,
	     gpointer      user_data)
{
	GtkWidget *window;
	GtkWidget *hgrid;
	GtkWidget *side_panel;

	window = gtk_application_window_new (GTK_APPLICATION (g_app));
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
	gtk_window_set_titlebar (GTK_WINDOW (window), create_header_bar ());

	hgrid = gtk_grid_new ();
	side_panel = gtk_label_new ("Side panel");
	gtk_container_add (GTK_CONTAINER (hgrid), side_panel);
	gtk_container_add (GTK_CONTAINER (hgrid), gtk_label_new ("Text view"));

	add_win_actions (GTK_APPLICATION_WINDOW (window), side_panel);

	gtk_container_add (GTK_CONTAINER (window), hgrid);
	gtk_widget_show_all (window);

	amtk_action_info_store_check_all_used (action_info_store);
}

int
main (int    argc,
      char **argv)
{
	GtkApplication *app;
	int status;

	amtk_init ();

	app = gtk_application_new ("org.gnome.amtk.test-headerbar", G_APPLICATION_FLAGS_NONE);

	g_signal_connect (app,
			  "startup",
			  G_CALLBACK (startup_cb),
			  NULL);

	g_signal_connect (app,
			  "activate",
			  G_CALLBACK (activate_cb),
			  NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);

	amtk_finalize ();
	g_object_unref (app);
	g_clear_object (&action_info_store);
	return status;
}
