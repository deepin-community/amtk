/*
 * This file is part of Amtk - Actions, Menus and Toolbars Kit
 *
 * Copyright 2017, 2018 - Sébastien Wilmet <swilmet@gnome.org>
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

static AmtkActionInfoEntry file_action_info_entries[] =
{
	{ "win.open", "document-open", "_Open", "<Control>o",
	  "Open a file" },

	{ "app.quit", "application-exit", "_Quit", "<Control>q",
	  "Quit the application" },

	{ NULL }
};

static void
add_action_info_entries (void)
{
	const AmtkActionInfoEntry entries[] =
	{
		/* action, icon, label, accel, tooltip */

		{ "app.about", "help-about", "_About", NULL,
		  "About this application" },

		{ "win.show-side-panel", NULL, "_Side Panel", "F9",
		  "Toggle side panel visibility" },
	};

	g_assert (action_info_store == NULL);
	action_info_store = amtk_action_info_store_new ();

	amtk_action_info_store_add_entries (action_info_store,
					    entries,
					    G_N_ELEMENTS (entries),
					    NULL);

	amtk_action_info_store_add_entries (action_info_store,
					    file_action_info_entries, -1,
					    NULL);
}

static void
quit_activate_cb (GSimpleAction *action,
		  GVariant      *parameter,
		  gpointer       user_data)
{
	g_application_quit (G_APPLICATION (user_data));
}

static void
about_activate_cb (GSimpleAction *action,
		   GVariant      *parameter,
		   gpointer       user_data)
{
	g_print ("About\n");
}

static void
add_app_action_entries (GApplication *app)
{
	const GActionEntry entries[] =
	{
		{ "quit", quit_activate_cb },
		{ "about", about_activate_cb },
	};

	amtk_action_map_add_action_entries_check_dups (G_ACTION_MAP (app),
						       entries,
						       G_N_ELEMENTS (entries),
						       app);
}

static void
startup_cb (GApplication *g_app,
	    gpointer      user_data)
{
	add_action_info_entries ();
	add_app_action_entries (g_app);
}

static GtkWidget *
create_view_submenu (void)
{
	GtkMenuShell *view_submenu;
	AmtkFactory *factory;

	view_submenu = GTK_MENU_SHELL (gtk_menu_new ());

	factory = amtk_factory_new_with_default_application ();
	gtk_menu_shell_append (view_submenu, amtk_factory_create_check_menu_item (factory, "win.show-side-panel"));
	g_object_unref (factory);

	return GTK_WIDGET (view_submenu);
}

static GtkWidget *
create_help_submenu (void)
{
	GtkMenuShell *help_submenu;
	AmtkFactory *factory;

	help_submenu = GTK_MENU_SHELL (gtk_menu_new ());

	factory = amtk_factory_new_with_default_application ();
	gtk_menu_shell_append (help_submenu, amtk_factory_create_menu_item (factory, "app.about"));
	g_object_unref (factory);

	return GTK_WIDGET (help_submenu);
}

static GtkMenuBar *
create_menu_bar (void)
{
	AmtkFactory *factory;
	GtkWidget *file_menu_item;
	GtkWidget *view_menu_item;
	GtkWidget *help_menu_item;
	GtkMenuBar *menu_bar;

	factory = amtk_factory_new_with_default_application ();

	file_menu_item = gtk_menu_item_new_with_mnemonic ("_File");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu_item),
				   amtk_factory_create_simple_menu (factory, file_action_info_entries, -1));

	view_menu_item = gtk_menu_item_new_with_mnemonic ("_View");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (view_menu_item),
				   create_view_submenu ());

	help_menu_item = gtk_menu_item_new_with_mnemonic ("_Help");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_menu_item),
				   create_help_submenu ());

	menu_bar = GTK_MENU_BAR (gtk_menu_bar_new ());
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), file_menu_item);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), view_menu_item);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), help_menu_item);

	amtk_action_info_store_check_all_used (action_info_store);

	g_object_unref (factory);
	return menu_bar;
}

static void
add_win_actions (GtkApplicationWindow *window,
		 GtkWidget            *side_panel)
{
	GPropertyAction *side_panel_action;

	side_panel_action = g_property_action_new ("show-side-panel", side_panel, "visible");
	g_action_map_add_action (G_ACTION_MAP (window), G_ACTION (side_panel_action));
	g_object_unref (side_panel_action);
}

static void
activate_cb (GApplication *g_app,
	     gpointer      user_data)
{
	GtkWidget *window;
	GtkWidget *vgrid;
	GtkWidget *hgrid;
	GtkWidget *side_panel;

	window = gtk_application_window_new (GTK_APPLICATION (g_app));
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

	vgrid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);

	gtk_container_add (GTK_CONTAINER (vgrid), GTK_WIDGET (create_menu_bar ()));

	hgrid = gtk_grid_new ();
	side_panel = gtk_label_new ("Side panel");
	gtk_container_add (GTK_CONTAINER (hgrid), side_panel);
	gtk_container_add (GTK_CONTAINER (hgrid), gtk_label_new ("Text view"));
	gtk_container_add (GTK_CONTAINER (vgrid), hgrid);

	add_win_actions (GTK_APPLICATION_WINDOW (window), side_panel);

	gtk_container_add (GTK_CONTAINER (window), vgrid);
	gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
	GtkApplication *app;
	int status;

	amtk_init ();

	app = gtk_application_new ("org.gnome.amtk.test-traditional-ui", G_APPLICATION_FLAGS_NONE);

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
