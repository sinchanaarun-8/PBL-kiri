#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simulator.h"

static GtkWidget *pages_entry;
static GtkWidget *frames_spin;
static GtkWidget *algorithm_combo;
static GtkWidget *output_view;
static GtkWidget *graph_area;

static int parse_pages(const char *input, int pages[], int *count)
{
    char *copy = g_strdup(input);
    char *token;
    char *saveptr = NULL;
    int n = 0;
    int valid = 1;

    token = strtok_r(copy, " ,\t\n", &saveptr);

    while (token != NULL) {
        char *end = NULL;
        long value;

        if (n >= MAX_REFERENCES || token[0] == '\0') {
            valid = 0;
            break;
        }

        for (char *p = token; *p; p++) {
            if (!isdigit((unsigned char)*p)) {
                valid = 0;
                break;
            }
        }

        if (!valid)
            break;

        value = strtol(token, &end, 10);

        if (*end != '\0' || value > 2147483647L) {
            valid = 0;
            break;
        }

        pages[n++] = (int)value;
        token = strtok_r(NULL, " ,\t\n", &saveptr);
    }

    g_free(copy);

    if (n == 0)
        valid = 0;

    if (valid)
        *count = n;

    return valid;
}

static void append_result(GString *text, const char *name,
                          SimulationResult result, int frame_count)
{
    g_string_append_printf(text, "\n%s\n", name);
    g_string_append(text, "----------------------------------------\n");
    g_string_append(text, "Page\tFrames\t\tStatus\n");

    for (int i = 0; i < result.total_references; i++) {
        SimulationStep step = result.steps[i];

        g_string_append_printf(text, "%d\t", step.page);

        for (int j = 0; j < frame_count; j++) {
            if (step.frames[j] == -1)
                g_string_append(text, "- ");
            else
                g_string_append_printf(text, "%d ", step.frames[j]);
        }

        g_string_append_printf(text, "\t\t%s\n",
                               step.is_hit ? "Hit" : "Fault");
    }

    g_string_append_printf(
        text,
        "\nTotal references: %d\nHits: %d\nPage faults: %d\n"
        "Hit ratio: %.2f%%\nFault ratio: %.2f%%\n",
        result.total_references,
        result.total_hits,
        result.total_faults,
        result.hit_ratio * 100.0,
        result.fault_ratio * 100.0
    );
}

static int graph_faults[3] = {0, 0, 0};
static const char *graph_names[3] = {"FIFO", "LRU", "Optimal"};

static gboolean draw_fault_graph(GtkWidget *widget, cairo_t *cr,
                                 gpointer data)
{
    (void)data;

    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    double width = allocation.width;
    double height = allocation.height;
    double left = 55.0;
    double top = 25.0;
    double bottom = height - 35.0;
    double chart_height = bottom - top;
    double chart_width = width - left - 25.0;
    int max_faults = 1;

    for (int i = 0; i < 3; i++) {
        if (graph_faults[i] > max_faults)
            max_faults = graph_faults[i];
    }

    cairo_set_source_rgb(cr, 0.12, 0.12, 0.12);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);
    cairo_move_to(cr, 10, 16);
    cairo_show_text(cr, "Page Fault Comparison");

    cairo_set_line_width(cr, 1);
    cairo_move_to(cr, left, top);
    cairo_line_to(cr, left, bottom);
    cairo_line_to(cr, width - 15, bottom);
    cairo_stroke(cr);

    double slot = chart_width / 3.0;
    double bar_width = slot * 0.48;
    double colors[3][3] = {
        {0.25, 0.48, 0.82},
        {0.95, 0.62, 0.20},
        {0.25, 0.68, 0.42}
    };

    for (int i = 0; i < 3; i++) {
        double bar_height = chart_height * graph_faults[i] / max_faults;
        double x = left + slot * i + (slot - bar_width) / 2.0;
        double y = bottom - bar_height;

        cairo_set_source_rgb(cr, colors[i][0], colors[i][1], colors[i][2]);
        cairo_rectangle(cr, x, y, bar_width, bar_height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.12, 0.12, 0.12);
        char value[16];
        g_snprintf(value, sizeof(value), "%d", graph_faults[i]);
        cairo_move_to(cr, x + bar_width / 2.0 - 4, y - 5);
        cairo_show_text(cr, value);

        cairo_move_to(cr, x + bar_width / 2.0 - 20, bottom + 20);
        cairo_show_text(cr, graph_names[i]);
    }

    return FALSE;
}

