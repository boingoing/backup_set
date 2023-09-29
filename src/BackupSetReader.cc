//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "BackupSetReader.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include "BackupSet.h"

BackupSetReader::BackupSetReader(BackupSet& backup_set) :
    backup_set_(backup_set) {}

void BackupSetReader::read(std::istream& is) {
  std::string line;
  std::string sha1hash;
  std::string filename;

  // Process one line at a time.
  while (std::getline(is, line)) {
    std::istringstream line_stream(line);

    // Read the sha1 hash and ' ' delimiter.
    if (line_stream >> sha1hash >> std::ws) {

      // Fetch the rest of the line as the filename.
      if (std::getline(line_stream, filename)) {
        // If that all parsed correctly, add the file to the set.
        backup_set_.addFile(sha1hash, filename);
      }
    }
  }
}
