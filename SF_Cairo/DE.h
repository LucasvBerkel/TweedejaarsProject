#define TEXT_HEIGHT 4 // The height of character "h" in pixels in Cairo (with monospace font)
#define TEXT_WIDTH 3 // The height of character "h" in pixels (with monospace font)
#define SF_YELLOW 1.0, 1.0, 0.33
#define SF_GREEN 0.0, 0.66, 0.0
#define SF_BLUE 0.33, 1.0, 1.0
#define SF_ORANGE 1.0, 0.33, 0.33
#define WINDOW_HEIGHT 240
#define WINDOW_WIDTH 320

void Open_Graphics(void);

void Initialize_Graphics(cairo_t *cr);

void Close_Graphics(cairo_t *cr);
void Close_Graphics_SF();

float Fcos(int Headings_Degs);
float Fsin(int Headings_Degs);

void snapCoords(cairo_t *canvas, int x, int y);
void cairo_line(cairo_t *cr, int x1, int y1, int x2, int y2);
void cairo_text_at(cairo_t *cr, int x, int y, const char *string);

void clip_path_rect(cairo_t *cr);
void clear_prev_path(cairo_t *cr, cairo_path_t *prevPath);
void clean(cairo_t *cr);
void update_drawing(cairo_t *cr);

void Draw_Frame(cairo_t *cr);
void Draw_Hexagone(cairo_t *cr,int X_Center,int Y_Center,int Hex_Outer_Radius);
void Draw_Ship(cairo_t *cr, int x, int y, int Headings, int size);
void Draw_Fort(cairo_t *cr, int x, int y, int Headings, int size );
void Draw_Mine (cairo_t *cr, int x, int y, int size);
void Draw_Missile (cairo_t *cr, int x, int y, int Headings, int size);
void Draw_Shell(cairo_t *cr, int x, int y, int Headings, int size);

int Find_Headings(int x1,int y1,int x2,int y2);

void set_initial_vals(cairo_t *cr);
void start_drawing();

void stop_drawing();

int move_update();
void update_frame(cairo_t *cr);
unsigned char* update_frame_SF();

//static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
//void animation_loop(GtkWidget *darea);