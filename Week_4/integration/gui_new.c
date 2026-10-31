
#include <gtk/gtk.h>
#include <cairo.h>
#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAXP 10
#define MAXR 10
#define MAXE 200

typedef int (*InitFn)(int, int, const int *, const int *, const int *);
typedef int (*SafetyFn)(int *, int *, int *);
typedef int (*RequestFn)(int, const int *, int *, int *, int *);
typedef int (*WaitingRequestFn)(int, const int *);
typedef int (*DetectFn)(int *);
typedef int (*RecoverFn)(int);
typedef int (*StateFn)(int *, int *, int *, int *, int *);
typedef int (*GraphFn)(int *, int *, int *, int *, int *);
typedef int (*CycleFn)(void);

static void *lib;
static InitFn bridge_init;
static SafetyFn bridge_check_safety;
static RequestFn bridge_request;
static WaitingRequestFn bridge_set_waiting_request;
static DetectFn bridge_detect_deadlock;
static RecoverFn bridge_recover;
static StateFn bridge_get_state;
static GraphFn bridge_get_graph;
static CycleFn bridge_graph_has_cycle;

static GtkWidget *window, *output, *graph_area;
static GtkWidget *process_combo, *recover_combo;
static GtkSpinButton *req_spin[MAXR];
static GtkSpinButton *allocation_spin[MAXP][MAXR];
static GtkSpinButton *maximum_spin[MAXP][MAXR];
static GtkSpinButton *available_spin[MAXR];
static int process_count = 5, resource_count = 3;
static int edge_count, edge_types[MAXE], edge_process[MAXE];
static int edge_resource[MAXE], edge_instances[MAXE];
static char last_message[8192] = "Welcome to the Deadlock Simulation System.";

static int allocation[MAXP * MAXR];
static int maximum[MAXP * MAXR];
static int available[MAXR] = {3, 3, 2};

static void show_message(const char *message)
{
    GtkTextBuffer *buffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(output));
    gtk_text_buffer_set_text(buffer, message, -1);
}

static void append_state(char *buf, size_t size)
{
    int a[MAXP * MAXR] = {0}, m[MAXP * MAXR] = {0};
    int n[MAXP * MAXR] = {0}, av[MAXR] = {0};
    int rq[MAXP * MAXR] = {0};

    if (!bridge_get_state(a, m, n, av, rq))
        return;

    size_t used = strlen(buf);

    snprintf(buf + used, size - used,
             "\n\nCURRENT SYSTEM STATE\n"
             "--------------------\n");

    used = strlen(buf + 0);

    for (int p = 0; p < process_count; p++) {
        used = strlen(buf);
        snprintf(buf + used, size - used, "P%d  Allocation: [", p);
        for (int r = 0; r < resource_count; r++) {
            used = strlen(buf);
            snprintf(buf + used, size - used, "%s%d",
                     r ? ", " : "", a[p * MAXR + r]);
        }

        used = strlen(buf);
        snprintf(buf + used, size - used, "]  Need: [");
        for (int r = 0; r < resource_count; r++) {
            used = strlen(buf);
            snprintf(buf + used, size - used, "%s%d",
                     r ? ", " : "", n[p * MAXR + r]);
        }

        used = strlen(buf);
        snprintf(buf + used, size - used, "]  Request: [");
        for (int r = 0; r < resource_count; r++) {
            used = strlen(buf);
            snprintf(buf + used, size - used, "%s%d",
                     r ? ", " : "", rq[p * MAXR + r]);
        }

        used = strlen(buf);
        snprintf(buf + used, size - used, "]\n");
    }

    used = strlen(buf);
    snprintf(buf + used, size - used, "Available: [");
    for (int r = 0; r < resource_count; r++) {
        used = strlen(buf);
        snprintf(buf + used, size - used, "%s%d",
                 r ? ", " : "", av[r]);
    }

    used = strlen(buf);
    snprintf(buf + used, size - used, "]\n");
}

static void refresh_graph(void)
{
    edge_count = 0;
    bridge_get_graph(&edge_count, edge_types, edge_process,
                     edge_resource, edge_instances);
    gtk_widget_queue_draw(graph_area);
}

