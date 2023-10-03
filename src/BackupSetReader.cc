//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "BackupSetReader.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>

#include "BackupSet.h"

namespace {

// Returns true if |sha1hash| is a valid 40-character hex-string.
bool isValidSha1Hash(const std::string& sha1hash) {
  // Simple regex expects 40 valid hex-characters, case insensitive.
  const std::regex sha1hash_regex("^[a-f0-9]{40}$", std::regex::icase);
  std::smatch sha1hash_match;

  // See if the sha1hash matches the expected regex.
  if (std::regex_match(sha1hash, sha1hash_match, sha1hash_regex)) {
    return true;
  }

  // Didn't match the regex. Either the string wasn't 40-characters or
  // some of the characters were not valid in hex.
  return false;
}

}  // namespace

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
      // Validate the sha1hash is valid if we enabled doing that.
      if (should_validate_ && !isValidSha1Hash(sha1hash)) {
        continue;
      }

      // Fetch the rest of the line as the filename.
      if (std::getline(line_stream, filename)) {
        // If that all parsed correctly, add the file to the set.
        backup_set_.addFile(sha1hash, filename);
      }
    }
  }
}

void BackupSetReader::enableValidation() {
  should_validate_ = true;
}
