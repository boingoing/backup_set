//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "BackupSet.h"
#include "BackupSetReader.h"
#include "BackupSetWriter.h"

using Args = std::vector<std::string>;

constexpr const auto DefaultNewFilename = "New.sha1.txt";
constexpr const auto DefaultOldFilename = "Old.sha1.txt";
constexpr const auto DefaultNewNotInOldFilename = "NewNotInOld.txt";
constexpr const auto DefaultOldNotInNewFilename = "OldNotInNew.txt";

struct Options {
  std::string new_filename = DefaultNewFilename;
  std::string old_filename = DefaultOldFilename;
  bool write_files = false;
};

void printHelp() {
  std::cout << "Usage: backup_set_compare [--new filename] [--old filename] [--writefiles]" << std::endl << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << std::setw(2) << "" << std::left << std::setw(16) << "--new filename";
  std::cout << "Load the new backup set from filename (Default: " << std::quoted(DefaultNewFilename) << ")" << std::endl;
  std::cout << std::setw(2) << "" << std::left << std::setw(16) << "--old filename";
  std::cout << "Load the old backup set from filename (Default: " << std::quoted(DefaultOldFilename) << ")" << std::endl;
  std::cout << std::setw(2) << "" << std::left << std::setw(16) << "--writefiles";
  std::cout << "Write the sets of missing files between old and new backup sets to files." << std::endl;
  std::cout << std::setw(2) << "" << std::left << std::setw(16) << "--help";
  std::cout << "Display this usage information" << std::endl;
}

void parseArgs(const Args& args, Options& options) {
  for (auto iter = args.cbegin() + 1; iter != args.cend(); iter++) {
    const auto& arg = *iter;
    if (arg == "--new") {
      // If there are no more arguments, break out of the loop.
      if (++iter == args.cend()) {
        break;
      }
      options.new_filename = *iter;
    } else if (arg == "--old") {
      // If there are no more arguments, break out of the loop.
      if (++iter == args.cend()) {
        break;
      }
      options.old_filename = *iter;
    } else if (arg == "--writefiles") {
      options.write_files = true;
    } else if (arg == "--help") {
      printHelp();
      exit(0);
    } else {
      std::cout << "Unknown option: " << std::quoted(arg) << std::endl;
      printHelp();
      exit(-1);
    }
  }
}

void readFromFile(BackupSet& backup_set, const std::string& filename) {
  BackupSetReader reader(backup_set);
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  reader.read(ifs);
  ifs.close();
}

void writeToStream(const std::vector<std::string>& filenames, std::ostream& os) {
  for (const auto& f : filenames) {
    os << f << std::endl;
  }
}

void writeToFile(const std::vector<std::string>& filenames, const std::string& filename) {
  std::ofstream ofs;
  ofs.open(filename, std::ofstream::out);
  writeToStream(filenames, ofs);
  ofs.close();
}

int main(int argc, const char** argv) {
  std::cout << "Backup set comparer. Determine which files are missing between two backup sets." << std::endl << std::endl;

  // Convert argc/argv to a vector. 
  Args args;
  for (int i = 0; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  Options options;
  parseArgs(args, options);

  BackupSet new_set;
  BackupSet old_set;
  readFromFile(new_set, options.new_filename);
  readFromFile(old_set, options.old_filename);

  const auto new_not_in_old = old_set.getMissingFiles(new_set);
  const auto old_not_in_new = new_set.getMissingFiles(old_set);

  if (options.write_files) {
    writeToFile(new_not_in_old, DefaultNewNotInOldFilename);
    writeToFile(old_not_in_new, DefaultOldNotInNewFilename);
  } else {
    std::cout << "Files found in new but not present in old (NewNotInOld):" << std::endl;
    writeToStream(new_not_in_old, std::cout);
    std::cout << std::endl;

    std::cout << "Files found in old but not present in new (OldNotInNew):" << std::endl;
    writeToStream(old_not_in_new, std::cout);
    std::cout << std::endl;
  }

  std::cout << "Done" << std::endl;
  return 0;
}