static void safety_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int safe = 0, sequence[MAXP] = {0}, length = 0;
    int status = bridge_check_safety(&safe, sequence, &length);

    char buf[8192];
    snprintf(buf, sizeof(buf),
             "BANKER'S ALGORITHM SAFETY CHECK\n"
             "================================\n");

    if (status != 0) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Safety check failed. Backend status: %d\n", status);
    } else if (safe) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Result: SAFE STATE\nSafe sequence: ");
        for (int i = 0; i < length; i++) {
            snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                     "%sP%d", i ? " -> " : "", sequence[i]);
        }
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "\n");
    } else {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Result: UNSAFE STATE\nNo complete safe sequence exists.\n");
    }

    append_state(buf, sizeof(buf));
    show_message(buf);
    g_strlcpy(last_message, buf, sizeof(last_message));
    refresh_graph();
}

static void detect_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int deadlocked[MAXP] = {0};
    int count = bridge_detect_deadlock(deadlocked);

    char buf[8192];
    snprintf(buf, sizeof(buf),
             "DEADLOCK DETECTION\n"
             "==================\n");

    if (count < 0) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Detection failed.\n");
    } else if (count == 0) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "No deadlocked processes detected.\n");
    } else {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Deadlocked processes (%d): ", count);
        int first = 1;
        for (int p = 0; p < process_count; p++) {
            if (deadlocked[p]) {
                snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                         "%sP%d", first ? "" : ", ", p);
                first = 0;
            }
        }
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "\n");
    }

    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
             "Resource Allocation Graph cycle: %s\n",
             bridge_graph_has_cycle() ? "YES" : "NO");

    append_state(buf, sizeof(buf));
    show_message(buf);
    g_strlcpy(last_message, buf, sizeof(last_message));
    refresh_graph();
}

static void request_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int p = gtk_combo_box_get_active(GTK_COMBO_BOX(process_combo));
    if (p < 0 || p >= process_count)
        return;

    int req[MAXR] = {0};
    for (int r = 0; r < resource_count; r++)
        req[r] = gtk_spin_button_get_value_as_int(req_spin[r]);

    int safe = 0, sequence[MAXP] = {0}, length = 0;
    int status = bridge_request(p, req, &safe, sequence, &length);

    char buf[8192];
    snprintf(buf, sizeof(buf),
             "RESOURCE REQUEST\n"
             "================\n"
             "Process: P%d\nRequest: [", p);

    for (int r = 0; r < resource_count; r++) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "%s%d", r ? ", " : "", req[r]);
    }

    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
             "]\nBackend status: %d\n", status);

    if (status == 0 && safe) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Request granted. Resulting state is safe.\nSequence: ");
        for (int i = 0; i < length; i++) {
            snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                     "%sP%d", i ? " -> " : "", sequence[i]);
        }
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "\n");
    } else {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Request not granted. Check the status and safety result.\n");
    }

    append_state(buf, sizeof(buf));
    show_message(buf);
    g_strlcpy(last_message, buf, sizeof(last_message));
    refresh_graph();
}

static void apply_matrix_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int a[MAXP * MAXR] = {0};
    int m[MAXP * MAXR] = {0};
    int av[MAXR] = {0};

    for (int r = 0; r < resource_count; r++) {
        av[r] = gtk_spin_button_get_value_as_int(available_spin[r]);
        if (av[r] < 0) {
            show_message("Available values cannot be negative.");
            return;
        }
    }

    for (int p = 0; p < process_count; p++) {
        for (int r = 0; r < resource_count; r++) {
            int index = p * MAXR + r;
            a[index] = gtk_spin_button_get_value_as_int(allocation_spin[p][r]);
            m[index] = gtk_spin_button_get_value_as_int(maximum_spin[p][r]);

            if (a[index] < 0 || m[index] < 0) {
                show_message("Matrix values cannot be negative.");
                return;
            }

            if (a[index] > m[index]) {
                char message[160];
                snprintf(message, sizeof(message),
                         "Invalid values: Allocation exceeds Maximum for P%d, R%d.",
                         p, r);
                show_message(message);
                return;
            }
        }
    }

    int status = bridge_init(process_count, resource_count, a, m, av);
    if (status != 0) {
        char message[160];
        snprintf(message, sizeof(message),
                 "Backend rejected the configuration. Status: %d", status);
        show_message(message);
        return;
    }

    memcpy(allocation, a, sizeof(allocation));
    memcpy(maximum, m, sizeof(maximum));
    memcpy(available, av, sizeof(available));

    char message[256];
    snprintf(message, sizeof(message),
             "Configuration applied successfully.\n"
             "Processes: %d | Resource types: %d\n"
             "Run Check System Safety to calculate the safe sequence.",
             process_count, resource_count);
    show_message(message);
    g_strlcpy(last_message, message, sizeof(last_message));
    refresh_graph();
}

