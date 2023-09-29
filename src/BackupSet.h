//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __BackupSet_h__
#define __BackupSet_h__

#include <map>
#include <string>
#include <vector>

// Hold the details of a set of backup files.
// Each file consists of a full filesystem path and the sha1 hash of
// the file contents.
// File identity is determined by the sha1 hash regardless of the filename.
class BackupSet {
 private:
  std::map<std::string, std::string> hash_to_filename_map_;

  friend class BackupSetWriter;

 public:
  // Add a mapping from |sha1| => |filename| into the backup set.
  void addFile(const std::string& sha1, const std::string& filename);

  // Return the set of filenames which are found in |rhs| but not found in this.
  std::vector<std::string> getMissingFiles(const BackupSet& rhs);
};

#endif  // __BackupSet_h__
