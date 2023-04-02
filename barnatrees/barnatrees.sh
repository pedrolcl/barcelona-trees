#!/bin/bash

if [ ! -r barnatrees.txt ]; then
    sqlite3 -batch barnatrees.db 'select description from summary where name like "treedbcreator%"' > barnatrees.txt
fi

if [ ! -e barnatrees.db.7z ]; then
    7z a barnatrees.db.7z barnatrees.db
fi