static void waiting_request_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int p = gtk_combo_box_get_active(GTK_COMBO_BOX(process_combo));
    if (p < 0 || p >= process_count)
        return;

    int req[MAXR] = {0};
    for (int r = 0; r < resource_count; r++)
        req[r] = gtk_spin_button_get_value_as_int(req_spin[r]);

    int status = bridge_set_waiting_request(p, req);

    char buf[8192];
    snprintf(buf, sizeof(buf),
             "WAITING RESOURCE REQUEST\n"
             "========================\n"
             "Process: P%d\n"
             "Request: [", p);

    for (int r = 0; r < resource_count; r++) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "%s%d", r ? ", " : "", req[r]);
    }

    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
             "]\nBackend status: %d\n", status);

    if (status == 0) {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Waiting request recorded successfully. "
                 "Resources have not been granted.\n");
    } else {
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf),
                 "Waiting request could not be recorded. "
                 "Check the request values and process state.\n");
    }

    append_state(buf, sizeof(buf));
    show_message(buf);
    g_strlcpy(last_message, buf, sizeof(last_message));
    refresh_graph();
}

static void recover_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int p = gtk_combo_box_get_active(GTK_COMBO_BOX(recover_combo));
    if (p < 0 || p >= process_count)
        return;

    int result = bridge_recover(p);
    char buf[8192];

    snprintf(buf, sizeof(buf),
             "PROCESS RECOVERY\n"
             "================\n"
             "Process: P%d\nRecovery result: %s (code %d)\n",
             p, result != 0 ? "SUCCESS" : "FAILED", result);

    append_state(buf, sizeof(buf));
    show_message(buf);
    g_strlcpy(last_message, buf, sizeof(last_message));
    refresh_graph();
}

static void draw_arrow(cairo_t *cr, double x1, double y1,
                       double x2, double y2)
{
    cairo_move_to(cr, x1, y1);
    cairo_line_to(cr, x2, y2);
    cairo_stroke(cr);

    double angle = atan2(y2 - y1, x2 - x1);
    double len = 9.0;

    cairo_move_to(cr, x2, y2);
    cairo_line_to(cr,
                  x2 - len * cos(angle - 0.45),
                  y2 - len * sin(angle - 0.45));
    cairo_line_to(cr,
                  x2 - len * cos(angle + 0.45),
                  y2 - len * sin(angle + 0.45));
    cairo_close_path(cr);
    cairo_fill(cr);
}

static gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    (void)data;

    GtkAllocation al;
    gtk_widget_get_allocation(widget, &al);

    double w = al.width, h = al.height;
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 13);
    cairo_set_source_rgb(cr, 0.12, 0.17, 0.28);
    cairo_move_to(cr, 16, 22);
    cairo_show_text(cr, "RESOURCE ALLOCATION GRAPH");

    double top = 55;
    double usable_h = h - top - 20;
    if (usable_h < 50)
        usable_h = 50;

    double px = w * 0.25;
    double rx = w * 0.73;
    double p_y[MAXP], r_y[MAXR];

    for (int p = 0; p < process_count; p++)
        p_y[p] = top + usable_h * (p + 0.5) / process_count;

    for (int r = 0; r < resource_count; r++)
        r_y[r] = top + usable_h * (r + 0.5) / resource_count;

    for (int e = 0; e < edge_count && e < MAXE; e++) {
        int p = edge_process[e], r = edge_resource[e];
        if (p < 0 || p >= process_count ||
            r < 0 || r >= resource_count)
            continue;

        if (edge_types[e] == 0) {
            cairo_set_source_rgb(cr, 0.12, 0.38, 0.82);
            draw_arrow(cr, rx - 19, r_y[r], px + 20, p_y[p]);
        } else {
            cairo_set_source_rgb(cr, 0.92, 0.43, 0.10);
            draw_arrow(cr, px + 20, p_y[p], rx - 19, r_y[r]);
        }

        cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
        cairo_set_font_size(cr, 10);
        cairo_move_to(cr, (px + rx) / 2 - 8,
                      (p_y[p] + r_y[r]) / 2 - 3);
        char label[24];
        snprintf(label, sizeof(label), "%d", edge_instances[e]);
        cairo_show_text(cr, label);
    }

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 12);

    for (int p = 0; p < process_count; p++) {
        cairo_set_source_rgb(cr, 0.85, 0.91, 1.0);
        cairo_arc(cr, px, p_y[p], 19, 0, 2 * G_PI);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.12, 0.30, 0.65);
        cairo_stroke(cr);
        cairo_move_to(cr, px - 11, p_y[p] + 4);
        char label[16];
        snprintf(label, sizeof(label), "P%d", p);
        cairo_show_text(cr, label);
    }

    for (int r = 0; r < resource_count; r++) {
        cairo_set_source_rgb(cr, 1.0, 0.88, 0.67);
        cairo_rectangle(cr, rx - 19, r_y[r] - 17, 38, 34);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.72, 0.37, 0.08);
        cairo_stroke(cr);
        cairo_move_to(cr, rx - 11, r_y[r] + 4);
        char label[16];
        snprintf(label, sizeof(label), "R%d", r);
        cairo_show_text(cr, label);
    }

    cairo_set_font_size(cr, 10);
    cairo_set_source_rgb(cr, 0.12, 0.38, 0.82);
    cairo_move_to(cr, 15, h - 8);
    cairo_show_text(cr, "Blue: allocated resource -> process");

    cairo_set_source_rgb(cr, 0.92, 0.43, 0.10);
    cairo_move_to(cr, w * 0.52, h - 8);
    cairo_show_text(cr, "Orange: process -> request");

    return FALSE;
}

