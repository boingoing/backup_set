//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "BackupSetWriter.h"

#include <iomanip>
#include <iostream>
#include <map>

#include "BackupSet.h"

BackupSetWriter::BackupSetWriter(const BackupSet& backup_set) :
    backup_set_(backup_set) {}

void BackupSetWriter::write(std::ostream& os) {
  for (const auto& sha1_filename_pair : backup_set_.hash_to_filename_map_) {
      os << sha1_filename_pair.first << " " << sha1_filename_pair.second << std::endl;
  }
}
