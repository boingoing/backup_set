//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __BackupSetReader_h__
#define __BackupSetReader_h__

#include <iostream>

class BackupSet;

// A BackupSet may be serialized into a series of lines where each line
// begins with a 40-character sha1hash followed by a single space character
// followed by the filename and line terminator.
// This utility class can read a BackupSet from a buffer containing a serialized
// BackupSet as defined above.
class BackupSetReader {
 private:
  BackupSet& backup_set_;
  bool should_validate_ = false;

 public:
  BackupSetReader() = delete;
  explicit BackupSetReader(BackupSet& backup_set);
  ~BackupSetReader() = default;

  // Read lines from the input stream and store file information into the
  // BackupSet.
  void read(std::istream& is);

  // Enable validation of the input stream while reading.
  void enableValidation();
};

#endif  // __BackupSetReader_h__