static void load_symbol(void **target, const char *name)
{
    *target = dlsym(lib, name);
}

#define LOAD(fn) load_symbol((void **)&fn, #fn)


static void show_need_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int a[MAXP * MAXR] = {0}, m[MAXP * MAXR] = {0};
    int need[MAXP * MAXR] = {0}, av[MAXR] = {0};
    int requests[MAXP * MAXR] = {0};

    if (!bridge_get_state(a, m, need, av, requests)) {
        show_message("Unable to retrieve the Need matrix.");
        return;
    }

    char buf[8192] = "NEED MATRIX (Maximum - Allocation)\n"
                    "==================================\n";

    for (int p = 0; p < process_count; p++) {
        size_t used = strlen(buf);
        snprintf(buf + used, sizeof(buf) - used, "P%d: [", p);

        for (int r = 0; r < resource_count; r++) {
            used = strlen(buf);
            snprintf(buf + used, sizeof(buf) - used, "%s%d",
                     r ? ", " : "", need[p * MAXR + r]);
        }

        used = strlen(buf);
        snprintf(buf + used, sizeof(buf) - used, "]\n");
    }

    append_state(buf, sizeof(buf));
    show_message(buf);
    g_strlcpy(last_message, buf, sizeof(last_message));
}

static void refresh_graph_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;
    refresh_graph();
}

static void system_status_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int safe = 0, sequence[MAXP] = {0}, length = 0;
    int status = bridge_check_safety(&safe, sequence, &length);

    char buf[8192];
    snprintf(buf, sizeof(buf),
             "SYSTEM STATUS\n"
             "=============\n"
             "Processes: %d\n"
             "Resource types: %d\n"
             "Banker's safety status: %s\n",
             process_count, resource_count,
             status == 0 ? (safe ? "SAFE" : "UNSAFE") : "CHECK FAILED");

    append_state(buf, sizeof(buf));
    show_message(buf);
    g_strlcpy(last_message, buf, sizeof(last_message));
    refresh_graph();
}

static void clear_output_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;
    show_message("");
    last_message[0] = '\0';
}

static void exit_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;
    gtk_main_quit();
}


static void create_processes_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;
    char message[128];
    snprintf(message, sizeof(message),
             "PROCESS CONFIGURATION\nProcesses available: %d\nProcess IDs: P0 to P%d",
             process_count, process_count - 1);
    show_message(message);
}

static void create_resources_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;
    char message[128];
    snprintf(message, sizeof(message),
             "RESOURCE CONFIGURATION\nResource types: %d\nResource IDs: R0 to R%d",
             resource_count, resource_count - 1);
    show_message(message);
}

static void set_allocation_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;
    gtk_widget_grab_focus(GTK_WIDGET(allocation_spin[0][0]));
    show_message("Edit Allocation values, then click Apply Configuration.");
}

