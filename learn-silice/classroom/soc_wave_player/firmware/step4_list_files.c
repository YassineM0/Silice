// @sylefeb 2022-01-10
// MIT license, see LICENSE_MIT in Silice repo root
// https://github.com/sylefeb/Silice/

#include "config.h"
#include "std.h"
#include "oled.h"
#include "display.h"
#include "printf.h"
#include "sdcard.h"

// include the fat32 library
#include "fat_io_lib/src/fat_filelib.h"

#define MAX_FILES 32
#define MAX_FILENAME_LEN 64

// Structure to store file information
typedef struct {
  char filename[MAX_FILENAME_LEN];
  int size;
} FileEntry;

FileEntry files[MAX_FILES];
int n_items = 0;

// Function to scan and store all files
void scan_files() {
  n_items = 0;
  const char *path = "/";
  FL_DIR dirstat;
  
  if (fl_opendir(path, &dirstat)) {
    struct fs_dir_ent dirent;
    while (fl_readdir(&dirstat, &dirent) == 0 && n_items < MAX_FILES) {
      if (!dirent.is_dir) {
        // Copy filename
        int i = 0;
        while (dirent.filename[i] && i < MAX_FILENAME_LEN - 1) {
          files[n_items].filename[i] = dirent.filename[i];
          i++;
        }
        files[n_items].filename[i] = '\0';
        files[n_items].size = dirent.size;
        n_items++;
      }
    }
    fl_closedir(&dirstat);
  }
}

void main()
{
  int selected = 0;
  int pulse = 0;
  int prev_buttons = 0;
  
  // turn LEDs off
  *LEDS = 0;
  
  // install putchar handler for printf
  f_putchar = display_putchar;
  
  // init screen
  oled_init();
  oled_fullscreen();
  oled_clear(0);
  
  // init sdcard
  sdcard_init();
  
  // initialise File IO Library
  fl_init();
  
  // attach media access functions to library
  display_set_cursor(0,0);
  display_set_front_back_color(255,0);
  printf("Initializing SD card...\n");
  display_refresh();
  
  while (fl_attach_media(sdcard_readsector, sdcard_writesector) != FAT_INIT_OK) {
    // keep trying, we need this
  }
  
  // Scan files once at startup
  scan_files();
  
  // Main menu loop
  while(1) {
    // Clear screen and draw header
    display_set_cursor(0,0);
    
    // Pulsing header
    display_set_front_back_color((pulse+127)&255, pulse);
    pulse += 7;
    printf("    ===== files =====    \n\n");
    
    // Display files
    display_set_front_back_color(255,0);
    if (n_items == 0) {
      printf("No files found!\n");
    } else {
      for (int i = 0; i < n_items; ++i) {
        if (i == selected) {
          // Highlight selected item
          display_set_front_back_color(0, 255);
        } else {
          display_set_front_back_color(255, 0);
        }
        printf("%d> %s\n", i, files[i].filename);
        printf("   [%d bytes]\n", files[i].size);
      }
    }
    
    display_refresh();
    
    // Read buttons with debouncing
    int curr_buttons = *BUTTONS;
    int button_press = curr_buttons & ~prev_buttons; // Detect rising edge
    
    if (button_press & (1<<3)) { 
      selected++;
    }
    if (button_press & (1<<4)) { 
      selected--;
    }
    
    // Wrap around selection
    if (selected < 0) {
      selected = n_items - 1;
    }
    if (selected >= n_items) {
      selected = 0;
    }
    
    prev_buttons = curr_buttons;
  }
}
