#!/bin/bash
sqlite3 -batch barnatrees.db 'select description from summary where name like "treedbcreator%"' > barnatrees.txt