static void set_maximum_clicked(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;
    gtk_widget_grab_focus(GTK_WIDGET(maximum_spin[0][0]));
    show_message("Edit Maximum values, then click Apply Configuration.");
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    lib = dlopen("./build/libsimulator.so", RTLD_NOW);
    if (!lib) {
        fprintf(stderr, "Cannot load backend: %s\n", dlerror());
        return 1;
    }

    LOAD(bridge_init);
    LOAD(bridge_check_safety);
    LOAD(bridge_request);
    LOAD(bridge_set_waiting_request);
    LOAD(bridge_detect_deadlock);
    LOAD(bridge_recover);
    LOAD(bridge_get_state);
    LOAD(bridge_get_graph);
    LOAD(bridge_graph_has_cycle);

    if (!bridge_init || !bridge_check_safety || !bridge_request ||
        !bridge_set_waiting_request || !bridge_detect_deadlock ||
        !bridge_recover || !bridge_get_state || !bridge_get_graph ||
        !bridge_graph_has_cycle) {
        fprintf(stderr, "A required bridge function is missing.\n");
        return 1;
    }

    int a[MAXP * MAXR] = {0};
    int m[MAXP * MAXR] = {0};
    int av[MAXR] = {3, 3, 2};

    int aa[5][3] = {
        {0, 1, 0}, {2, 0, 0}, {3, 0, 2},
        {2, 1, 1}, {0, 0, 2}
    };

    int mm[5][3] = {
        {7, 5, 3}, {3, 2, 2}, {9, 0, 2},
        {2, 2, 2}, {4, 3, 3}
    };

    for (int p = 0; p < process_count; p++) {
        for (int r = 0; r < resource_count; r++) {
            a[p * MAXR + r] = aa[p][r];
            m[p * MAXR + r] = mm[p][r];
        }
    }

    memcpy(allocation, a, sizeof(a));
    memcpy(maximum, m, sizeof(m));
    memcpy(available, av, sizeof(available));

    if (bridge_init(process_count, resource_count, a, m, av) != 0) {
        fprintf(stderr, "Backend initialization failed.\n");
        return 1;
    }

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Interactive Deadlock Simulation System");
    gtk_window_set_default_size(GTK_WINDOW(window), 1250, 820);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *css = gtk_css_provider_new();
    const char *style =
        "window { background: #f1f5f9; }"
        "frame { border: 1px solid #cbd5e1; border-radius: 8px; }"
        "frame > label { color: #17365d; font-weight: bold; }"
        "button { padding: 9px; border-radius: 6px;"
        " background: #e2eaf5; color: #17365d; font-weight: bold; }"
        "button:hover { background: #cbdcf2; }"
        "textview { background: #ffffff; color: #172033; }";

    gtk_css_provider_load_from_data(css, style, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(root), 16);
    gtk_container_add(GTK_CONTAINER(window), root);

    GtkWidget *header = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(root), header, FALSE, FALSE, 0);

    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title),
        "<span size='xx-large' weight='bold' foreground='#17365d'>"
        "INTERACTIVE DEADLOCK SIMULATION SYSTEM</span>");
    gtk_box_pack_start(GTK_BOX(header), title, FALSE, FALSE, 0);

    GtkWidget *subtitle = gtk_label_new(
        "OPERATING SYSTEMS  |  BANKER'S ALGORITHM  |  DEADLOCK DETECTION");
    gtk_box_pack_start(GTK_BOX(header), subtitle, FALSE, FALSE, 0);

    GtkWidget *status = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(status),
        "<span weight='bold' foreground='#16803c'>"
        "● SYSTEM STATUS: READY</span>");
    gtk_box_pack_start(GTK_BOX(root), status, FALSE, FALSE, 5);

    GtkWidget *columns = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 14);
    gtk_box_pack_start(GTK_BOX(root), columns, TRUE, TRUE, 0);

    /* LEFT SIDE */
    /* LEFT SIDE - SCROLLABLE */
    GtkWidget *left_scroll = gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(left_scroll),
        GTK_POLICY_NEVER,
        GTK_POLICY_AUTOMATIC
    );

    gtk_widget_set_size_request(left_scroll, 570, -1);
    gtk_box_pack_start(GTK_BOX(columns), left_scroll, TRUE, TRUE, 0);

    GtkWidget *left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_add(GTK_CONTAINER(left_scroll), left);

    GtkWidget *controls_frame = gtk_frame_new("Simulation Controls");
    gtk_box_pack_start(GTK_BOX(left), controls_frame, FALSE, FALSE, 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 12);
    gtk_container_add(GTK_CONTAINER(controls_frame), grid);

    GtkWidget *b;

