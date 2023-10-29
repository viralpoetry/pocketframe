#include <dirent.h>
#include <sys/stat.h>

#include <cstdlib>

#include "inkview.h"

static ifont *font;
static int y_log;
static bool debug = false;
static const int kFontSize = 12;
static const char *PICTURE_DIR = "/mnt/ext1/My pictures/PocketFrame/";

static void log_message(const char *msg) {
    if (strlen(msg) == 0 || debug) {
        return;
    }
    DrawTextRect(0, y_log, ScreenWidth(), kFontSize, msg, ALIGN_LEFT);
    PartialUpdate(0, y_log, ScreenWidth(), y_log + kFontSize + 2);
    y_log += kFontSize + 2;
}

int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

static int main_handler(int event_type, int param_one, int param_two) {
    if (EVT_INIT == event_type) {
        font = OpenFont("LiberationSans", kFontSize, 0);
        SetFont(font, BLACK);
        y_log = 0;
        ClearScreen();
        FullUpdate();
        bool are_we_sleeping = true;

        // Read the content of a directory
        // https://stackoverflow.com/a/612176
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(PICTURE_DIR)) != NULL) {
            // Print all the files and directories within directory
            while ((ent = readdir(dir)) != NULL) {
                char picfile[300];
                strcpy(picfile, PICTURE_DIR);
                strcat(picfile, ent->d_name);
                if (access(picfile, F_OK) == 0) {
                    // Load picture and write it to a buffer
                    if (is_regular_file(picfile)) {
                        ibitmap *picture =
                            LoadJPEG(picfile, ScreenWidth(), ScreenHeight(),
                                     100, 100, 1);
                        Stretch(picture->data, IMAGE_GRAY2, picture->width,
                                picture->height, picture->scanline, 0, 0,
                                picture->width, picture->height, 0);
                        log_message(picfile);
                        are_we_sleeping = true;
                    } else {
                        // Skip . & .. in directory listing
                        log_message(picfile);
                        are_we_sleeping = false;
                    }
                } else {
                    // File doesn't exist
                    log_message("File doesn't exist.");
                }
                if (are_we_sleeping) {
                    // Copy buffer to the real screen
                    FullUpdate();
                    // sleep(1200); // 20 mins
                    sleep(10);
                }
            }
            closedir(dir);
        } else {
            // Could not open directory
            log_message("Could not open directory.");
            // perror("Could not open directory.");
            return EXIT_FAILURE;
        }

        log_message("Picture stream ended.");

        CloseFont(font);
    } else if (EVT_KEYPRESS == event_type) {
        CloseApp();
    }
    return 0;
}

int main(int argc, char *argv[]) {
    InkViewMain(main_handler);
    return 0;
}
