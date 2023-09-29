//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#ifndef __BackupSetWriter_h__
#define __BackupSetWriter_h__

#include <iostream>

class BackupSet;

// A BackupSet may be serialized into a series of lines where each line
// begins with a 40-character sha1hash followed by a single space character
// followed by the filename and line terminator.
// This utility class can serialize a BackupSet into a buffer in the format
// as defined above.
class BackupSetWriter {
 private:
  const BackupSet& backup_set_;

 public:
  BackupSetWriter() = delete;
  explicit BackupSetWriter(const BackupSet& backup_set);
  ~BackupSetWriter() = default;

  void write(std::ostream& os);
};

#endif  // __BackupSetWriter_h__