#define ADD_BUTTON(label, callback, col, row) \
    b = gtk_button_new_with_label(label); \
    gtk_widget_set_hexpand(b, TRUE); \
    g_signal_connect(b, "clicked", G_CALLBACK(callback), NULL); \
    gtk_grid_attach(GTK_GRID(grid), b, col, row, 1, 1)

    ADD_BUTTON("Create Processes", create_processes_clicked, 0, 0);
    ADD_BUTTON("Create Resources", create_resources_clicked, 1, 0);

    ADD_BUTTON("Set Allocation", set_allocation_clicked, 0, 1);
    ADD_BUTTON("Set Maximum", set_maximum_clicked, 1, 1);

    ADD_BUTTON("Show Need", show_need_clicked, 0, 2);
    ADD_BUTTON("Request Resources", request_clicked, 1, 2);

    ADD_BUTTON("Banker's Algorithm", safety_clicked, 0, 3);
    ADD_BUTTON("Detect Deadlock", detect_clicked, 1, 3);

    ADD_BUTTON("Recover / Terminate", recover_clicked, 0, 4);
    ADD_BUTTON("Record Waiting Request", waiting_request_clicked, 1, 4);

    ADD_BUTTON("Show System Status", system_status_clicked, 0, 5);
    ADD_BUTTON("Refresh Graph", refresh_graph_clicked, 1, 5);

    ADD_BUTTON("Clear Output", clear_output_clicked, 0, 6);
    ADD_BUTTON("Exit", exit_clicked, 1, 6);

