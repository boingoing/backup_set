//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "BackupSet.h"

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

// Add a mapping from |sha1| => |filename| into the backup set.
void BackupSet::addFile(const std::string& sha1, const std::string& filename) {
  // Assume no collision.
  hash_to_filename_map_[sha1] = filename;
}

// Return the set of filenames which are found in |rhs| but not found in this.
std::vector<std::string> BackupSet::getMissingFiles(const BackupSet& rhs) {
  std::vector<std::string> missing;
  for (const auto& hash_filename_pair : rhs.hash_to_filename_map_) {
    const auto iter = hash_to_filename_map_.find(hash_filename_pair.first);
    if (iter == hash_to_filename_map_.cend()) {
      missing.push_back(hash_filename_pair.second);
    }
  }
  return missing;
}
