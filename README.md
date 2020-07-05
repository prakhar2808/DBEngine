# myDB - Database Engine

**myDB is a key-value store that is inspired from Google's LevelDB architecture. It provides an ordered mapping from string keys to string values in the form of memtables (in-memory) and sstables (on-disk).**

Authors: Prakhar Sinha and Shorya Jain

# Features

- Keys and values are arbitrary byte arrays with maximum length 256 bytes each.
- The key-value pairs stored are sorted by keys.
- It is based on a client-server architecture. All the clients share the same data space, thereby providing the facility to read the data written by other clients.
- The basic operations supported are : `Put(key,value)`, `Get(key)`, `Delete(key)`. Updates can be performed as `Put(key,new value)`.
- All keys are unique.
- Both key and value are non-null.
- Since it is a key-value store, and not a SQL database, it does not support SQL queries.

# Architecture
- The key-value store comprises of two data structures : <br>**1. An in-memory ordered map (memtable).** <br> **2. On-disk files containing encoded key-value pairs sorted by keys (sstables).**

- On receiving a **put \<foo> \<bar>** operation, the database server stores the key-value pair **\<foo, bar>** in the in-memory memtable.

- When the memtable's size reaches a certain threshold (say, 1000 key-value pairs), an asynchronous task is triggered to encode and dump the memtable on the disk as an sstable. Also, since the sstable is sorted by keys, an index is also created which is loaded in-memory and also dumped on the disk (so that it can be directly loaded again on restarting the server instead of parsing through each sstable). An in-memory bloom filter of the keys dumped in the sstable is also created and also dumped on the disk (same reason as the index). Meanwhile, the put operations received are continued in another memtable created in the memory.

- On receiving a **get \<foo>** request, the in-memory memtable is checked first for the presence of the key in it. If not found, the sstables are iterated back from the most recent one. In-memory bloom filter of each sstable is used to quickly verify whether the key might be present in that sstable or not. If the bloom filter suggests the presence of the key, the index is used to fetch the file block at a particular offset, thereby reducing disk I/O.

- On receiving a **delete \<foo>** request, a **put \<foo> \<>** operation is triggered, which sets the value for the key as null (indicating that the key has been deleted). Thus, on encountering a null value for a particular key during **get** operation, it would imply that the key is not present.

- Compaction can be performed on the sstables in the background, so as to remove multiple occurences of a particular key due to updation and deletion.

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