static void run_simulation(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    int pages[MAX_REFERENCES];
    int count = 0;
    int frame_count = (int)gtk_spin_button_get_value_as_int(
        GTK_SPIN_BUTTON(frames_spin));

    const char *input = gtk_entry_get_text(GTK_ENTRY(pages_entry));
    const char *algorithm = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo));

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(output_view));

    if (!parse_pages(input, pages, &count)) {
        gtk_text_buffer_set_text(
            buffer,
            "Invalid page-reference input.\n"
            "Enter 1 to 1000 non-negative page numbers separated by spaces or commas.",
            -1);
        g_free((gpointer)algorithm);
        return;
    }

    if (frame_count < 1 || frame_count > MAX_FRAMES) {
        gtk_text_buffer_set_text(buffer, "Frame count is invalid.", -1);
        g_free((gpointer)algorithm);
        return;
    }

    if (algorithm == NULL) {
        gtk_text_buffer_set_text(buffer, "Select an algorithm.", -1);
        return;
    }

    GString *text = g_string_new("PAGE REPLACEMENT SIMULATOR\n");

    if (strcmp(algorithm, "FIFO") == 0 ||
        strcmp(algorithm, "Compare All") == 0) {
        SimulationResult r = simulate_fifo(pages, count, frame_count);
        append_result(text, "FIFO", r, frame_count);
    }

    if (strcmp(algorithm, "LRU") == 0 ||
        strcmp(algorithm, "Compare All") == 0) {
        SimulationResult r = simulate_lru(pages, count, frame_count);
        append_result(text, "LRU", r, frame_count);
    }

    if (strcmp(algorithm, "Optimal") == 0 ||
        strcmp(algorithm, "Compare All") == 0) {
        SimulationResult r = simulate_optimal(pages, count, frame_count);
        append_result(text, "OPTIMAL", r, frame_count);
    }

    if (strcmp(algorithm, "Compare All") == 0) {
        SimulationResult f = simulate_fifo(pages, count, frame_count);
        SimulationResult l = simulate_lru(pages, count, frame_count);
        SimulationResult o = simulate_optimal(pages, count, frame_count);

        graph_faults[0] = f.total_faults;
        graph_faults[1] = l.total_faults;
        graph_faults[2] = o.total_faults;
        gtk_widget_queue_draw(graph_area);

        g_string_append(text, "\nCOMPARISON SUMMARY\n");
        g_string_append(text, "Algorithm\tHits\tFaults\n");
        g_string_append_printf(text, "FIFO\t\t%d\t%d\n",
                               f.total_hits, f.total_faults);
        g_string_append_printf(text, "LRU\t\t%d\t%d\n",
                               l.total_hits, l.total_faults);
        g_string_append_printf(text, "Optimal\t\t%d\t%d\n",
                               o.total_hits, o.total_faults);
    }

    gtk_text_buffer_set_text(buffer, text->str, -1);
    g_string_free(text, TRUE);
    g_free((gpointer)algorithm);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Page Replacement Algorithm Simulator");
    gtk_window_set_default_size(GTK_WINDOW(window), 850, 650);
    gtk_container_set_border_width(GTK_CONTAINER(window), 12);

    GtkWidget *outer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), outer);

    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title),
        "<span size='xx-large' weight='bold'>Page Replacement Simulator</span>");
    gtk_box_pack_start(GTK_BOX(outer), title, FALSE, FALSE, 5);

    GtkWidget *page_label = gtk_label_new(
        "Page references (e.g. 7 0 1 2 0 3 0 4 2 3):");
    gtk_widget_set_halign(page_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(outer), page_label, FALSE, FALSE, 0);

    pages_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(pages_entry),
                                   "Enter page numbers separated by spaces or commas");
    gtk_box_pack_start(GTK_BOX(outer), pages_entry, FALSE, FALSE, 0);

    GtkWidget *controls = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(outer), controls, FALSE, FALSE, 0);

    GtkWidget *frame_label = gtk_label_new("Number of frames:");
    gtk_box_pack_start(GTK_BOX(controls), frame_label, FALSE, FALSE, 0);

    frames_spin = gtk_spin_button_new_with_range(1, MAX_FRAMES, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(frames_spin), 3);
    gtk_box_pack_start(GTK_BOX(controls), frames_spin, FALSE, FALSE, 0);

    GtkWidget *algo_label = gtk_label_new("Algorithm:");
    gtk_box_pack_start(GTK_BOX(controls), algo_label, FALSE, FALSE, 0);

    algorithm_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "FIFO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "LRU");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Optimal");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(algorithm_combo), "Compare All");
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithm_combo), 3);
    gtk_box_pack_start(GTK_BOX(controls), algorithm_combo, FALSE, FALSE, 0);

    GtkWidget *run_button = gtk_button_new_with_label("Run Simulation");
    gtk_box_pack_start(GTK_BOX(controls), run_button, FALSE, FALSE, 0);
    g_signal_connect(run_button, "clicked",
                     G_CALLBACK(run_simulation), NULL);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(outer), scroll, TRUE, TRUE, 0);

    output_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(output_view), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(output_view), TRUE);
    gtk_container_add(GTK_CONTAINER(scroll), output_view);

    graph_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(graph_area, -1, 220);
    gtk_box_pack_start(GTK_BOX(outer), graph_area, FALSE, FALSE, 0);
    g_signal_connect(graph_area, "draw",
                     G_CALLBACK(draw_fault_graph), NULL);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
