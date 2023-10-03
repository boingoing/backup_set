[![main build status](https://app.travis-ci.com/boingoing/backup_set.svg?branch=main)](https://app.travis-ci.com/github/boingoing/backup_set/builds)

## What is this?

A tool for computing the set of files missing between two backup sets and related code for reading, writing, and comparing such backup sets.

A backup set is defined as a file where each line contains a 40-digit sha1 hash followed by a single space character followed by a filename which may contain any number of slashes, backslashes, spaces, etc.

```console
1111111111111111111111111111111111111111 c:\file 1.txt
2222222222222222222222222222222222222222 c:\file 2.txt
3333333333333333333333333333333333333333 c:\file 3.txt
```

Files are identified and compared by their sha1 hash and their filename itself is arbitrary. A file with the same sha1 hash in both backup sets will be considered as the same file, regardless of the filenames of both files. Two files with the same filename but different sha1 hash are considered to be different files.

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
* `test_runner` is a simple unit test runner which contains and runs unit tests for the backup set implementation.
  * Supports a `--verbose` flag to control outputting a verbose trace log.
  * Supports a `--filter string` flag to control which unit tests are run. Filter strings are case-sensitive.
* `backup_set_compare` is a tool which can compute the set of files missing between old and new backup sets.
  * Supports a `--new filename` flag to choose the name of file containing the new backup set (Default: New.sha1.txt).
  * Supports a `--old filename` flag to choose the name of file containing the old backup set (Default: Old.sha1.txt).
  * Supports a `--writefiles` flag to control writing the set of missing filenames to output files. Otherwise the sets are written to the console.
  * Supports a `--validate` flag to enable validation of the backup set input files. When passsed, verifies that the sha1hash values are 40 valid hex-characters. Otherwise the sha1hash is treated as a unique string value.
    * Note: Lines in the input file which contain invalid sha1hash strings are ignored but no error is generated.

## Testing

```console
> test_runner
Simple unit test runner

Running BackupSetTest tests...

Total tests: 8
Passed tests: 8
Failed tests: 0
Disabled tests: 0
Skipped tests: 0
```

## Using

`Old.sha1.txt:`
```console
1111111111111111111111111111111111111111 c:\file 1.txt
2222222222222222222222222222222222222222 c:\file 2.txt
3333333333333333333333333333333333333333 c:\file 3.txt
```
`New.sha1.txt:`
```console
2222222222222222222222222222222222222222 c:\file 2.txt
3333333333333333333333333333333333333333 c:\file 3.txt
4444444444444444444444444444444444444444 c:\file 4.txt
```

```console
> backup_set_compare --new New.sha1.txt --old Old.sha1.txt
Backup set comparer. Determine which files are missing between two backup sets.

Files found in new but not present in old (NewNotInOld):
c:\file 4.txt

Files found in old but not present in new (OldNotInNew):
c:\file 1.txt

Done
```