#undef ADD_BUTTON

    /* RESOURCE REQUEST */
    GtkWidget *request_frame = gtk_frame_new("Resource Request");
    gtk_box_pack_start(GTK_BOX(left), request_frame, FALSE, FALSE, 0);

    GtkWidget *request_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(request_box), 10);
    gtk_container_add(GTK_CONTAINER(request_frame), request_box);

    process_combo = gtk_combo_box_text_new();
    for (int p = 0; p < process_count; p++) {
        char name[16];
        snprintf(name, sizeof(name), "P%d", p);
        gtk_combo_box_text_append_text(
            GTK_COMBO_BOX_TEXT(process_combo), name);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(process_combo), 0);
    gtk_box_pack_start(GTK_BOX(request_box), process_combo, TRUE, TRUE, 0);

    for (int r = 0; r < resource_count; r++) {
        char label[16];
        snprintf(label, sizeof(label), "R%d", r);
        gtk_box_pack_start(GTK_BOX(request_box),
                           gtk_label_new(label), FALSE, FALSE, 0);

        req_spin[r] = GTK_SPIN_BUTTON(
            gtk_spin_button_new_with_range(0, 100, 1));
        gtk_box_pack_start(GTK_BOX(request_box),
                           GTK_WIDGET(req_spin[r]), FALSE, FALSE, 0);
    }

    /* RECOVERY */
    GtkWidget *recovery_frame = gtk_frame_new("Process Recovery");
    gtk_box_pack_start(GTK_BOX(left), recovery_frame, FALSE, FALSE, 0);

    GtkWidget *recovery_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(recovery_box), 10);
    gtk_container_add(GTK_CONTAINER(recovery_frame), recovery_box);

    recover_combo = gtk_combo_box_text_new();
    for (int p = 0; p < process_count; p++) {
        char name[16];
        snprintf(name, sizeof(name), "P%d", p);
        gtk_combo_box_text_append_text(
            GTK_COMBO_BOX_TEXT(recover_combo), name);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(recover_combo), 0);
    gtk_box_pack_start(GTK_BOX(recovery_box), recover_combo, TRUE, TRUE, 0);

    b = gtk_button_new_with_label("Recover Process");
    g_signal_connect(b, "clicked", G_CALLBACK(recover_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(recovery_box), b, TRUE, TRUE, 0);


    /* RESOURCE CONFIGURATION */
    GtkWidget *matrix_frame = gtk_frame_new("Resource Configuration");
    gtk_box_pack_start(GTK_BOX(left), matrix_frame, FALSE, FALSE, 0);

    GtkWidget *matrix_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(matrix_scroll),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(matrix_scroll, -1, 240);
    gtk_container_add(GTK_CONTAINER(matrix_frame), matrix_scroll);

    GtkWidget *matrix_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(matrix_box), 8);
    gtk_container_add(GTK_CONTAINER(matrix_scroll), matrix_box);

    GtkWidget *matrix_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(matrix_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(matrix_grid), 6);
    gtk_box_pack_start(GTK_BOX(matrix_box), matrix_grid, FALSE, FALSE, 0);

    gtk_grid_attach(GTK_GRID(matrix_grid), gtk_label_new("Process"),
                    0, 0, 1, 1);

    for (int r = 0; r < resource_count; r++) {
        char label[16];
        snprintf(label, sizeof(label), "R%d", r);
        gtk_grid_attach(GTK_GRID(matrix_grid), gtk_label_new(label),
                        r + 1, 0, 1, 1);
    }

    gtk_grid_attach(GTK_GRID(matrix_grid), gtk_label_new("Allocation"),
                    0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(matrix_grid), gtk_label_new("Maximum"),
                    0, process_count + 2, 1, 1);

    for (int p = 0; p < process_count; p++) {
        char label[16];
        snprintf(label, sizeof(label), "P%d", p);

        gtk_grid_attach(GTK_GRID(matrix_grid), gtk_label_new(label),
                        0, p + 2, 1, 1);
        gtk_grid_attach(GTK_GRID(matrix_grid), gtk_label_new(label),
                        0, process_count + p + 3, 1, 1);

        for (int r = 0; r < resource_count; r++) {
            allocation_spin[p][r] = GTK_SPIN_BUTTON(
                gtk_spin_button_new_with_range(0, 100, 1));
            maximum_spin[p][r] = GTK_SPIN_BUTTON(
                gtk_spin_button_new_with_range(0, 100, 1));

            gtk_spin_button_set_value(allocation_spin[p][r],
                                      allocation[p * MAXR + r]);
            gtk_spin_button_set_value(maximum_spin[p][r],
                                      maximum[p * MAXR + r]);

            gtk_grid_attach(GTK_GRID(matrix_grid),
                            GTK_WIDGET(allocation_spin[p][r]),
                            r + 1, p + 2, 1, 1);
            gtk_grid_attach(GTK_GRID(matrix_grid),
                            GTK_WIDGET(maximum_spin[p][r]),
                            r + 1, process_count + p + 3, 1, 1);
        }
    }

    GtkWidget *available_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(matrix_box), available_row, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(available_row), gtk_label_new("Available:"),
                       FALSE, FALSE, 0);

    for (int r = 0; r < resource_count; r++) {
        char label[16];
        snprintf(label, sizeof(label), "R%d", r);

        gtk_box_pack_start(GTK_BOX(available_row), gtk_label_new(label),
                           FALSE, FALSE, 0);

        available_spin[r] = GTK_SPIN_BUTTON(
            gtk_spin_button_new_with_range(0, 100, 1));
        gtk_spin_button_set_value(available_spin[r], available[r]);

        gtk_box_pack_start(GTK_BOX(available_row),
                           GTK_WIDGET(available_spin[r]), FALSE, FALSE, 0);
    }

    b = gtk_button_new_with_label("Apply Configuration");
    g_signal_connect(b, "clicked", G_CALLBACK(apply_matrix_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(matrix_box), b, FALSE, FALSE, 0);

    /* RESOURCE ALLOCATION GRAPH */
    GtkWidget *graph_frame = gtk_frame_new("Resource Allocation Graph");
    gtk_box_pack_start(GTK_BOX(left), graph_frame, TRUE, TRUE, 0);

    graph_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(graph_area, 550, 340);
    gtk_container_add(GTK_CONTAINER(graph_frame), graph_area);
    g_signal_connect(graph_area, "draw", G_CALLBACK(draw_graph), NULL);

    /* RIGHT SIDE: OUTPUT */
    GtkWidget *right_frame = gtk_frame_new("Simulation Output");
    gtk_widget_set_size_request(right_frame, 430, -1);
    gtk_box_pack_start(GTK_BOX(columns), right_frame, TRUE, TRUE, 0);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);

    output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(output), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(output), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(output), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(output), 12);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(output), 12);

    gtk_container_add(GTK_CONTAINER(scroll), output);
    gtk_container_add(GTK_CONTAINER(right_frame), scroll);

    refresh_graph();
    show_message(last_message);

    gtk_widget_show_all(window);
    gtk_main();

    dlclose(lib);
    return 0;
}
