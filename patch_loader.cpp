#include "SdFat.h"
#include <arduino.h>
#include <cstring>

#include "patch_loader.h"
#include "tfi_parser.h"

namespace thea {
namespace patch_loader {

#define MAX_FILE_NAME_SIZE 256

SdFatSdio sd;
SdFile current_file;

void init() {
  if (!sd.begin()) {
    Serial.println("Patch loader failed to start SD.");
    return;
  }
  sd.vwd()->rewind();
}

const char *get_extension(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if (!dot)
    return "\0\0\0";
  return dot + 1;
}

bool load_next_file_with_extension(SdFile *file, const char *extension) {
  char filename[MAX_FILE_NAME_SIZE];

  do {
    if (file->isOpen())
      file->close();
    bool success = file->openNext(sd.vwd(), O_READ);
    if (!success)
      sd.vwd()->rewind(); // wrap around.
    // TODO: Fix degenerative case where there isn't any files
    file->getName(filename, MAX_FILE_NAME_SIZE);
  } while (strncmp(extension, get_extension(filename), 3) != 0);

  return true;
}

bool load_nth(int n, thea::ym2612::ChannelPatch *patch) {
  sd.vwd()->rewind();
  for (; n >= 0; n--) {
    load_next_file_with_extension(&current_file, "tfi");
  }

  if (!current_file.isOpen()) {
    Serial.println("File is not open.");
    return false;
  }

  char filename[MAX_FILE_NAME_SIZE];
  current_file.getName(filename, MAX_FILE_NAME_SIZE);
  strncpy(patch->name, filename, 20);

  thea::tfi::parse(current_file, patch);

  Serial.printf("Loading patch %s\n", filename);

  return true;
};

} // namespace patch_loader
} // namespace thea
