# myDB - Database Engine

**myDB is a key-value store that is inspired from Google's LevelDB architecture. It provides an ordered mapping from string keys to string values in the form of memtables (in-memory) and sstables (on-disk).**

Authors: Prakhar Sinha and Shorya Jain

# Features

- Keys and values are arbitrary byte arrays with maximum length 256 bytes each.
- The key-value pairs stored are sorted by keys.
- It is based on a client-server architecture. All the clients share the same data space, thereby providing the facility to read the data written by other clients.
- The basic operations supported are : `Put(key,value)`, `Get(key)`, `Delete(key)`.
- Since it is a key-value store, and not a SQL database, it does not support SQL queries.

# Architecture

TODO

# Cloning the source

`git clone --recurse-submodules https://github.com/prakhar2808/DBEngine`

# Building & Running

Build is supported only in POSIX environment.

- To build: `$make`
- To run the DB server : `$make run`
- To run a DB client : `$make clientrun`
- To clear all the data stored in the DB : `$make cleandata`

# Syntax

- To put a key **foo** and the corresponding value **bar** : `put <foo> <bar>`
- To get the value corresponding to a key **foo** : `get <foo>`
- To delete a key **foo** : `delete <foo>`
- To terminate the client process : `:exit`

# Future goals

- SSTables compaction

# Note

- The server and client codes are hardcoded to run on localhost. Modifications in the IP addresses and ports can be made in the files `server.cc` and `client.cc`
