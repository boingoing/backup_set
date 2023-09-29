## What is this?

A tool for computing the set of files missing between two backup sets and related code for reading, writing, and comparing such backup sets.

A backup set is defined as a file where each line contains a 40-digit sha1 hash followed by a single space character followed by a filename which may contain any number of slashes, backslashes, spaces, etc.

```console
1111111111111111111111111111111111111111 c:\file 1.txt
2222222222222222222222222222222222222222 c:\file 2.txt
3333333333333333333333333333333333333333 c:\file 3.txt
```

## Building

This repo includes a cmake definition. It has been tested on Windows, linux, and MacOS.

```console
> git clone https://github.com/boingoing/backup_set/backup_set.git
> mkdir backup_set/build
> cd backup_set/build
> cmake ..
> make
```

## Running

There are two binaries which are defined in this repo.
* `test_runner` is a simple unit test runner which contains tests for the backup set implementation and runs defined unit tests.
  * Supports a `--verbose` flag to control outputting a verbose trace log.
  * Supports a `--filter string` flag to control which unit tests are run. Filter strings are case-sensitive.
* `backup_set_compare` is a tool which can compute the set of files missing between old and new backup sets.
  * Supports a `--new filename` flag to choose the name of file containing the new backup set (Default: New.sha1.txt)
  * Supports a `--old filename` flag to choose the name of file containing the old backup set (Default: Old.sha1.txt)
  * Supports a `--writefiles` flag to control writing the set of missing filenames to output files. Otherwise the sets are written to the console.
